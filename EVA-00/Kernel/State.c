#include "State.h"

EvaState g_State;

void State_Reset(void) {
    g_State.PilotInput = 0.0f;

    g_State.PilotFB = 0.0f;
    g_State.PilotLR = 0.0f;
    g_State.PilotArm = 0.0f;

    g_State.SyncRatio = 0.0f;
    g_State.ActuatorOutput = 0.0f;
    g_State.ActuatorOutputLeft = g_State.PilotFB + g_State.PilotLR;
    g_State.ActuatorOutputRight = g_State.PilotFB - g_State.PilotLR;

    g_State.RejectionCoefficient = 0.05f;

    g_State.ACTION_Attack = false;
    g_State.ACTION_Stop = false;

    g_State.BatteryAmount = 100.0f;
    g_State.BatteryMode = EVA_BATTERY_PLUGGED;
    g_State.SystemState = EVA_RUNNING;

    g_State.FaultDetected = false;
    g_State.FaultCode = 0;
    g_State.FaultSeverity = 0;

    g_State.Tick = 0;
}
