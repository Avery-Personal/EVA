#include "State.h"

EvaState g_State;

void State_Reset(void) {
    g_State.Pilot.Input = 0.0f;

    g_State.Pilot.RawInput.Move_Y = 0.0f;
    g_State.Pilot.RawInput.Move_X = 0.0f;

    g_State.Pilot.RawInput.ArmL = 0.0f;
    g_State.Pilot.RawInput.ArmR = 0.0f;

    g_State.Pilot.FilteredInput.Move_Y = 0.0f;
    g_State.Pilot.FilteredInput.Move_X = 0.0f;

    g_State.Pilot.FilteredInput.ArmL = 0.0f;
    g_State.Pilot.FilteredInput.ArmR = 0.0f;

    g_State.Pilot.Output.Move_Y = 0.0f;
    g_State.Pilot.Output.Move_X = 0.0f;

    g_State.Pilot.Output.ArmL = 0.0f;
    g_State.Pilot.Output.ArmR = 0.0f;

    g_State.Pilot.Effectiveness = .85f;
    g_State.Pilot.Conscious = 1.0f;
    g_State.Pilot.Fatigue = 0.01f;

    g_State.SyncRatio = 0.0f;
    g_State.Actuators.ActuatorOutput = 0.0f;
    g_State.Actuators.ActuatorOutputLeft = g_State.Pilot.Output.Move_Y + g_State.Pilot.Output.Move_X;
    g_State.Actuators.ActuatorOutputRight = g_State.Pilot.Output.Move_Y - g_State.Pilot.Output.Move_X;

    g_State.RejectionCoefficient = 0.05f;

    g_State.ACTION_Attack = false;
    g_State.ACTION_Stop = false;

    g_State.DamageFactor = 0.0f;
    g_State.Responsiveness = 1.0f;

    g_State.BatteryLevel = 100.0f;
    g_State.BatteryMode = EVA_BATTERY_RACING;
    g_State.SystemState = EVA_RUNNING;

    g_State.Berserk = false;
    g_State.DummyPlug = false;

    g_State.FaultDetected = false;
    g_State.FaultCode = 0;
    g_State.FaultSeverity = 0;

    g_State.Tick = 0;
}
