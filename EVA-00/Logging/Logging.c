#include "Logging.h"

static bool StartedAutoLog = false;
static bool StartedLog = false;
static bool ErrorLT = false;

const char *RED = "\033[0;31m";
const char *GREEN = "\033[0;32m";
const char *YELLOW = "\033[0;33m";
const char *BLUE = "\033[0;34m";
const char *RESET = "\033[0m";

const char *EvaStateToString(EvaState_t Mode) {
    switch (Mode) {
        case EVA_RUNNING: return "RUNNING";
        case EVA_EMERGENCY_STOP: return "EMERGENCY_STOP";
        case EVA_FORCE_STOP: return "FORCE STOP";
        case EVA_SHUTDOWN: return "SHUTDOWN";

        default: return "UNKNOWN";
    }
}

const char *EvaBatteryToString(EvaBatteryMode Mode) {
    switch (Mode) {
        case EVA_BATTERY_PLUGGED: return "PLUGGED";
        case EVA_BATTERY_SLOW: return "SLOW";
        case EVA_BATTERY_NORMAL: return "NORMAL";
        case EVA_BATTERY_RACING: return "RACING";

        default: return "UNKNOWN";
    }
}

const char *EvaErrorCodeToString() {
    switch (g_State.FaultCode) {
        case 1: return "ACTUATOR LOAD SATURATION";
        case 2: return "ACTUATOR STRUCTURAL FRACTURE";
        case 3: return "SYNCAGE LOAD SATURATION";
        case 4: return "SYNCAGE STRUCTURAL FRACTURE";
        case 5: return "PILOT INPUT AMPLIFICATION FAULT";
        case 6: return "PILOT SIGNAL DEGRADATION";
        case 7: return "NERUAL REJECTION SPIKE";
        case 8: return "KERNEL TICK DESYNCHRONIZATION";
        case 9: return "SYSTEM SENSOR INCOHERENCE";
        
        default: return "CLEAR";
    }
}

const char *ColoredLog(const char *Message, const char *Color) {
    printf("%s%s%s\n", Color, Message, RESET);
}

void ClearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void Log_State() {
    if (StartedLog == false && ErrorLT == false) {
        printf("=== EVA-00 STATISTICS ===\n\n");

        StartedLog = true;
    }

    if (g_State.FaultDetected == false) {
        if (ErrorLT == true)
            ErrorLT = false;

        printf("[TICK %i] Pilot Input: %.2f | Sync: %.2f | Output: %.2f | Faulted: %s\n", g_State.Tick, g_State.PilotInput, g_State.SyncRatio, g_State.ActuatorOutput, g_State.FaultDetected ? "True" : "False");
    } else if (g_State.FaultDetected == true && ErrorLT == false) {
        Log_Error(1);

        ErrorLT = true;
    }

    fflush(stdout);
}

void Log_FullState() {
    if (StartedLog == false && ErrorLT == false) {
        printf("=== EVA-00 STATISTICS ===\n\n");

        StartedLog = true;
    }

    if (g_State.FaultDetected == false) {
        if (ErrorLT == true)
            ErrorLT = false;

        printf("[TICK %i] Pilot Input: %.2f | Sync: %.2f | Output: %.2f | RC: %.2f | FORW/BACK: %.2f | LEFT/RIGHT: %.2f | ARM HEIGHT: %.2f | System State: %s | Faulted: %s\n", g_State.Tick, g_State.PilotInput, g_State.SyncRatio, g_State.ActuatorOutput, g_State.RejectionCoefficient, g_State.PilotFB, g_State.PilotLR, g_State.PilotArm, EvaStateToString(g_State.SystemState), g_State.FaultDetected ? "True" : "False");
    } else if (g_State.FaultDetected == true && ErrorLT == false) {
        Log_Error(1);

        ErrorLT = true;
    }

    fflush(stdout);
}

void Log_Error(int Error) {
    if (ErrorLT == false && g_State.FaultDetected == true) {
        printf("EVA-00 ERROR CODE %i\n", Error);
        
        fflush(stdout);

        ErrorLT = true;
    }
}

void Log_ErrorDetected() {
    printf("[ERROR ANALYSIS] Faulted: %s\n", g_State.FaultDetected ? "YES" : "NO");

    fflush(stdout);
}

// Evangelion Logging

void ELog_State() {
    printf("STATE: %s\n", EvaStateToString(g_State.SystemState));

    fflush(stdout);
}

void ELog_Life() {
    printf("BATTERY MODE: %s | BATTERY: %i\n", EvaBatteryToString(g_State.BatteryMode), g_State.BatteryAmount);

    fflush(stdout);
}

void ELog_HardwareStatistics() {
    printf("LIFETIME: %i | FB MOVEMENT: %.4f | LR MOVEMENT: %.4f | ARM MOVEMENT: %.4f | STATUS: %s | BATTERY LIFE: %.4f | ERROR DETECTED: %s\n", g_State.Tick, g_State.PilotFB, g_State.PilotLR, g_State.PilotArm, EvaStateToString(g_State.SystemState), g_State.BatteryAmount, g_State.FaultDetected ? "YES" : "NO");

    fflush(stdout);
}

void ELog_Error() {
    printf("ERRORS: %s\n", g_State.FaultDetected ? "YES" : "NO");

    fflush(stdout);
}

