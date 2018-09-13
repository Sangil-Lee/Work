#include <boost/format.hpp> 
#include <iostream> 

int main() 
{ 
  std::cout << boost::format("%+d %d %d") % 99 % 100 % 99 << std::endl; 
} 