#ifndef STATE_H
#define STATE_H

#include <stdbool.h>

typedef struct {
    float PilotInput;

    float SyncRatio;
    float ActuatorOutput;

    bool FaultDetected;
    bool RUNNING;

    bool FORCE_STOP;

    int Tick;
} EvaState;

extern EvaState g_State;

void State_Reset(void);

#endif 
