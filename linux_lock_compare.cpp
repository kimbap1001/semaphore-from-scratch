#include <iostream>
#include <pthread.h>
#include <vector>
#include <thread>
#include <chrono>

constexpr int ITER = 100000;
int counter = 0;

template <typename LockFn, typename UnlockFn>
void test_lock(const std::string& name,
               LockFn lock_fn,
               UnlockFn unlock_fn) {
    for (int n : {1, 2, 4, 8, 16}) {
        counter = 0;
        std::vector<std::thread> threads;

        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < n; ++i) {
            threads.emplace_back([&]() {
                for (int j = 0; j < ITER; ++j) {
                    lock_fn();
                    ++counter;
                    unlock_fn();
                }
            });
        }

        for (auto& t : threads) t.join();

        auto end = std::chrono::high_resolution_clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        std::cout << "[" << name << "] Threads: " << n
                  << ", Time: " << dur << " ms, Counter: " << counter << std::endl;
    }
}

int main() {
    // 1. Sleep mutex (pthread_mutex_t)
    pthread_mutex_t sleep_mutex = PTHREAD_MUTEX_INITIALIZER;
    auto sleep_lock = [&]() { pthread_mutex_lock(&sleep_mutex); };
    auto sleep_unlock = [&]() { pthread_mutex_unlock(&sleep_mutex); };

    std::cout << "==== Using pthread_mutex (sleep-and-wait) ====" << std::endl;
    test_lock("pthread_mutex", sleep_lock, sleep_unlock);

    // 2. Spinlock (pthread_spinlock_t)
    pthread_spinlock_t spinlock;
    pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE);
    auto spin_lock = [&]() { pthread_spin_lock(&spinlock); };
    auto spin_unlock = [&]() { pthread_spin_unlock(&spinlock); };

    std::cout << "\n==== Using pthread_spinlock (busy-wait) ====" << std::endl;
    test_lock("pthread_spinlock", spin_lock, spin_unlock);

    pthread_spin_destroy(&spinlock);
    return 0;
}

