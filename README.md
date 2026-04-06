# semaphore-from-scratch

A systems programming project implementing a **custom semaphore** in C++ from the ground up using `std::mutex` and `std::condition_variable`, along with thread synchronization benchmarks comparing Spinlock, SleepMutex, and POSIX primitives.

---

## Overview

This project was developed as part of an Operating Systems course assignment. The core goal was to understand and implement low-level thread synchronization primitives without relying on POSIX `sem_t`.

**What I implemented:**
- `MySemaphore` class — a counting semaphore built with mutex + condition variable
- Replaced POSIX `sem_*` calls in a printer resource simulation with `MySemaphore`
- Extended `Makefile` to support multiple build targets

---

## Project Structure
```
├── my_semaphore.h       # Semaphore interface
├── my_semaphore.cpp     # Custom semaphore implementation (core)
├── printer_sim.cpp      # 50-thread printer simulation using MySemaphore
├── spinlock.h/cpp       # Atomic flag-based spinlock (provided)
├── sleep_mutex.h/cpp    # Condition variable-based mutex (provided)
├── linux_lock_compare.cpp  # POSIX pthread benchmark (provided)
└── Makefile
```
---

## My Implementation: MySemaphore

Implemented a POSIX-compatible counting semaphore using C++ standard library primitives:
```cpp
void MySemaphore::wait() {
    std::unique_lock<std::mutex> lock(mtx);
    while (count <= 0) {
        cv.wait(lock); // releases lock and suspends thread
    }
    --count;
}

void MySemaphore::post() {
    std::unique_lock<std::mutex> lock(mtx);
    ++count;
    cv.notify_one();
}
```

Key design decisions:
- `while (count <= 0)` loop guards against **spurious wakeups**
- RAII pattern via `std::unique_lock` ensures exception-safe lock management
- `cv.notify_one()` minimizes unnecessary thread wakeups

---

## Printer Resource Simulation
<img width="221" height="523" alt="image" src="https://github.com/user-attachments/assets/52d639dd-4789-43de-ac2d-42145a19fe31" />

Simulated 50 concurrent threads competing for 3 shared printer resources.

- Each thread calls `printers.wait()` to acquire a printer
- Holds the resource for a random duration (100–1000ms)
- Releases via `printers.post()`
Thread 2  released a printer after 246 ms.
Thread 0  released a printer after 311 ms.
Thread 3  released a printer after 140 ms.
...
Thread 45 released a printer after 994 ms.

---

## Benchmark Results

### Spinlock vs SleepMutex (custom implementations)
<img width="615" height="252" alt="image" src="https://github.com/user-attachments/assets/7417c38b-f201-4f52-b1af-92e2b4de656d" />

| Threads | Spinlock | SleepMutex |
|---------|----------|------------|
| 1       | 2 ms     | 5 ms       |
| 2       | 18 ms    | 24 ms      |
| 4       | 54 ms    | 207 ms     |
| 8       | 130 ms   | 1474 ms    |
| 16      | 503 ms   | 10648 ms   |

SleepMutex degrades significantly at higher thread counts due to **condition variable overhead** — each unlock/notify cycle carries more cost than busy-waiting in low-contention scenarios.

### POSIX pthread_mutex vs pthread_spinlock
<img width="742" height="305" alt="image" src="https://github.com/user-attachments/assets/c663b147-d35e-4014-b809-f477c63f7c70" />

| Threads | pthread_mutex | pthread_spinlock |
|---------|--------------|-----------------|
| 1       | 3 ms         | 0 ms            |
| 2       | 12 ms        | 12 ms           |
| 4       | 30 ms        | 28 ms           |
| 8       | 55 ms        | 65 ms           |
| 16      | 102 ms       | 167 ms          |

POSIX `pthread_mutex` outperforms `pthread_spinlock` at higher thread counts — busy-waiting threads consume CPU cycles even while waiting, increasing scheduler contention.

---

## Key Takeaways

- **Spinlock** is efficient for short critical sections with low contention but wastes CPU under high thread counts
- **Sleep-and-wait (mutex/semaphore)** scales better as threads are removed from the scheduler queue while waiting
- The custom `MySemaphore` correctly replicates POSIX semaphore behavior and passes the printer simulation

---

## Build & Run
```bash
# Build printer simulation with MySemaphore
make

# Run
./printer_sim

# Build and run POSIX benchmark
g++ -std=c++17 -O2 -pthread -o linux_lock_compare linux_lock_compare.cpp
./linux_lock_compare
```

---

## Environment

- Language: C++17
- OS: Linux (Ubuntu)
- Compiler: g++ with `-pthread`
