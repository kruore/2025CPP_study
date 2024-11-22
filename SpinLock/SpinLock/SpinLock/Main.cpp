#include <iostream>
#include <thread>
#include <atomic>

class SpinLock {
public:
    SpinLock() : _locked(false) {} // 명시적 초기화

    void lock() {
        bool expected = false;
        while (!_locked.compare_exchange_strong(expected, true)) {
            expected = false; // 비교 실패 시 다시 초기화
        }
    }

    void unlock() {
        _locked.store(false);
    }

private:
    std::atomic<bool> _locked;
};

// Custom lock guard for SpinLock
class SpinLockGuard {
public:
    explicit SpinLockGuard(SpinLock& lock) : _lock(lock) {
        _lock.lock(); // 잠금
    }
    ~SpinLockGuard() {
        _lock.unlock(); // 해제
    }

    SpinLockGuard(const SpinLockGuard&) = delete; // 복사 금지
    SpinLockGuard& operator=(const SpinLockGuard&) = delete; // 복사 대입 금지

private:
    SpinLock& _lock;
};

volatile int sum = 0;
SpinLock spinLock;

void Add() {
    for (int i = 0; i < 10000; i++) {
        SpinLockGuard guard(spinLock); // Custom guard 사용
        sum++;
    }
}

void Sub() {
    for (int i = 0; i < 10000; i++) {
        SpinLockGuard guard(spinLock); // Custom guard 사용
        sum--;
    }
}

int main() {
    std::thread t1(Add);
    std::thread t2(Sub);

    t1.join();
    t2.join();

    std::cout << "Final sum: " << sum << std::endl; // 출력 결과: 항상 0
    return 0;
}
