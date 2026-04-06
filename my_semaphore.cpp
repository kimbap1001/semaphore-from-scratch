#include "my_semaphore.h"

MySemaphore::MySemaphore(int initial_count) 
    : count(initial_count) {}  // 초기 카운트 값 설정

MySemaphore::~MySemaphore() {
    // 자원 정리 필요 없음 (RAII 패턴으로 자동 관리)
}

void MySemaphore::wait() {
    std::unique_lock<std::mutex> lock(mtx);
    // 카운트가 0 이하일 때까지 대기 (spurious wakeup 방지)
    while (count <= 0) {
        cv.wait(lock);  // 락 해제 & 조건 충족 시까지 대기
    }
    --count;  // 카운트 감소 (임계 영역 진입)
}

void MySemaphore::post() {
    std::unique_lock<std::mutex> lock(mtx);
    ++count;    // 카운트 증가
    cv.notify_one();  // 대기 중인 스레드 1개 깨우기
}

