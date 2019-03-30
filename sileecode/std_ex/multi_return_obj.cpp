#include <iostream>
#include <tuple>
#include <utility>

using namespace std;


template< typename outputs> struct calc_t;

template<> struct calc_t < string >
{
	string operator()(const char* text) const
	{
		return text;
	};

	string operator() (double dval)
	{
		to_string(dval);
	};

	string operator() (int ival)
	{
		to_string(ival);
	};


}


template< typename... Outputs, typename... Inputs >
inline tuple< Outputs... > TestVariadic( Inputs&&... inputs)
{
	return tuple< Output... > 
};


int main()
{

	return 0;
}

