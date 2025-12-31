#include <stdio.h>
#include <stdlib.h>

#include <conio.h>

#include "Pilot.h"

#include "../Kernel/Kernel.h"
#include "../Logging/Logging.h"

void PILOT_Init(void) {
    g_State.PilotFB = 0.0f;
    g_State.PilotLR = 0.0f;
    g_State.PilotArm = 0.0f;

    g_State.ACTION_Attack = false;
    g_State.ACTION_Stop = false;

    printf("=== PILOT SYSTEM ONLINE ===\n");
}

void PILOT_Update(void) {
    g_State.ACTION_Attack = false;
    g_State.ACTION_Stop = false;
    
    if (!_kbhit)
        return;

    char Key = _getch();

    switch (Key) {
        case 'w': g_State.PilotFB += 0.1f; break;
        case 's': g_State.PilotFB -= 0.1f; break;
        
        case 'a': g_State.PilotLR -= 0.1f; break;
        case 'd': g_State.PilotLR += 0.1f; break;

        case 'e': g_State.PilotArm += 0.1f; break;
        case 'q': g_State.PilotArm -= 0.1f; break;

        case ' ': g_State.ACTION_Attack = true; break;
        case 'x': g_State.ACTION_Stop = true; break;

        case 27:
            g_State.FORCE_STOP = true;
            
            Log_Error(99);
            break;
        
        default:
            break;
    }

    if (g_State.PilotFB > 1.0f) g_State.PilotFB = 1.0f;
    if (g_State.PilotFB < -1.0f) g_State.PilotFB = -1.0f;

    if (g_State.PilotLR > 1.0f) g_State.PilotLR = 1.0f;
    if (g_State.PilotLR < -1.0f) g_State.PilotLR = -1.0f;

    if (g_State.PilotArm > 1.0f) g_State.PilotArm = 1.0f;
    if (g_State.PilotArm < -1.0f) g_State.PilotArm = -1.0f;
}

void PILOT_Shutdown(void) {
    printf("=== PILOT SYSTEM OFFLINE ===");
}
