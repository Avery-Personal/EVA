#ifndef STATE_H
#define STATE_H

#include <stdbool.h>

typedef enum {
    EVA_RUNNING = 0,

    EVA_EMERGENCY_STOP,
    EVA_FORCE_STOP,

    EVA_SHUTDOWN,
} EvaState_t;

typedef enum {
    EVA_BATTERY_PLUGGED,

    EVA_BATTERY_SLOW,
    EVA_BATTERY_NORMAL,
    EVA_BATTERY_RACING,
} EvaBatteryMode;

typedef struct {    
    // Pilot Data

    float PilotInput;

    float PilotFB;
    float PilotLR;
    float PilotArm;

    float SyncRatio;
    float RejectionCoefficient;

    // EVA x Pilot Output

    float ActuatorOutput;
    float ActuatorOutputLeft;
    float ActuatorOutputRight;

    // Actions

    bool ACTION_Attack;
    bool ACTION_Stop;

    // Settings

    double BatteryAmount;
    EvaBatteryMode BatteryMode;

    bool FaultDetected;
    int FaultCode;
    int FaultSeverity;

    EvaState_t SystemState;

    int Tick;
} EvaState;

extern EvaState g_State;

void State_Reset(void);

#endif 
