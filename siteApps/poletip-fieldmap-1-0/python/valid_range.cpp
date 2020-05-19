#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <string>

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

		//ifstream dataFile("./bptHWRHI");
		ifstream dataFile("./SCL3_HWR_BI_Map.txt");

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

				char str[128];
				strcpy (str, strToken.c_str());

				char *pch = 0;
				if(!(pch = strtok (str," \t,[]"))) continue;
				current = strtod(pch, NULL);

				while (pch != NULL)
				{
						if(!(pch = strtok (NULL," \t,[]"))) continue;
						gradient = strtod(pch, NULL);
						m.insert(std::pair<double,double>(gradient, current));
				};
		}

		while(true)
		{
			char c;
			cout << "Enter->" << endl;

			do{
				c=getchar();
				putchar(c);
			}while( c != '\n');

			double value = getRandom(0.1, 35.0);
			cout << "Random Number ==>> " << floor(1000.0 * value) / 1000.0 << endl;

			std::pair<double, double> range(floor(10.0 * value) / 10.0, floor(1000.0 * value) / 1000.0);

			auto iter = m.rbegin();
			if( value > iter->first) {
				value = iter->first;
			}

			//std::map<double, double>::iterator low = m.lower_bound(value);
			auto low = m.lower_bound(value);
			std::cout <<"Last Key: " << (double)iter->first << ", CloseKey: " << low->first << "(Field), Current: " << low->second << "(mA)" << std::endl;
		}

}
