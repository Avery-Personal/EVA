#ifndef STATE_H
#define STATE_H

#include <stdbool.h>

typedef enum {
    EVA_RUNNING = 0,

    EVA_EMERGENCY_STOP,
    EVA_FORCE_STOP,

    EVA_SHUTDOWN,
} EvaState_t;

typedef struct {
    float PilotInput;

    float PilotFB;
    float PilotLR;
    float PilotArm;

    float SyncRatio;
    float RejectionCoefficient;

    float ActuatorOutput;
    float ActuatorOutputLeft;
    float ActuatorOutputRight;
    
    // Actions

    bool ACTION_Attack;
    bool ACTION_Stop;

    // Settings

    bool FaultDetected;
    EvaState_t SystemState;

    int Tick;
} EvaState;

extern EvaState g_State;

void State_Reset(void);

#endif 
