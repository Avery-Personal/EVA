#include "Kernel.h"

static float LastSync = 0.0f;

static float FB_Queue[MAX_LAG_TICKS];
static float LR_Queue[MAX_LAG_TICKS];
static int LagIndex = 0;

void UPDATE_Input(void) {
    float T = g_State.Tick * 0.01f;
    float Jitter = ((rand() % 101) / 1000.0f) - 0.05f;

    g_State.Pilot.Input = 0.5f + 0.5f * sin(T) + Jitter;

    if (g_State.Pilot.Input > 1.0f) g_State.Pilot.Input = 1.0f;
    if (g_State.Pilot.Input < 0.0f) g_State.Pilot.Input = 0.0f;
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

void UPDATE_RC(void) {
    if (g_State.Pilot.Input > 0.85f)
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

void UPDATE_Control(void) {
    float S = g_State.SyncRatio;
    float P = g_State.Pilot.Effectiveness;
    float C = g_State.Responsiveness;
    float D = g_State.DamageFactor;
    float T = g_State.Pilot.Trauma;

    float O = (S * P * C) * (1.0f - D) * (1.0f - T);

    if (O > 1.0f) O = 1.0f;
    if (O < 0.0f) O = 0.0f;

    g_State.Actuators.ActuatorOutput = O;
}

void UPDATE_Locomotion(void) {
    float C = g_State.Responsiveness;

    float TargetFB = g_State.Pilot.Move_X * g_State.Actuators.ActuatorOutput;
    float TargetLR = g_State.Pilot.Move_Y * g_State.Actuators.ActuatorOutput;

    float ResponseStep = C * (float) TICK_DURATION;
    if (ResponseStep > 1.0f) ResponseStep = 1.0f;

    g_State.Actuators.LimbActuators.Locomotion.VelocityFB += (TargetFB - g_State.Actuators.LimbActuators.Locomotion.VelocityFB) * ResponseStep;
    g_State.Actuators.LimbActuators.Locomotion.VelocityLR += (TargetLR - g_State.Actuators.LimbActuators.Locomotion.VelocityLR) * ResponseStep;

    int LagTicks = (int)((1.0f - C) * MAX_LAG_TICKS);
    if (LagTicks < 1) LagTicks = 1;

    FB_Queue[LagIndex] = g_State.Actuators.LimbActuators.Locomotion.VelocityFB;
    LR_Queue[LagIndex] = g_State.Actuators.LimbActuators.Locomotion.VelocityLR;

    int ReadIndex = (LagIndex + MAX_LAG_TICKS - LagTicks) % MAX_LAG_TICKS;

    g_State.Actuators.LimbActuators.Locomotion.VelocityFB = FB_Queue[ReadIndex];
    g_State.Actuators.LimbActuators.Locomotion.VelocityLR = LR_Queue[ReadIndex];

    LagIndex = (LagIndex + 1) % MAX_LAG_TICKS;
}

void UPDATE_Actuators(void) {
    g_State.Actuators.LimbActuators.LeftLeg.Position = g_State.Actuators.LimbActuators.Locomotion.VelocityFB * g_State.Actuators.ActuatorOutput;
    g_State.Actuators.LimbActuators.RightLeg.Position = g_State.Actuators.LimbActuators.Locomotion.VelocityFB * g_State.Actuators.ActuatorOutput;
    g_State.Actuators.LimbActuators.LeftLeg.ActualPower = fabsf(g_State.Actuators.LimbActuators.Locomotion.VelocityFB) * g_State.Actuators.ActuatorOutput;
    g_State.Actuators.LimbActuators.RightLeg.ActualPower = fabsf(g_State.Actuators.LimbActuators.Locomotion.VelocityFB) * g_State.Actuators.ActuatorOutput;
}

void UPDATE_TRAUMA(void) {

}

// CHANGE FUNCTIONS

void CHANGE_Input(float Input) {
    if (Input < 0.0f) Input = 0.0f;
    if (Input > 1.0f) Input = 1.0f;

    g_State.Pilot.Input = Input;
}

// Systems Updating

void Systems_Update(void) {
    PILOT_Update();

    UPDATE_Input();
    UPDATE_Sync();
    UPDATE_RC();

    UPDATE_Control();
    UPDATE_Locomotion();
    UPDATE_Actuators();

    CHECK_Faults();

    if (g_State.FaultDetected == true)
        Log_Error(g_State.FaultCode);
}

void Systems_ShutdownTick(void) {
    g_State.Actuators.ActuatorOutput -= 0.1f * g_State.Actuators.ActuatorOutput;
    if (fabs(g_State.Actuators.ActuatorOutput) < 0.001f)
        g_State.Actuators.ActuatorOutput = 0.0f;

    g_State.Pilot.Input = 0.0f;
    g_State.Pilot.Move_Y = 0.0f;
    g_State.Pilot.Move_X = 0.0f;

    g_State.Pilot.ArmL = 0.0f;
    g_State.Pilot.ArmR = 0.0f;

    g_State.SyncRatio = g_State.SyncRatio;
    g_State.FaultDetected = false;

    if (g_State.Actuators.ActuatorOutput == 0.0f)
        g_State.SystemState = EVA_SHUTDOWN;
}

void Systems_EmergencyCut(void) {
    g_State.Actuators.ActuatorOutput = 0.0f;

    g_State.Pilot.Input = 0.0f;
    g_State.Pilot.Move_Y = 0.0f;
    g_State.Pilot.Move_X = 0.0f;

    g_State.Pilot.ArmL = 0.0f;
    g_State.Pilot.ArmR = 0.0f;

    g_State.FaultDetected = true;

    Log_Error(999);
}

// Actuator Functions

void ZERO_Actuators(void) {
    g_State.Actuators.ActuatorOutput = 0.0f;
}

int Actuators_Saturated(void) {
    if (g_State.Actuators.ActuatorOutput > 1.0f)
        return 1;
        
    if (g_State.Actuators.ActuatorOutputLeft > 1.0f)
        return 1;
        
    if (g_State.Actuators.ActuatorOutputRight > 1.0f)
        return 1;
    
    return 0;
}

// EVANGELION FUNCTIONS

void EVA__Init(void) {
    PILOT_Init();
    State_Reset();
    Timebase_Init();

    g_State.Pilot.Input = 0.0f;
    g_State.Actuators.ActuatorOutput = 0.0f;
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

// Severity | 1 - Minor | 2 - Major | 3 - Shutdown
void ERROR_Log(int Severity) {
    if (Severity == 1) {
        Log_Error(g_State.FaultCode);
    } else if (Severity == 2) {
        if (Actuators_Saturated() || g_State.Pilot.Input < 0.0f || g_State.Pilot.Input > 0.0f) {
            printf("[EVA-00] MAJOR ERROR: EVA-Analysis Overflow/Pilot Input fualt\n");

            g_State.SystemState = EVA_EMERGENCY_STOP;

            ZERO_Actuators();
            exit(EXIT_FAILURE);
        }
    } else if (Severity == 3) {
        EVA__ForceStop();
    }
}

void CHECK_Faults(void) {
    if (g_State.Actuators.ActuatorOutput > 1.0f || g_State.Actuators.ActuatorOutput < -1.0f) {
        if (g_State.Actuators.ActuatorOutput > 1.0f)
            g_State.FaultCode = 1;
        
        if (g_State.Actuators.ActuatorOutput < -1.0f)
            g_State.FaultCode = 2;

        g_State.FaultDetected = true;
    }

    if (g_State.SyncRatio > 1.0f || g_State.SyncRatio < 0.0f) {
        if (g_State.SyncRatio > 1.0f)
            g_State.FaultCode = 3;
        
        if (g_State.SyncRatio < 0.0f)
            g_State.FaultCode = 4;

        g_State.FaultDetected = true;
    }

    if (g_State.Pilot.Input > 1.0f || g_State.Pilot.Input < 0.0f) {
        if (g_State.Pilot.Input > 1.0f)
            g_State.FaultCode = 5;
        
        if (g_State.Pilot.Input < 0.0f)
            g_State.FaultCode = 6;

        g_State.FaultDetected = true;
    }

    if (g_State.RejectionCoefficient > 0.7f && (rand() % 100) < 10) {
        g_State.FaultCode = 7;

        return;
    }
    
    if (g_State.Tick >= MAX_TICKS) {
        g_State.FaultDetected = true;
        g_State.FaultCode = 8;
    }

    if (Actuators_Saturated()) {
        g_State.FaultCode = 9;

        EVA__Emergency_Shutdown("EVA SYSTEM READINGS MALFUCTION");
    }
}
