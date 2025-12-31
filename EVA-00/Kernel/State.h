#ifndef STATE_H
#define STATE_H

#include <stdbool.h>

typedef struct {
    float PilotInput;

    float PilotFB;
    float PilotLR;
    float PilotArm;

    float SyncRatio;
    float ActuatorOutput;

    // Actions

    bool ACTION_Attack;
    bool ACTION_Stop;

    // Settings

    bool FaultDetected;
    bool RUNNING;

    bool FORCE_STOP;

    int Tick;
} EvaState;

extern EvaState g_State;

void State_Reset(void);

#endif 
