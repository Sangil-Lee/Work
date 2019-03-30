#include <boost/any.hpp> 
#include <iostream> 

int main() 
{ 
  boost::any a = 1; 
  std::cout << boost::any_cast<int>(a) << std::endl; 
  a = 3.14; 
  std::cout << boost::any_cast<double>(a) << std::endl; 
  a = true; 
  std::cout << boost::any_cast<bool>(a) << std::endl; 
} 