// Pilot Logging

void PLog_Sync() {
    printf("Pilot Sync: %.2f\n", g_State.SyncRatio);

    fflush(stdout);
}

void PLog_IsError() {
    printf("Eva Unit 00 Error(s): %s\n", g_State.FaultDetected ? "YES": "NO");

    fflush(stdout);
}

void PLog_PilotStatus() {
    printf("[EVA-00] Pilot Overall: %.2f | Sync: %.2f\n", g_State.PilotInput, g_State.SyncRatio);

    fflush(stdout);
}

void PLog_EVA_Arm_Test() {
    printf("[EVA-00] Center-Arm: %.2f\n", g_State.PilotArm);
}

void PLog_EVA_Position() {
    if (g_State.PilotFB >= 0.0f && g_State.PilotLR >= 0.0f)
        printf("[EVA-00] FORWARD: %.2f | RGHT: %.2f\n", g_State.PilotFB, g_State.PilotLR);
        
    if (g_State.PilotFB >= 0.0f && g_State.PilotLR <= 0.0f)
        printf("[EVA-00] FORWARD: %.2f | LEFT: %.2f\n", g_State.PilotFB, g_State.PilotLR);
        
    if (g_State.PilotFB <= 0.0f && g_State.PilotLR <= 0.0f)
        printf("[EVA-00] BCKWARD: %.2f | LEFT: %.2f\n", g_State.PilotFB, g_State.PilotLR);
        
    if (g_State.PilotFB <= 0.0f && g_State.PilotLR >= 0.0f)
        printf("[EVA-00] BCKWARD: %.2f | RGHT: %.2f\n", g_State.PilotFB, g_State.PilotLR);

    fflush(stdout);
}

void PLog_EVA_Actions() {
    printf("[EVA-00] Attacking: %s | Stopping: %s\n", g_State.ACTION_Attack ? "YES" : "NO", g_State.ACTION_Stop ? "YES" : "NO");

    fflush(stdout);
}

// Automated Logging

void ALog_State() {
    switch (LoggingMode) {
        case 'P': PLog_PilotStatus(); break;
        case 'E': ELog_State(); break;
        case 'S': Log_State(); break;
    }
    
    fflush(stdout);
}

void ALog_FullState() {
    switch (LoggingMode) {
        case 'P': PLog_PilotStatus(); break;
        case 'E': ELog_HardwareStatistics(); break;
        case 'S': Log_FullState(); break;
    }
    
    fflush(stdout);
}

void ALog_Error() {
    switch (LoggingMode) {
        case 'P': PLog_IsError(); break;
        case 'E': ELog_Error(); break;
        case 'S': Log_ErrorDetected(); break;
    }
    
    fflush(stdout);
}

/*

EVA SYSTEM TERMINAL
===================
UNIT: EVA-00
TICK: 6272
===================
SYNC RATIO: 72.7%
PILOT INPUT: AX=+0.78 AY=-0.12
ACTUATOR OUTPUT: L=0.94 R=0.94
==================
STATUS FLAGS

ACTUATOR LOAD HIGH (LEFT LEG)
RESPONSE LATENCY +12ms

ACTION REQUIRED
PILOT CORRECTION ADVISED

*/

const char *VLog_SyncHelper() {
    if (g_State.SyncRatio >= 0.65f) {
        return GREEN;
    } else if (g_State.SyncRatio >= 0.25f && g_State.SyncRatio < 0.65f) {
        return YELLOW;
    } else if (g_State.SyncRatio >= 0.0f && g_State.SyncRatio < 0.25f) {
        return RED;
    }
}

const char *VLog_FBHelper() {
    if (g_State.PilotFB > 0.0f)
        return "+";

    return " ";
}

const char *VLog_LRHelper() {
    if (g_State.PilotLR > 0.0f)
        return "+";

    return " ";
}

void VLog_Auto() {
    ClearScreen();

    if (!StartedAutoLog && !ErrorLT) {
        printf("EVA SYSTEM TERMINAL\n");
        printf("===================\n");

        StartedAutoLog = true;
    }

    printf("UNIT: %sEVA-00%s\n", BLUE, RESET);
    printf("TICK: %i\n", g_State.Tick);
    printf("===================\n");
    printf("%sSYNC RATIO: %.2f%s\n", VLog_SyncHelper(), g_State.SyncRatio, RESET);
    printf("PILOT INPUT: AX=%s%.2f AY=%s%.2f\n", VLog_LRHelper(), g_State.PilotLR, VLog_FBHelper(), g_State.PilotFB);
    printf("ACTUATOR OUTPUT: L=%.2f R=%.2f\n", g_State.ActuatorOutputLeft, g_State.ActuatorOutputRight);

    if (g_State.FaultDetected) {
        printf("===================\n");
        printf("STATUS FLAGS\n\n");
        printf("%s\n", EvaErrorCodeToString());

        // 1, 2, 5, 6, 7, 9
        if (g_State.FaultCode != 3 || g_State.FaultCode != 4 || g_State.FaultCode != 8) {
            printf("\nACTION REQUIRED\n");

            if (g_State.FaultCode == 5 || g_State.FaultCode == 6 || g_State.FaultCode == 7)
                printf("PILOT CORRECTION ADVISED\n");
        }
    }
}
