#include "State.h"

EvaState g_State;

void State_Reset(void) {
    g_State.PilotInput = 0.0f;

    g_State.PilotFB = 0.0f;
    g_State.PilotLR = 0.0f;
    g_State.PilotArm = 0.0f;

    g_State.SyncRatio = 0.0f;
    g_State.ActuatorOutput = 0.0f;
    g_State.RejectionCoefficient = 0.05f;

    g_State.ACTION_Attack = false;
    g_State.ACTION_Stop = false;

    g_State.FaultDetected = false;
    g_State.SystemState = EVA_RUNNING;

    g_State.Tick = 0;
}
