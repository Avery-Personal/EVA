#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>

#include "../Kernel/State.h"
#include "../Config.h"

// Helpers

const char *EvaStateToString(EvaState_t Mode);

// Globalized Logging Data

void Log_State();
void Log_FullState();
void Log_Error(int Error);

// Pilot Logging Data

void PLog_Sync();
void PLog_IsError();
void PLog_PilotStatus();

void PLog_EVA_Arm_Test();
void PLog_EVA_Position();
void PLog_EVA_Actions();

// A - Automated Log (Determines whether if it's a EVA-00 Log, Pilot Log, or Static Log)
void ALog_State();
void ALog_FullState();
void ALog_Error();

#endif
