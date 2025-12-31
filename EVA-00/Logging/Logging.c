#include "Logging.h"

static bool StartedLog = false;
static bool ErrorLT = false;

const char *EvaStateToString(EvaState_t Mode) {
    switch (Mode) {
        case EVA_RUNNING: return "RUNNING";
        case EVA_EMERGENCY_STOP: return "EMERGENCY_STOP";
        case EVA_FORCE_STOP: return "FORCE STOP";
        case EVA_SHUTDOWN: return "SHUTDOWN";

        default: return "UNKNOWN";
    }
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
        case 'E': Log_State(); break;
    }
    
    fflush(stdout);
}

void ALog_FullState() {
    switch (LoggingMode) {
        case 'P': PLog_PilotStatus(); break;
        case 'E': Log_FullState(); break;
    }
    
    fflush(stdout);
}

void ALog_Error() {
    switch (LoggingMode) {
        case 'P': PLog_IsError(); break;
        case 'E': Log_ErrorDetected(); break;
    }
    
    fflush(stdout);
}
