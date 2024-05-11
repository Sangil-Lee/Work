#include <iostream>
#include <algorithm>
#include <vector>


int main() 
{
	//Vector
	std::vector<int> myvector(8);
	std::fill (myvector.begin(), myvector.begin() + 4, 5);
	std::fill (myvector.begin()+3, myvector.end() - 2, 8);

	std::cout << "Filled: [";

	for(const auto& number: myvector)
	{
		std::cout << " " << number;
	};
	std::cout << " ]\n";


	return 0;
}
