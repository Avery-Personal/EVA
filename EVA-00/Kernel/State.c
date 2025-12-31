#include "State.h"

EvaState g_State;

void State_Reset(void) {
    g_State.PilotInput = 0.0f;

    g_State.SyncRatio = 0.0f;
    g_State.ActuatorOutput = 0.0f;

    g_State.FaultDetected = false;

    g_State.RUNNING = true;
    g_State.FORCE_STOP = false;

    g_State.Tick = 0;
}
