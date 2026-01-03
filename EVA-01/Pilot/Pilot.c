#include <stdio.h>
#include <stdlib.h>

#include <conio.h>

#include "Pilot.h"

#include "../Kernel/Kernel.h"
#include "../Logging/Logging.h"

static float SmoothedGain = 0.0f;

static float Smooth(float Previous, float Target) {
    return Previous + PILOT_SMOOTHING * (Target - Previous);
}

void PILOT_Init(void) {
    g_State.Pilot.RawInput.Move_X = 0.0f;
    g_State.Pilot.RawInput.Move_Y = 0.0f;

    g_State.Pilot.RawInput.ArmL = 0.0f;
    g_State.Pilot.RawInput.ArmR = 0.0f;
    
    g_State.Pilot.FilteredInput.Move_X = 0.0f;
    g_State.Pilot.FilteredInput.Move_Y = 0.0f;

    g_State.Pilot.FilteredInput.ArmL = 0.0f;
    g_State.Pilot.FilteredInput.ArmR = 0.0f;
    
    g_State.Pilot.Output.Move_X = 0.0f;
    g_State.Pilot.Output.Move_Y = 0.0f;

    g_State.Pilot.Output.ArmL = 0.0f;
    g_State.Pilot.Output.ArmR = 0.0f;

    g_State.Pilot.Input = 0;

    g_State.Pilot.Effectiveness = 0.0f;
    g_State.Pilot.Fatigue = 0.0f;
    g_State.Pilot.Conscious = 0.5f;

    g_State.Pilot.Trauma = 0.0f;
    g_State.Pilot.Stress = 0.0f;

    g_State.Pilot.InputBias = BASE_GAIN;
    g_State.Pilot.InputGain = 0.0f;

    g_State.ACTION_Attack = false;
    g_State.ACTION_Stop = false;

    printf("=== PILOT SYSTEM ONLINE ===\n");
}

void PILOT_Update(void) {
    PILOT_RawRead();
    PILOT_Filter();
    PILOT_UpdateCalibration();
    PILOT_ApplyCalibration();
    

    if (g_State.Pilot.Output.Move_Y > 1.0f) g_State.Pilot.Output.Move_Y = 1.0f;
    if (g_State.Pilot.Output.Move_Y < -1.0f) g_State.Pilot.Output.Move_Y = -1.0f;

    if (g_State.Pilot.Output.Move_X > 1.0f) g_State.Pilot.Output.Move_X = 1.0f;
    if (g_State.Pilot.Output.Move_X < -1.0f) g_State.Pilot.Output.Move_X = -1.0f;

    if (g_State.Pilot.Output.ArmL > 1.0f) g_State.Pilot.Output.ArmL = 1.0f;
    if (g_State.Pilot.Output.ArmL < -1.0f) g_State.Pilot.Output.ArmL = -1.0f;

    if (g_State.Pilot.Output.ArmR > 1.0f) g_State.Pilot.Output.ArmR = 1.0f;
    if (g_State.Pilot.Output.ArmR < -1.0f) g_State.Pilot.Output.ArmR = -1.0f;
}

void PILOT_Shutdown(void) {
    printf("=== PILOT SYSTEM OFFLINE ===");
}

void PILOT_ApplyBias(void) {
    float SyncDrift = sinf(Timebase_Get() * 0.5f) * 0.01f;

    g_State.Pilot.InputBias = (g_State.Pilot.Stress * 0.03f) + (g_State.Pilot.Trauma * 0.02f) + SyncDrift;
}

void PILOT_ApplyGain(void) {
    float TargetGain = BASE_GAIN * g_State.SyncRatio * g_State.Pilot.Conscious * (1.0f - g_State.Pilot.Fatigue) * (1.0f - g_State.Pilot.Trauma) * g_State.Responsiveness;

    SmoothedGain += GAIN_SMOOTH * (TargetGain - SmoothedGain);
    g_State.Pilot.InputGain = SmoothedGain;
}

