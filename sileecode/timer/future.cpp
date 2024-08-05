#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
#include <mutex>
#include <future>


class FutureTimerManager {
private:
    std::vector<std::future<void>> futures;

public:
    void addTimer(std::function<void()> task, std::chrono::milliseconds duration) {
        futures.push_back(std::async(std::launch::async, [task, duration]() {
            std::this_thread::sleep_for(duration);
            task();
        }));
    }

    void waitAll() {
        for (auto& future : futures) {
            future.wait();
        }
        futures.clear();
    }
};

// 사용 예제
int main() {
    FutureTimerManager timerManager;

    // 여러 개의 타이머 추가
    timerManager.addTimer([]() {
        std::cout << "타이머 1 실행 (2초 후)" << std::endl;
    }, std::chrono::seconds(2));

    timerManager.addTimer([]() {
        std::cout << "타이머 2 실행 (3초 후)" << std::endl;
    }, std::chrono::seconds(3));

    timerManager.addTimer([]() {
        std::cout << "타이머 3 실행 (1초 후)" << std::endl;
    }, std::chrono::seconds(1));

    // 모든 타이머가 완료될 때까지 대기
    timerManager.waitAll();

    std::cout << "모든 타이머 완료" << std::endl;

    return 0;
}
