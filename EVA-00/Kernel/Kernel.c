#include "Kernel.h"

static float LastSync = 0.0f;

void UPDATE_Input(void) {
    //g_State.PilotInput = 0.50f;

    float T = g_State.Tick * 0.01f;
    float Jitter = ((rand() % 101) / 1000.0f) - 0.05f;

    g_State.PilotInput = 0.5f + 0.5f * sin(T) + Jitter;

    if (g_State.PilotInput > 1.0f) g_State.PilotInput = 1.0f;
    if (g_State.PilotInput < 0.0f) g_State.PilotInput = 0.0f;
}

void UPDATE_Sync(void) {
    double Base = 0.65f;
    double Oscillation = 0.05 * sin(0.02 * g_State.Tick);
    
    g_State.SyncRatio = (float)(Base + Oscillation);

    float EffectiveSync = g_State.SyncRatio * (1.0f - g_State.RejectionCoefficient);

    g_State.SyncRatio = EffectiveSync;

    if (g_State.SyncRatio > 1.0f) g_State.SyncRatio = 1.0f;
    if (g_State.SyncRatio < 0.0f) g_State.SyncRatio = 0.0f;
}

void UPDATE_Actuator(void) {
    float Target = g_State.PilotInput * g_State.SyncRatio;
    float HostileSmoothing = ACTUATOR_SMOOTHING * (1.0f - g_State.RejectionCoefficient);

    if (HostileSmoothing < 0.0f)
        HostileSmoothing = 0.01f;

    g_State.ActuatorOutput += HostileSmoothing * (Target - g_State.ActuatorOutput);

    if (g_State.ActuatorOutput > 1.0f) g_State.ActuatorOutput = 1.0f;
    if (g_State.ActuatorOutput < 0.0f) g_State.ActuatorOutput = 0.0f;
}

void UPDATE_RC(void) {
    if (g_State.PilotInput > 0.85f)
        g_State.RejectionCoefficient += 0.002f;

    if (fabs(g_State.SyncRatio - LastSync) > 0.05f)
        g_State.RejectionCoefficient += 0.001f;

    if (g_State.FaultDetected)
        g_State.RejectionCoefficient += 0.01f;

    g_State.RejectionCoefficient -= 0.0005f;
    LastSync = g_State.SyncRatio;

    if (g_State.RejectionCoefficient > 1.0f) g_State.RejectionCoefficient = 1.0f;
    if (g_State.RejectionCoefficient < 0.0f) g_State.RejectionCoefficient = 0.0f;
}

// CHANGE FUNCTIONS

void CHANGE_Input(float Input) {
    if (Input < 0.0f) Input = 0.0f;
    if (Input > 1.0f) Input = 1.0f;

    g_State.PilotInput = Input;
}

// Systems Updating

void Systems_Update(void) {
    PILOT_Update();

    UPDATE_Input();
    UPDATE_Sync();
    UPDATE_RC();
    UPDATE_Actuator();

    CHECK_Faults();

    if (g_State.FaultDetected == true)
        EVA__Emergency_Shutdown("");
}

void Systems_ShutdownTick(void) {
    g_State.ActuatorOutput -= 0.1f * g_State.ActuatorOutput;
    if (fabs(g_State.ActuatorOutput) < 0.001f)
        g_State.ActuatorOutput = 0.0f;

    g_State.PilotInput = 0.0f;
    g_State.PilotFB = 0.0f;
    g_State.PilotLR = 0.0f;
    g_State.PilotArm = 0.0f;

    g_State.SyncRatio = g_State.SyncRatio;
    g_State.FaultDetected = false;

    if (g_State.ActuatorOutput == 0.0f)
        g_State.SystemState = EVA_SHUTDOWN;
}

void Systems_EmergencyCut(void) {
    g_State.ActuatorOutput = 0.0f;

    g_State.PilotInput = 0.0f;
    g_State.PilotFB = 0.0f;
    g_State.PilotLR = 0.0f;
    g_State.PilotArm = 0.0f;

    g_State.FaultDetected = true;

    Log_Error(999);
}

// Actuator Functions

void ZERO_Actuators(void) {
    g_State.ActuatorOutput = 0.0f;
}

int Actuators_Saturated(void) {
    if (g_State.ActuatorOutput > 1.0f)
        return 1;
    
    return 0;
}

// EVANGELION FUNCTIONS

void EVA__Init(void) {
    PILOT_Init();
    State_Reset();
    Timebase_Init();

    g_State.PilotInput = 0.0f;
    g_State.ActuatorOutput = 0.0f;
    g_State.SyncRatio = 0.0f;
    g_State.FaultDetected = false;

    printf("=== EVA-00 Initialized ===\n");
}

void EVA__Shutdown(void) {
    if (g_State.SystemState == EVA_RUNNING) {
        g_State.SystemState = EVA_SHUTDOWN;

        printf("[EVA-00] Shutdown Requested\n");
    }
}

void EVA__Emergency_Shutdown(const char *Reason) {
    printf("[EVA-00] EMERGENCY SHUTDOWN COMMENCING: %s\n", Reason);

    g_State.SystemState = EVA_EMERGENCY_STOP;

    ZERO_Actuators();
}

void EVA__ForceStop(void) {
    printf("=== EVA-00 FORCE SHUTDOWN ===");

    g_State.SystemState = EVA_FORCE_STOP;
}

void EVA__Simulate(void) {
    if (g_State.Tick >= MAX_TICKS)
        g_State.SystemState = EVA_FORCE_STOP;
    
    if (g_State.SystemState == EVA_FORCE_STOP)
        exit(EXIT_FAILURE);
    
    if (g_State.SystemState == EVA_EMERGENCY_STOP)
        ZERO_Actuators();

    switch (g_State.SystemState) {
        case EVA_RUNNING:
            Systems_Update();
            
            break;

        case EVA_SHUTDOWN:
            Systems_ShutdownTick();

            break;
        
        case EVA_FORCE_STOP:
        case EVA_EMERGENCY_STOP:
            Systems_EmergencyCut();

            break;

        default:
            break;
    }

    Timebase_Tick();
    g_State.Tick++;
}

void CHECK_Faults(void) {
    if (g_State.ActuatorOutput > 1.0f || g_State.ActuatorOutput < -1.0f)
        g_State.FaultDetected = true;
    
    if (g_State.SyncRatio > 1.0f || g_State.SyncRatio < 0.0f)
        g_State.FaultDetected = true;
    
    if (g_State.PilotInput > 1.0f || g_State.PilotInput < 0.0f)
        g_State.FaultDetected = true;

    if (g_State.RejectionCoefficient > 0.7f && (rand() % 100) < 10)
        return;
    
    if (g_State.Tick >= MAX_TICKS)
        g_State.FaultDetected = true;

    if (Actuators_Saturated())
        EVA__Emergency_Shutdown("EVA SYSTEM READINGS MALFUCTION");
}
