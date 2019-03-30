// compile with g++-4.7 -std=c++11 -pthread threadtest.cpp -o threadtest

#include <iostream>
#include <thread>
#include <cstdlib>
#include <chrono>

static const int num_threads = 10;

int main() {

	srand((unsigned)time(nullptr));

	std::thread threads[num_threads];

	for (int i = 0; i < num_threads; ++i) 
	{
		threads[i] = std::thread([=]() 
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100));
			std::cout << "Launched from thread " << i << std::endl;
		});
	}

	std::cout << "Launched from main" << std::endl;

	for (auto &thread : threads) {
		thread.join();
	}

	return 0;
}
