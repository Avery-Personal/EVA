#include "Kernel/Kernel.h"

int main() {
    EVA__Init();
    Boot_General();

    while (g_State.SystemState == EVA_RUNNING) {
        EVA__Simulate();
        ELog_HardwareStatistics();

        if (g_State.Tick >= 1750)
            g_State.SystemState = EVA_SHUTDOWN;
        
        Timebase_SleepUntilNextTick();
    }

    return 0;
}
