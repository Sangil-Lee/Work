#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>


int main ()
{
	std::stringstream hexastring;

	short data = 100;
	hexastring << std::setfill('0') << std::setw(sizeof(int)) << std::hex << data;
	std::cout << "Data: " << hexastring.str() << std::endl;
	return 0;
	
}
