#include <iostream>
#include <algorithm>
#include <vector>


template <typename T>
class DStepInc
{
public:
	DStepInc(const T in_param, const T in_param2 ):mul{in_param},step{in_param2},mFlag{false},sum{0.0}
	{}
	//void operator() (const T& value) {
	void operator() (T& value) {
		value = value*mul+step;
		mFlag = true;
		sum += value;
	}

	operator bool(void) const {return mFlag;}
	operator T(void) const {return sum;}

	private:
	T mul;
	T step;
	bool mFlag;
	T sum;
};

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

	std::vector<double> dv(10,8);
	auto dprint = [](const double& cnt) {std::cout << cnt << ' ' ;};
	std::for_each(dv.begin(), dv.end(), dprint);
	newline();

	double result = std::for_each(dv.begin(), dv.end(), DStepInc<double>(0.9, 5.5));
	std::for_each(dv.begin(), dv.end(), dprint);
	newline();
	std::cout<< "Result: " << result << std::endl;

	std::for_each(dv.begin(), dv.end(), DStepInc<double>(1, 1));
	std::for_each(dv.begin(), dv.end(), dprint);
	newline();


	return 0;
}
