#include <iostream>
#include <tuple>
#include <utility>

using namespace std;

template< typename Output > struct convert_t;
template<> struct convert_t< string >
{
	string operator()( const char *text ) const
	{
		return text;
	}

	string operator()( double val ) const
	{
		return to_string( val );
	}

	string operator()( int val ) const
	{
		return to_string( val );
	}
};

inline void next()
{}

template< typename First, typename... Others >
inline void next( First &&first, Others&&... others )
{
	//cout << first << "\n";
	next( forward< Others >( others )... );
}

template< typename... Outputs, typename... Inputs >
inline tuple< Outputs... > funkyfunc( Inputs&&... inputs )
{
	cout << "Inputs...\n";
	//show( forward< Inputs >( inputs )... );
	next( forward< Inputs >( inputs )... );

	return tuple< Outputs... >( convert_t< Outputs >()( inputs )... );
//	return tuple< Outputs... >( ( inputs )... );
}

int main()
{
	auto t = funkyfunc< string, double, int >( "text", 123.0, 32 );
	cout << "Outputs...\n";
	cout << get<0>( t ) << "\n";
	cout << get<1>( t ) << "\n";
	cout << get<2>( t ) << "\n";

	return 0;
}
