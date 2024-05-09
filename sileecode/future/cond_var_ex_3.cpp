#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

bool is_ready_1(false);
bool is_ready_2(false);
std::mutex m;
std::condition_variable cv;

void test1()
{
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::unique_lock<std::mutex> lk(m);
    is_ready_1 = true;
    cv.notify_one();
}
void test2()
{
    std::this_thread::sleep_for(std::chrono::seconds(6));
    std::unique_lock<std::mutex> lk(m);
    is_ready_2 = true;
    cv.notify_one();
}

int main()
{
    std::thread t1(test1);
    std::thread t2(test2);

    std::unique_lock<std::mutex> lk(m);
    while (!is_ready_1)
    {
        cv.wait(lk);
        if (is_ready_1)
            std::cout << "Spurious wake-1 up!\n";
    }

    while (!is_ready_2)
    {
         cv.wait(lk);
         if (is_ready_2)
         std::cout << "Spurious wake-2 up!\n";
    }
    t1.join(); 
    t2.join();
//    system("pause");
}
