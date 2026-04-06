#pragma once
#include <mutex>
#include <condition_variable>

class SleepMutex {
    std::mutex mtx;
    std::condition_variable cv;
    bool is_locked = false;
public:
    void lock();
    void unlock();
};

