#include <boost/function.hpp> 
#include <iostream> 
#include <cstdlib> 
#include <cstring> 

int main() 
{ 
  boost::function<int (const char*)> f = std::atoi; 
  std::cout << f("1609") << std::endl; 
  f = std::strlen; 
  std::cout << f("1609") << std::endl; 
} 