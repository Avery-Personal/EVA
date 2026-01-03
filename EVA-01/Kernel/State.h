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
    float Position;

    float AllocatedPower; // Amount of energy/signal applied
    float ActualPower; // Literal power extraction (25% of 100.0f -> 0.25f (Varies on battery mode; Normal mode is visualized in said example))

    bool Active;
} HardwareActuator;

typedef struct {
    float VelocityFB;
    float VelocityLR;

    float TorsoRotation; // Angular Velocity
} LocomotionState;

typedef struct {
    HardwareActuator LeftLeg;
    HardwareActuator RightLeg;
    
    HardwareActuator LeftArm;
    HardwareActuator RightArm;

    HardwareActuator Torso;
    HardwareActuator Head;

    LocomotionState Locomotion;
    float TotalActuatorOutput; // Total actuator power
} ActuatorState;

typedef struct {
    ActuatorState LimbActuators;
    float ActuatorOutput;

    float ActuatorOutputLeft;
    float ActuatorOutputRight;
} EvaActuators;

typedef struct {
    float Move_X;
    float Move_Y;
    
    float ArmL;
    float ArmR;
} PilotRawInput;

typedef struct {
    float Move_X;
    float Move_Y;
    
    float ArmL;
    float ArmR;
} PilotFilteredInput;

typedef struct {
    float Move_X;
    float Move_Y;
    
    float ArmL;
    float ArmR;
} PilotOutput;

typedef struct {
    PilotRawInput RawInput;
    PilotFilteredInput FilteredInput;
    PilotOutput Output;

    // Pilot overall effectiveness | Input = BaseSkill [.85f] * SyncRatio * (1 - Fatigue) * (1 - Trauma) * InputFactor
    // InputFactor = 1.0f - fabsf(Pilot -> Move_X - Pilot -> Move_X_Previous) * 0.1f - fabsf(Pilot -> Move_Y - pilot -> Move_Y_Previous) * 0.1f;
    // BaseSkill = .85f
    int Input;

    // Physical

    float Effectiveness;
    float Fatigue;
    float Conscious;

    // Mental

    float Trauma;
    float Stress;

    float InputGain;
    float InputBias;
} EvaPilot;

typedef struct {    
    // Pilot Data

    EvaPilot Pilot;
    EvaActuators Actuators;

    float SyncRatio;
    float RejectionCoefficient;

    // Actions

    bool ACTION_Attack;
    bool ACTION_Stop;

    // Health

    float DamageFactor;
    float Responsiveness;

    // Settings

    double BatteryLevel;
    EvaBatteryMode BatteryMode;

    bool Berserk;
    bool DummyPlug;

    bool FaultDetected;
    int FaultCode;
    int FaultSeverity;

    EvaState_t SystemState;

    int Tick;
} EvaState;

extern EvaState g_State;

void State_Reset(void);

#endif 
