#include "Kernel/Kernel.h"

int main() {
    State_Reset();
    Timebase_Init();

    while (g_State.RUNNING) {
        EVA__Simulate();
        PLog_EVA_Actions();

        if (g_State.Tick >= 250)
            g_State.RUNNING = false;
    }

    return 0;
}
