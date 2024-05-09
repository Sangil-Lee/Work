#include <chrono>
#include <future>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

int main()
{
	std::future<int> future = std::async(std::launch::async, []()
	{
		//std::this_thread::sleep_for(10000ms);
		std::this_thread::sleep_for(1s);
		return 9;
	});


	std::cout << "Waiting..." << std::endl;

	std::future_status status;

	do
	{
		switch(status = future.wait_for(1s); status)
		{
			case std::future_status::deferred:
				std::cout<< "Deferred \n";
				break;
			case std::future_status::timeout:
				std::cout<< "Timeout.. \n";
				break;
			case std::future_status::ready:
				std::cout<< "Ready.. \n";
				break;
		}
	}while(status != std::future_status::ready);

	std::cout << "Result: " << future.get() << std::endl;

}
