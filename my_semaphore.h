#ifndef MY_SEMAPHORE_H
#define MY_SEMAPHORE_H

#include <mutex>
#include <condition_variable>

class MySemaphore {
private:
    // TODO: 세마포어 구현에 필요한 멤버 변수들을 선언하세요.
    // 예: 카운트 변수, 뮤텍스, 조건 변수 등
    int count;
    std::mutex mtx;
    std::condition_variable cv;

public:
    // 생성자: 세마포어의 초기 카운트 값을 설정합니다.
    MySemaphore(int initial_count);

    // 소멸자: 필요한 경우 리소스를 정리합니다.
    ~MySemaphore();

    // wait 연산 (P 또는 down): 카운트를 감소시키고, 0 이하이면 대기합니다.
    void wait();

    // post 연산 (V 또는 up): 카운트를 증가시키고, 대기 중인 스레드가 있다면 깨웁니다.
    void post();
};

#endif // MY_SEMAPHORE_H
