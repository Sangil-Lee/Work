#include <chrono>
#include <future>
#include <iostream>
#include <thread>
 
int main()
{
/*
    const double infinity = std::numeric_limits<double>::infinity();
    std::chrono::duration<double> inf{ infinity };
    std::chrono::duration<double> one{ 1.0 };
    inf + one; // as if std::chrono::duration<double>{ infinity + 1.0 }
*/
    std::chrono::system_clock::time_point two_seconds_passed
        = std::chrono::system_clock::now() + std::chrono::seconds(2);
 
    // Make a future that takes 1 second to complete
    std::promise<int> p1;
    std::future<int> f_completes = p1.get_future();
    std::thread([](std::promise<int> p1)
                { 
                    std::this_thread::sleep_for(std::chrono::seconds(1)); 
                    p1.set_value_at_thread_exit(9); 
                }, 
                std::move(p1)
    ).detach();
 
    // Make a future that takes 5 seconds to complete
    std::promise<int> p2;
    std::future<int> f_times_out = p2.get_future();
    std::thread([](std::promise<int> p2)
                { 
                    std::this_thread::sleep_for(std::chrono::seconds(5)); 
                    p2.set_value_at_thread_exit(8); 
                }, 
                std::move(p2)
    ).detach();
 
    std::cout << "Waiting for 2 seconds..." << std::endl;
 
    if (std::future_status::ready == f_times_out.wait_until(two_seconds_passed))
        std::cout << "f_times_out: " << f_times_out.get() << "\n";
    else
        std::cout << "f_times_out did not complete!\n";

    if (std::future_status::ready == f_completes.wait_until(two_seconds_passed))
        std::cout << "f_completes: " << f_completes.get() << "\n";
    else
        std::cout << "f_completes did not complete!\n";
 
 
    std::cout << "Done!\n";
}
