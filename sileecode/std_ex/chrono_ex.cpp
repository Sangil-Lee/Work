// system_clock example
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>

long fibonacci(int n)
{
	if (n < 3) return 1;
	return fibonacci(n-1) + fibonacci(n-2);
}


int main ()
{
	using std::chrono::system_clock;

	std::chrono::duration<int,std::ratio<60*60*24> > two_day (2);

	system_clock::time_point today = system_clock::now();
	system_clock::time_point nexttomorrow = today + two_day;

	std::time_t tt;

	tt = system_clock::to_time_t ( today );
	std::cout << "today is: " << ctime(&tt);

	tt = system_clock::to_time_t ( nexttomorrow );
	std::cout << "the next tomorrow will be: " << ctime(&tt);

	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	std::cout << "f(42) = " << fibonacci(42) << '\n';
	end = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = end-start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);

	std::cout << "finished computation at " << std::ctime(&end_time)
		<< "elapsed time: " << elapsed_seconds.count() << "s\n";


	using namespace std::chrono;

	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	std::cout << "printing out 1000 stars...\n";
	for (int i=0; i<1000; ++i) std::cout << "*";
	std::cout << std::endl;

	high_resolution_clock::time_point t2 = high_resolution_clock::now();

	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

	std::cout << "It took me " << time_span.count() << " seconds.";
	std::cout << std::endl;
	return 0;
}

