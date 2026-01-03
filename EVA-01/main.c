#include "Kernel/Kernel.h"

int main() {
    EVA__Init();
    Boot_General();

    while (g_State.SystemState == EVA_RUNNING) {
        EVA__Simulate();
        Log_FullState();

        if (g_State.Tick >= MAX_TICKS)
            g_State.SystemState = EVA_SHUTDOWN;
        
        Timebase_SleepUntilNextTick();
    }

    return 0;
}
