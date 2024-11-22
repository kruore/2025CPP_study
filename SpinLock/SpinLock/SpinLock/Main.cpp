#include <iostream>
#include <thread>
#include <atomic>

class SpinLock {
public:
    SpinLock() : _locked(false) {} // ����� �ʱ�ȭ

    void lock() {
        bool expected = false;
        while (!_locked.compare_exchange_strong(expected, true)) {
            expected = false; // �� ���� �� �ٽ� �ʱ�ȭ
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
        _lock.lock(); // ���
    }
    ~SpinLockGuard() {
        _lock.unlock(); // ����
    }

    SpinLockGuard(const SpinLockGuard&) = delete; // ���� ����
    SpinLockGuard& operator=(const SpinLockGuard&) = delete; // ���� ���� ����

private:
    SpinLock& _lock;
};

volatile int sum = 0;
SpinLock spinLock;

void Add() {
    for (int i = 0; i < 10000; i++) {
        SpinLockGuard guard(spinLock); // Custom guard ���
        sum++;
    }
}

void Sub() {
    for (int i = 0; i < 10000; i++) {
        SpinLockGuard guard(spinLock); // Custom guard ���
        sum--;
    }
}

int main() {
    std::thread t1(Add);
    std::thread t2(Sub);

    t1.join();
    t2.join();

    std::cout << "Final sum: " << sum << std::endl; // ��� ���: �׻� 0
    return 0;
}
