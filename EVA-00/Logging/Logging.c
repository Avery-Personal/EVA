#include "Logging.h"

static bool StartedLog = false;
static bool ErrorLT = false;

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

        printf("[TICK %i] Pilot Input: %.2f | Sync: %.2f | Output: %.2f | FORW/BACK: %.2f | LEFT/RIGHT: %.2f | ARM HEIGHT: %.2f | Running: %s | Faulted: %s\n", g_State.Tick, g_State.PilotInput, g_State.SyncRatio, g_State.ActuatorOutput, g_State.PilotFB, g_State.PilotLR, g_State.PilotArm, g_State.RUNNING ? "True" : "False", g_State.FaultDetected ? "True" : "False");
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
