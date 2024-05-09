#include <thread>
#include <functional>
#include <iostream>
#include <chrono>
#include <atomic>

template <typename F , typename D = std::chrono::seconds>
struct TimerContext{
    using func_type = F;
    using duration_type = D;
    TimerContext(func_type in_func,duration_type in_interval):
        func{in_func}, interval{in_interval}, done{false}, is_stop{true}
    {}
    ~TimerContext(){
        std::cout << "quit\n";
    }
    void run(){
         while(!done){
            run_task();
         }
    }
    void start(){
        std::cout << "start\n";
        is_stop = false;
    }
    void stop(){
        std::cout << "stop\n";
        is_stop = true;
    }
    void im_done(){
       done = true;
    }
private:
    void run_task(){
       while(!is_stop && !done){
            func();
            std::this_thread::sleep_for(interval);
       }
    }
    func_type func;
    duration_type interval;
    std::atomic<bool> done; 
    std::atomic<bool> is_stop; //protect both done and is_stop because they're able to access by multiple threads
};

template <typename C = std::chrono::system_clock>
class CallBack{
public:
    using clock_type = C;
    int i = 1;
    std::chrono::time_point<clock_type> prev;
    void operator()(){
        auto now =  clock_type::now();
        std::cout << "run #" << i;
        if(i > 1){
            std::cout << " diff " <<  std::chrono::duration_cast<std::chrono::milliseconds>(now - prev).count() << "ms" ;
        }
        std::cout << '\n';
        prev = now;
        i++;
    }
};

#if 0
void print()
{
	static int idx = 0;

	std::cout <<"Index Count(" << idx++ <<")\n";
}
#else
void print(int& idx)
{
	std::cout <<"Index Count(" << idx++ <<")\n";
}
#endif
int main(){
    using namespace std::literals::chrono_literals;
    using cb_type = std::function<void()>;
    
    //TimerContext tc{CallBack{}, 0.5s};
    //TimerContext tc{CallBack{}, 0.01s};
#if 0
    TimerContext tc{print, 1s};
#else
    int index = 0;
    TimerContext tc{print, 1s};
#endif
    std::thread th{[&tc](){ 
        tc.run(); //block for make thread alive
    }};
    th.detach(); //let thread run independently

    tc.start();
    std::this_thread::sleep_for(2s); //do other thing

    tc.stop();
    std::this_thread::sleep_for(1s); //do other thing

    tc.start();
#if 1
    while(true)
    {
	    std::this_thread::sleep_for(100s); //do other thing
     }
#endif

    tc.im_done();

    std::cout << "done all thread\n";

    return 0;
}
