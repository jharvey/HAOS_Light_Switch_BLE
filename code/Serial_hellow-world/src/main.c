#include <zephyr/logging/log.h>

// Register the log module for this file
LOG_MODULE_REGISTER(my_app, LOG_LEVEL_INF);

int main(void) {
    // ... your existing LED setup code ...

    while (1) {
        LOG_INF("Hello World");
        // ... your existing LED blink code ...
        k_msleep(1000);
    }
}