void PILOT_RawRead(void) {
    g_State.ACTION_Attack = false;
    g_State.ACTION_Stop = false;

    if (!_kbhit())
        return;

    char Key = _getch();

    switch (Key) {
        case 'w': g_State.Pilot.RawInput.Move_Y += INPUT_GAIN; break;
        case 's': g_State.Pilot.RawInput.Move_Y -= INPUT_GAIN; break;
        
        case 'a': g_State.Pilot.RawInput.Move_X -= INPUT_GAIN; break;
        case 'd': g_State.Pilot.RawInput.Move_X += INPUT_GAIN; break;

        case 'e': g_State.Pilot.RawInput.ArmL += INPUT_GAIN;
                  g_State.Pilot.RawInput.ArmR += INPUT_GAIN;

                  break;
        case 'q': g_State.Pilot.RawInput.ArmL -= INPUT_GAIN;
                  g_State.Pilot.RawInput.ArmR -= INPUT_GAIN;

                  break;
        
        case 't': g_State.Pilot.RawInput.ArmL += INPUT_GAIN; break;
        case 'g': g_State.Pilot.RawInput.ArmL -= INPUT_GAIN; break;
        
        case 'y': g_State.Pilot.RawInput.ArmR += INPUT_GAIN; break;
        case 'h': g_State.Pilot.RawInput.ArmR -= INPUT_GAIN; break;

        case ' ': g_State.ACTION_Attack = true; break;
        case 'x': g_State.ACTION_Stop = true; break;

        case 27:
            g_State.SystemState = EVA_SHUTDOWN;
            
            Log_Error(99);
            break;
        
        default:
            break;
    }

    g_State.Actuators.ActuatorOutputLeft = g_State.Pilot.RawInput.Move_Y + g_State.Pilot.RawInput.Move_X;
    g_State.Actuators.ActuatorOutputRight = g_State.Pilot.RawInput.Move_Y - g_State.Pilot.RawInput.Move_X;

    g_State.Pilot.RawInput.Move_X *= INPUT_DECAY;
    g_State.Pilot.RawInput.Move_Y *= INPUT_DECAY;

    g_State.Pilot.RawInput.ArmL *= INPUT_DECAY;
    g_State.Pilot.RawInput.ArmR *= INPUT_DECAY;

    if (g_State.Actuators.ActuatorOutputLeft > 1.0f) g_State.Actuators.ActuatorOutputLeft = 1.0f;
    if (g_State.Actuators.ActuatorOutputLeft < 0.0f) g_State.Actuators.ActuatorOutputLeft = 0.0f;

    if (g_State.Actuators.ActuatorOutputRight > 1.0f) g_State.Actuators.ActuatorOutputRight = 1.0f;
    if (g_State.Actuators.ActuatorOutputRight < 0.0f) g_State.Actuators.ActuatorOutputRight = 0.0f;
}

void PILOT_Filter(void) {
    g_State.Pilot.FilteredInput.Move_X = Smooth(g_State.Pilot.FilteredInput.Move_X, g_State.Pilot.RawInput.Move_X);
    g_State.Pilot.FilteredInput.Move_Y = Smooth(g_State.Pilot.FilteredInput.Move_Y, g_State.Pilot.RawInput.Move_Y);
    
    g_State.Pilot.FilteredInput.ArmL = Smooth(g_State.Pilot.FilteredInput.ArmL, g_State.Pilot.RawInput.ArmL);
    g_State.Pilot.FilteredInput.ArmR = Smooth(g_State.Pilot.FilteredInput.ArmR, g_State.Pilot.RawInput.ArmR);
}

void PILOT_UpdateCalibration(void) {
    PILOT_ApplyBias();
    PILOT_ApplyGain();

    if (g_State.Pilot.InputGain < 0.05f)
        g_State.Pilot.InputGain *= 0.95f;

    if (g_State.SyncRatio > 0.9f && !g_State.DummyPlug)
        g_State.Pilot.InputGain *= 1.2f;
}

void PILOT_ApplyCalibration(void) {
    g_State.Pilot.Output.Move_X = g_State.Pilot.FilteredInput.Move_X * g_State.Pilot.InputGain + g_State.Pilot.InputBias;
    g_State.Pilot.Output.Move_Y = g_State.Pilot.FilteredInput.Move_Y * g_State.Pilot.InputGain + g_State.Pilot.InputBias;
    
    g_State.Pilot.Output.ArmL = g_State.Pilot.FilteredInput.ArmL * g_State.Pilot.InputGain + g_State.Pilot.InputBias;
    g_State.Pilot.Output.ArmR = g_State.Pilot.FilteredInput.ArmR * g_State.Pilot.InputGain + g_State.Pilot.InputBias;
}
