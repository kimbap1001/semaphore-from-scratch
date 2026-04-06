#include "spinlock.h"

void Spinlock::lock() {
    while (flag.test_and_set(std::memory_order_acquire)) {
        // busy wait
    }
}

void Spinlock::unlock() {
    flag.clear(std::memory_order_release);
}

