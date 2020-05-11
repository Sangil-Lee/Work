#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <cmath>

//g++ -o closet_key valid_range.cpp 

using namespace std;

double rounding( double x, int digit )
{
		return ( floor( (x) * pow( float(10), digit ) + 0.5f ) / pow( float(10), digit ) );
}

double getRandom(double min, double max)
{
		double before = rand() % (int)max + (int)min;
		double after = (double)rand() / RAND_MAX;
		double result = before + after;
		if (result < min || result > max) {
				result = getRandom(min, max);
		}
		return result;
}

int main()
{
		const size_t N = 10000;
		std::map<double, double> m;

		ifstream dataFile("./bptHWRHI");

		if(dataFile.fail())
		{
				cout <<"File Not Found" << endl;
				exit(-1);
		};

		srand (time(NULL));

		string strToken;
		double gradient = 0.0;
		double current = 0.0;

		while(!dataFile.eof())
		{
				getline(dataFile, strToken);
				if(strToken[0] == '#' || strToken.empty() == true) continue;

				char str[strToken.size()];
				strcpy (str, strToken.c_str());

				char *pch = 0;
				if(!(pch = strtok (str," \t,"))) continue;
				//printf("Field Gradient: %s\n", pch);
				gradient = strtod(pch, NULL);

				while (pch != NULL)
				{
						if(!(pch = strtok (NULL," \t,"))) continue;
						//printf("Current: %s\n", pch);
						current = strtod(pch, NULL);
						m.insert(std::pair<double,double>(gradient, current));
				};
		}

		//for ( size_t i = 0; i < N; i++ ) 
				//m.insert(std::pair<double,double>(getRandom(0.0, 35.0), getRandom(0.0, 225)));
		//double value = 2.3467;

		double value = getRandom(1.0, 35.0);
		//cout << rounding(value,3) << ' ' << rounding(value, 2) << endl;
		//cout << floor(10.0*value)/10.0 << ' ' << floor(1000.0*value)/1000.0 << endl;
		cout << "Random Number ==>> " << floor(1000.0*value)/1000.0 << endl;

		//std::pair<double, double> range( 1.110, 1.120 );
		//std::pair<double, double> range( rounding(value, 3), rounding(value,2));
		std::pair<double, double> range( floor(10.0 * value)/10.0, floor(1000.0*value)/1000.0);

#if 0
		auto low = m.lower_bound( range.first );
		auto upper = m.upper_bound( range.second );
#else
		//std::map<double, double>::iterator low = m.lower_bound( range.first );
		//std::map<double, double>::iterator  upper = m.upper_bound( range.second );
		std::map<double, double>::iterator low = m.lower_bound(value);
		//std::map<double, double>::iterator  upper = m.upper_bound(value);
#endif
		std::cout << low->first << ' ' << low->second << std::endl;

#if 0
		if ( low != upper )
		{
				for ( std::map<double, double>::iterator it = low; it != upper; ++it )
				{
						std::cout << it->first << ' ' << it->second << std::endl;
				}
		}
#endif
}
