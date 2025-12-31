#include "Kernel/Kernel.h"

int main() {
    State_Reset();
    Timebase_Init();

    while (g_State.RUNNING) {
        EVA__Simulate();
        Log_State();

        if (g_State.Tick >= 100)
            g_State.RUNNING = false;
    }

    return 0;
}
