#include "sleep_mutex.h"

void SleepMutex::lock() {
    std::unique_lock<std::mutex> lock(mtx);
    while (is_locked)
        cv.wait(lock);
    is_locked = true;
}

void SleepMutex::unlock() {
    std::unique_lock<std::mutex> lock(mtx);
    is_locked = false;
    cv.notify_one();
}

