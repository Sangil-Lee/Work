#include <tuple>
#include <iostream>
#include <cstdio>


//Compile g++ -std=c++11 source.cpp
using namespace std;


tuple < int, int, int, int> cal(int n1, int n2)
{
	return make_tuple(n1/n2, n1%n2, n1+n2,n1*n2);
}

int main()
{
	int re1, re2, re3, re4, a, b;

	cin >> a >> b;

	tie(re1, re2, re3, re4) = cal (a, b);

	auto result = cal (a, b);

	cout << re1 << "," << re2 << "," << re3 << "," << re4 << endl;
	printf("re1:%d, re2:%d, re3:%d, re4:%d\n", get<0>(result),	get<1>(result),	get<2>(result),	get<3>(result));	

	string str = to_string(12.345);

	return 0;

}
