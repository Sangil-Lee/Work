#include <iostream>
#include <algorithm>
#include <vector>

int main()
{

	//std::vector<int> v{8, 10};
	std::vector<int> v(10,8);

	auto print = [](const int& cnt) {std::cout << cnt << ' ' ;};
	auto newline = [](){std::cout<<std::endl;};

	int step = 2;
	auto stepinc = [](int& value) { value += 2; };

	std::for_each(v.begin(), v.end(), print);
	newline();

	std::for_each(v.begin(), v.end(), stepinc);
	std::for_each(v.begin(), v.end(), print);
	newline();
#if 1
	struct StepInc
	{
		void operator()(int& n) const { n+=step;}
		StepInc(int in_param, int in_param2):step{in_param},step2{in_param2} 
		{ 
			std::cout <<"Param list: " << step << ", " << step2 <<std::endl;
		}
		private:
		int step;
		int step2;
		
	};
#endif
	//std::for_each(v.begin(), v.end(), StepInc(4));
	std::for_each(v.begin(), v.end(), StepInc(4, 10));
	std::for_each(v.begin(), v.end(), print);
	newline();


	return 0;
}
