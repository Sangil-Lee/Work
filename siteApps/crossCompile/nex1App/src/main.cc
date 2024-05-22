#include <thread>
#include <functional>
#include <iostream>
#include <chrono>
#include <atomic>
#include <hello.h>

/*
 * Simple Timer
 *
 */

template<typename F, typename D=std::chrono::seconds>
class SimpleTimer{
public:
		using func_type = F;
		using duration_type = D;

		SimpleTimer(func_type in_func, duration_type in_intervals):func{in_func},intervals{in_intervals}, done{false}, is_stop{false}
		{
				std::cout << "Generated SimpleTimer" << std::endl;
		};
		~SimpleTimer()
		{
				std::cout << "SimpleTimer Quit" << std::endl;
		};

		void run()
		{
				while(!done)
				{
						run_task();
				}
		}
		void start()
		{
				std::cout <<"SimpleTimer Start!!\n";
				is_stop = false;
		}
		void stop()
		{
				std::cout <<"SimpleTimer Stopp!!\n";
				is_stop = true;
		}

		void im_done() { 
				std::cout << "SimpleTime all done!!\n";
				done=true; 
		};

private:
	void run_task(){
		while(!is_stop && !done)
		{
			func();
			std::this_thread::sleep_for(intervals);
		};
	};

	func_type func;
	duration_type intervals;
	std::atomic<bool> done;
	std::atomic<bool> is_stop;
};

void print()
{
		static int cnt = 0;
		std::cout <<"Function Type Called("<<cnt++<<")" << std::endl;
}

///CallBack Class

template<typename CT=std::chrono::system_clock>
class CallBack{
public:
		using clock_type = CT;
		std::chrono::time_point<clock_type> prev;
		void operator()(){
				auto now = clock_type::now();
				std::cout << "run #" << minc << "==> ";
				if(minc>=1) {
						helloWorld();
						std::cout << ", diff: " << std::chrono::duration_cast<std::chrono::milliseconds>(now-prev).count() << "ms\n";
				}
				prev = now;
				minc++;
		};

		CallBack(int in_minc, std::string in_cbname):minc(in_minc),cbname(in_cbname)
		{
				std::cout << "Callback Name: " << in_cbname << std::endl;
		}
private:
		int minc;
		std::string cbname;
};

int main()
{
	using namespace std::literals::chrono_literals;

    //SimpleTimer timer{print, 1s};
	int start = 0;
	std::string cbname = "CallBack Instance";
    SimpleTimer timer{CallBack{start, cbname}, 1s};
	std::thread timerthread {
			[&timer]()
			{
				timer.run();
			}
	};
	timerthread.detach();

	timer.start();
	
	int count = 0;
	while(true)
	{
		std::this_thread::sleep_for(1s);
		if(count >= 30 ) 
		{
			timer.stop();
			break;
		}
		count++;
	};

	timer.im_done();

	std::cout <<"Quit Program!! \n";

	return 0;
}
