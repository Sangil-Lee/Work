#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>


int main()
{
	std::random_device rd{};
	std::mt19937 gen{rd()};

	std::normal_distribution<> d{5,2};

	std::map<int, int> hist {};

	for(int n = 0; n < 10000; ++n)
	{
		//std::cout << d(gen) << ' ';
		++hist[std::round(d(gen))];
	}

	for(auto p: hist)
	{
//std::cout << std::setw(2) << p.first << ' ' << std::string(p.second/200, '*') << std::endl;
		std::cout << p.second << ' ';
		std::cout << std::setw(2) << p.first << ' ' << std::string(p.second/200, '*') << std::endl;
	}
	
	return 0;
}
