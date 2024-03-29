#ifndef EVERY_N_MICROS_H
#define EVERY_N_MICROS_H

#include <Arduino.h>

class EveryNMicros {
public:
    EveryNMicros(unsigned long interval) : intervalMicros(interval), previousMicros(0) {}

    bool shouldRun(unsigned long currentMicros) {
        if (currentMicros - previousMicros >= intervalMicros) {
            previousMicros = currentMicros;
            return true;
        }
        return false;
    }
    
private:
    unsigned long previousMicros;
    unsigned long intervalMicros;
};

#define CONCAT_INTERNAL(x, y) x##y
#define CONCAT(x, y) CONCAT_INTERNAL(x, y)
#define EVERY_N_MICROS(N) \
    static EveryNMicros CONCAT(everyNMicros_, __LINE__)(N); \
    if (CONCAT(everyNMicros_, __LINE__).shouldRun(micros()))

#endif