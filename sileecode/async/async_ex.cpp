#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <future>
#include <string>
#include <mutex>

std::mutex m;

struct X
{
	void foo(int i, const std::string& str)
	{
		std::lock_guard<std::mutex> lk(m);
		std::cout << str << ' ' << i << '\n';
	}
	void bar(const std::string& str)
	{
		std::lock_guard<std::mutex> lk(m);
		std::cout << str << '\n';
	}
	int operator()(int i)
	{
		std::lock_guard<std::mutex> lk(m);
		std::cout << i << '\n';
		return i + 10;
	}
};

template<typename RandomIt>
long long int parallel_sum(RandomIt beg, RandomIt end)
{
	auto len = end - beg;
	if (len < 1000)
	{
		return std::accumulate(beg, end, 0);
	}

	// 비동기 쓰레드를 생성하여 또 다른 부분 합을 구한다.
	RandomIt mid = beg + len / 2;
	auto handle = std::async(std::launch::async,
		parallel_sum<RandomIt>, mid, end);

	long long int sum = parallel_sum(beg, mid);
	// 여기서 결과가 계산될 때까지 기다리게 된다.
	return sum + handle.get();
}

int main()
{
	std::vector<int> v(10000, 1);
	std::cout << "The sum is " << parallel_sum(v.begin(), v.end()) << '\n';

	X x;

	auto a1 = std::async(&X::foo, &x, 42, "Hello");

	auto a2 = std::async(std::launch::deferred, &X::bar, x, "world!");

	auto a3 = std::async(std::launch::async, X(), 43);

	// 지연 플래그로 설정된 쓰레드는 여기서 실행을 시작한다.
	a2.wait();
	
	// 비동기 플래그로 비동기 실행된 쓰레드의 실행 결과를 기다린다.
	std::cout << a3.get() << '\n';

	return 0;
}
