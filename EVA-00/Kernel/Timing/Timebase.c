#include <time.h>

#include "Timebase.h"

static double SystemTime = 0.0;
static const double DeltaTime = 0.01;

void Timebase_Init(void) {
    SystemTime = 0.0;
}

void Timebase_Tick(void) {
    SystemTime += DeltaTime;
}

double Timebase_Get(void) {
    return SystemTime;
}
