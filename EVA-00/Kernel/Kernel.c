#include "Kernel.h"

void UPDATE_Input(void) {
    //g_State.PilotInput = 0.50f;

    float T = g_State.Tick * 0.01f;
    float Jitter = ((rand() % 101) / 1000.0f) - 0.05f;

    g_State.PilotInput = 0.5f + 0.5f * sin(T) + Jitter;

    if(g_State.PilotInput > 1.0f) g_State.PilotInput = 1.0f;
    if(g_State.PilotInput < 0.0f) g_State.PilotInput = 0.0f;
}

void UPDATE_Sync(void) {
    g_State.SyncRatio = 0.65f;
}

void UPDATE_Actuator(void) {
    g_State.ActuatorOutput = g_State.PilotInput * g_State.SyncRatio;
}

// CHANGE FUNCTIONS

void CHANGE_Input(float Input) {
    if (Input < 0.0f) Input = 0.0f;
    if (Input > 1.0f) Input = 1.0f;

    g_State.PilotInput = Input;
}

// EVANGELION FUNCTIONS

void EVA__Init(void) {
    State_Reset();
    Timebase_Init();

    printf("=== EVA-00 Initialized ===\n");
}

void EVA__Shutdown(void) {
    g_State.RUNNING = false;

    printf("=== EVA-00 Shutdown ===\n");
}

void EVA__Simulate(void) {
    if (g_State.Tick >= MAX_TICKS) {
        g_State.FORCE_STOP = true;
    } else if (g_State.FORCE_STOP == true) {
        exit(EXIT_FAILURE);
    }

    UPDATE_Input();
    UPDATE_Sync();
    UPDATE_Actuator();

    CHECK_Faults();

    if (g_State.FaultDetected == true)
        Log_Error(1);

    Timebase_Tick();
    g_State.Tick++;
}

void CHECK_Faults(void) {
    if (g_State.ActuatorOutput > 1.0f || g_State.ActuatorOutput < -1.0f) {
        g_State.FaultDetected = true;
    } else if (g_State.SyncRatio > 1.0f || g_State.SyncRatio < 0.0f) {
        g_State.FaultDetected = true;
    } else if (g_State.PilotInput > 1.0f || g_State.PilotInput < 0.0f) {
        g_State.FaultDetected = true;
    } else if (g_State.Tick >= MAX_TICKS) {
        g_State.FaultDetected = true;
    } else {
        g_State.FaultDetected = false;
    }
}
