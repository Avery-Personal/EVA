#ifndef PILOT_H
#define PILOT_H

#include <stdbool.h>

#define PILOT_SMOOTHING 0.12f
#define GAIN_SMOOTH 0.08f

#define INPUT_DECAY 0.85f
#define INPUT_GAIN 0.05f
#define BASE_GAIN 1.0f

static float Smooth(float Previous, float Target);

void PILOT_Init(void);
void PILOT_Update(void);
void PILOT_Shutdown(void);

void PILOT_ApplyBias(void);
void PILOT_ApplyGain(void);

void PILOT_RawRead(void);
void PILOT_Filter(void);
void PILOT_UpdateCalibration(void);
void PILOT_ApplyCalibration(void);

#endif
