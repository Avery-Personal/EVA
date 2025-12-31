#ifndef KERNEL_H
#define KERNEL_H

#include <stdlib.h>
#include <math.h>

#include "Timing/Timebase.h"
#include "State.h"
#include "../Logging/Logging.h"
#include "../Config.h"

// Constants

#define ACTUATOR_SMOOTHING 0.1f

// Systems

void UPDATE_Input(void);
void UPDATE_Sync(void);
void UPDATE_Actuator(void);

// Input Changes

void CHANGE_Input(float Input);

// EVA FUNCTIONS

void EVA__Init(void);
void EVA__Shutdown(void);


// Helper function to reduce later boilercode in testing
void EVA__Simulate(void);

void CHECK_Faults(void);

#endif
