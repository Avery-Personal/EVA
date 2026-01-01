#include "Kernel/Kernel.h"

int main() {
    State_Reset();
    Timebase_Init();

    while (g_State.SystemState == EVA_RUNNING) {
        EVA__Simulate();
        VLog_Auto();

        if (g_State.Tick >= 750)
            g_State.SystemState = EVA_SHUTDOWN;
        
        Timebase_SleepUntilNextTick();
    }

    return 0;
}
