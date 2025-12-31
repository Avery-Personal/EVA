#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>

#include "../Kernel/State.h"

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

#endif
