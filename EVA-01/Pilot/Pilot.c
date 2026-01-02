#include <stdio.h>
#include <stdlib.h>

#include <conio.h>

#include "Pilot.h"

#include "../Kernel/Kernel.h"
#include "../Logging/Logging.h"

void PILOT_Init(void) {
    g_State.Pilot.Move_X = 0.0f;
    g_State.Pilot.Move_Y = 0.0f;

    g_State.Pilot.ArmL = 0.0f;
    g_State.Pilot.ArmR = 0.0f;

    g_State.Pilot.Input = 0;

    g_State.Pilot.Effectiveness = 0.0f;
    g_State.Pilot.Fatigue = 0.0f;
    g_State.Pilot.Conscious = 0.5f;

    g_State.Pilot.Trauma = 0.0f;
    g_State.Pilot.Stress = 0.0f;

    g_State.Pilot.InputBias = 0.0f;
    g_State.Pilot.InputGain = 0.0f;

    g_State.Pilot.Move_Y = 0.0f;
    g_State.Pilot.Move_X = 0.0f;

    g_State.Pilot.ArmL = 0.0f;
    g_State.Pilot.ArmR = 0.0f;

    g_State.ACTION_Attack = false;
    g_State.ACTION_Stop = false;

    printf("=== PILOT SYSTEM ONLINE ===\n");
}

void PILOT_Update(void) {
    g_State.ACTION_Attack = false;
    g_State.ACTION_Stop = false;

    if (!_kbhit())
        return;

    char Key = _getch();

    switch (Key) {
        case 'w': g_State.Pilot.Move_Y += 0.1f; break;
        case 's': g_State.Pilot.Move_Y -= 0.1f; break;
        
        case 'a': g_State.Pilot.Move_X -= 0.1f; break;
        case 'd': g_State.Pilot.Move_X += 0.1f; break;

        case 'e': g_State.Pilot.ArmL += 0.1f; g_State.Pilot.ArmR += 0.1f; break;
        case 'q': g_State.Pilot.ArmL -= 0.1f; g_State.Pilot.ArmR -= 0.1f; break;
        
        case 't': g_State.Pilot.ArmL += 0.1f; break;
        case 'g': g_State.Pilot.ArmL -= 0.1f; break;
        
        case 'y': g_State.Pilot.ArmR += 0.1f; break;
        case 'h': g_State.Pilot.ArmR -= 0.1f; break;

        case ' ': g_State.ACTION_Attack = true; break;
        case 'x': g_State.ACTION_Stop = true; break;

        case 27:
            g_State.SystemState = EVA_SHUTDOWN;
            
            Log_Error(99);
            break;
        
        default:
            break;
    }

    g_State.Actuators.ActuatorOutputLeft = g_State.Pilot.Move_Y + g_State.Pilot.Move_X;
    g_State.Actuators.ActuatorOutputRight = g_State.Pilot.Move_Y - g_State.Pilot.Move_X;

    if (g_State.Pilot.Move_Y > 1.0f) g_State.Pilot.Move_Y = 1.0f;
    if (g_State.Pilot.Move_Y < -1.0f) g_State.Pilot.Move_Y = -1.0f;

    if (g_State.Pilot.Move_X > 1.0f) g_State.Pilot.Move_X = 1.0f;
    if (g_State.Pilot.Move_X < -1.0f) g_State.Pilot.Move_X = -1.0f;

    if (g_State.Pilot.ArmL > 1.0f) g_State.Pilot.ArmL = 1.0f;
    if (g_State.Pilot.ArmL < -1.0f) g_State.Pilot.ArmL = -1.0f;

    if (g_State.Pilot.ArmR > 1.0f) g_State.Pilot.ArmR = 1.0f;
    if (g_State.Pilot.ArmR < -1.0f) g_State.Pilot.ArmR = -1.0f;

    if (g_State.Actuators.ActuatorOutputLeft > 1.0f) g_State.Actuators.ActuatorOutputLeft = 1.0f;
    if (g_State.Actuators.ActuatorOutputLeft < 0.0f) g_State.Actuators.ActuatorOutputLeft = 0.0f;

    if (g_State.Actuators.ActuatorOutputRight > 1.0f) g_State.Actuators.ActuatorOutputRight = 1.0f;
    if (g_State.Actuators.ActuatorOutputRight < 0.0f) g_State.Actuators.ActuatorOutputRight = 0.0f;
}

void PILOT_Shutdown(void) {
    printf("=== PILOT SYSTEM OFFLINE ===");
}
