#include "../../Kernel/Kernel.h"
#include "../Diagnostics.h"

int Boot_General() {
    int Count = 0;
    int Passed = 0;
    int Failed = 0;

    Count++;
    if (g_State.BatteryLevel <= 0.0f) {
        printf("Power below minimum operating level\n");

        Failed++;
    }

    Count++;
    if (g_State.SyncRatio > 1.0f) {
        printf("Sync exceeds tolerance limit\n");

        Failed++;
    }

    Count++;
    if (g_State.Pilot.Conscious <= 0.05f) {
        printf("Pilot unconscious\n");

        Failed++;
    }

    Count++;
    if (g_State.Pilot.Trauma > MAX_TRAUMA) {
        printf("Pilot trauma critical\n");

        Failed++;
    }

    Passed = Count - Failed;

    printf("TESTS RAN: %i | TESTS PASSED: %i | TESTS FAILED: %i\n", Count, Passed, Failed);

    return Passed;
}
