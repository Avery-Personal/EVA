#ifndef TIMEBASE_H
#define TIMEBASE_H

#include <stdint.h>

void Timebase_Init(void);
void Timebase_Tick(void);
double Timebase_Get(void);
void Timebase_SleepUntilNextTick(void);

#define TICK_RATE_HZ 100
#define TICK_DURATION (1.0 / TICK_RATE_HZ)

#endif
