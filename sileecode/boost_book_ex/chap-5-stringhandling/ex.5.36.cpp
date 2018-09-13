#include <boost/format.hpp> 
#include <iostream> 

int main() 
{ 
  std::cout << boost::format("%+s %s %s") % 99 % 100 % 99 << std::endl; 
} 