#include <windows.h>

#include "Timebase.h"

static double SystemTime = 0.0;

static LARGE_INTEGER LastTick;
static LARGE_INTEGER Frequency;

void Timebase_Init(void) {
    SystemTime = 0.0;
    
    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&LastTick);
}

void Timebase_Tick(void) {
    SystemTime += TICK_DURATION;

    QueryPerformanceCounter(&LastTick);
}

double Timebase_Get(void) {
    return SystemTime;
}

void Timebase_SleepUntilNextTick(void) {
    LARGE_INTEGER NOW;

    QueryPerformanceCounter(&NOW);

    double Elapsed = (double)(NOW.QuadPart - LastTick.QuadPart) / (double) Frequency.QuadPart;
    double SleepTime = TICK_DURATION - Elapsed;

    if (SleepTime > 0.0) {
        DWORD ms = (DWORD)(SleepTime * 1000.0);
        if(ms > 0) Sleep(ms);

        QueryPerformanceCounter(&NOW);

        while ((double)(NOW.QuadPart - LastTick.QuadPart) / (double) Frequency.QuadPart < TICK_DURATION) {
            QueryPerformanceCounter(&NOW);
        }
    }
}   
