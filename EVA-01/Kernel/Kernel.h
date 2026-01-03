#ifndef KERNEL_H
#define KERNEL_H

#include <stdlib.h>
#include <math.h>

#include "Timing/Timebase.h"
#include "State.h"
#include "../Logging/Logging.h"
#include "../Config.h"
#include "../Pilot/Pilot.h"
#include "../Diagnostics/Diagnostics.h"

// Constants

#define ACTUATOR_SMOOTHING 0.1f

// Updates

void UPDATE_Battery(void);
void UPDATE_Input(void);
void UPDATE_Sync(void);
void UPDATE_RC(void);

// Actuators

void UPDATE_Control(void);

// Pilot

void UPDATE_Trauma(void);
void UPDATE_Stress(void);
void UPDATE_Locomotion(void);

// Input Changes

void CHANGE_Input(float Input);

// EVA FUNCTIONS

void EVA__Init(void);
void EVA__Shutdown(void);
void EVA__Emergency_Shutdown(const char *Reason);
void EVA__ForceStop(void);

// Helper function to reduce later boilercode in testing
void EVA__Simulate(void);

// Systems

void Systems_Update(void);
void Systems_ShutdownTick(void);
void Systems_EmergencyCut(void);

// Error System

void ERROR_Log(int Severity);
void CHECK_Faults(void);

#endif
