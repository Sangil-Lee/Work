#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>
#include <functional>

class ThreadManager {
private:
    std::vector<std::thread> threads;
    std::mutex mtx;

public:
    void addThread(std::function<void()> task) {
        std::lock_guard<std::mutex> lock(mtx);
        threads.emplace_back(task);
    }

    void joinAll() {
        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        threads.clear();
    }

    ~ThreadManager() {
        joinAll();
    }
};

class PeriodicTimer {
private:
    std::chrono::milliseconds interval;
    std::function<void()> callback;
    bool running;
    std::thread timerThread;

public:
    PeriodicTimer(std::chrono::milliseconds interval, std::function<void()> callback)
        : interval(interval), callback(callback), running(false) {}

    ~PeriodicTimer() {
        stop();
    }

    void start() {
        if (!running) {
            running = true;
            timerThread = std::thread([this]() {
                while (running) {
                    std::this_thread::sleep_for(interval);
                    if (running) {
                        callback();
                    }
                }
            });
        }
    }

    void stop() {
        running = false;
        if (timerThread.joinable()) {
            timerThread.join();
        }
    }
};

#if 0
// 사용 예제
int main() {
    int counter = 0;
    PeriodicTimer timer(std::chrono::seconds(1), [&counter]() {
        std::cout << "타이머 실행 횟수: " << ++counter << std::endl;
    });

    std::cout << "주기적 타이머 시작" << std::endl;
    timer.start();

    // 10초 동안 실행
	while(true)
		std::this_thread::sleep_for(std::chrono::seconds(1));

    timer.stop();
    std::cout << "주기적 타이머 종료" << std::endl;

    return 0;
}
#else
// 사용 예제
int main() {
    ThreadManager threadManager;

    // 여러 개의 타이머 생성 및 실행
    for (int i = 0; i < 3; ++i) {
        threadManager.addThread([i]() {
            PeriodicTimer timer(std::chrono::seconds(1), [i]() {
                std::cout << "타이머 " << i << " 실행" << std::endl;
            });
            timer.start();
            std::this_thread::sleep_for(std::chrono::seconds(5));
            timer.stop();
        });
    }
}
#endif
