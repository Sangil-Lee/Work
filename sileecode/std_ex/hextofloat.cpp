
#include <cstdio>
#include <iostream>
#include <utility>

using namespace std;


double hexstr2double(const std::string& hexstr)
{
	union
	{
		long long i;
		double    d;
	} value;

	value.i = std::stoll(hexstr, nullptr, 16);

	return value.d;
}

float hexstr2float(const std::string& hexstr)
{
	union
	{
		long	 i;
		float    d;
	} value;

	value.i = std::stol(hexstr, nullptr, 16);
	return value.d;
}

int main(int argc, char* argv[])
{

	if(argc != 2) return 0;


	//std::cout << "413DF3EBA463B0 = " << hexstr2double("413DF3EBA463B0") << '\n';
	std::cout << string(argv[1]) << " = "<< hexstr2float(string(argv[1])) << '\n';
	std::cout << string(argv[1]) << " = "<< hexstr2double(string(argv[1])) << '\n';
	return 0;

}


