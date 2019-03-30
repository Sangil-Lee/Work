#include <iostream>
#include <tuple>


using namespace std;


//compile:$>g++ -std=gnu++11 example.cpp

int main()
{
	//선언
	tuple <int, int, string> aTuple(10, 20, "hiru");

	auto bTuple = make_tuple(20,21,"hi",'z');

	int x = 0;

	std::get<x> (bTuple) = 100;

	cout << get<0>(bTuple) << endl;

	int aNumber; string aString; char aChar;

	aNumber = get<0>(aTuple);

	tie (ignore, ignore, aString, aChar) = bTuple;


	cout << aNumber << " " << aString << " " << aChar << endl;


	return 0;
}

