#include "../Kernel/Kernel.h"

int main() {
    EVA__Init();
    CHANGE_Input(.25f);

    while (g_State.SystemState == EVA_RUNNING) {
        EVA__Simulate();
        Log_State();

        if (g_State.Tick >= 1500)
            EVA__Shutdown();

        Timebase_SleepUntilNextTick();
    }

    return 0;
}