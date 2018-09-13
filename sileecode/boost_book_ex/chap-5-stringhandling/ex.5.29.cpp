#include <boost/format.hpp> 
#include <iostream> 

int main() 
{ 
  std::cout << boost::format("%1%.%2%.%3%") % 16 % 9 % 2011 << std::endl; 
} 