#include <iostream>
#include <unistd.h>       // usleep
#include <vector>
#include <cstdlib>
#include <ctime>
#include "my_semaphore.h"  // POSIX 세마포어 대신 사용자 정의 세마포어 헤더 포함

MySemaphore printers(3);  // 프린터 3대로 초기화 (sem_init 대체)

void* print_job(void* arg) {
    int id = *(int*)arg;
    delete (int*)arg;

    // 프린터 대기 (sem_wait 대체)
    printers.wait();
    
    int used_time = rand() % 901 + 100;  
    usleep(used_time * 1000);            

    std::cout << "Thread " << id << " released a printer after "
              << used_time << " ms.\n";
    
    // 프린터 반환 (sem_post 대체)
    printers.post();

    return nullptr;
}

int main() {
    srand(time(NULL));
    // sem_init 제거 (생성자에서 초기화 완료)

    std::vector<pthread_t> threads;

    for (int i = 0; i < 50; ++i) {
        int* tid = new int(i);
        pthread_t t;
        pthread_create(&t, nullptr, print_job, tid);
        threads.push_back(t);
    }

    for (auto& t : threads)
        pthread_join(t, nullptr);

    // sem_destroy 제거 (소멸자에서 자동 처리)
    return 0;
}

