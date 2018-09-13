#include <boost/variant.hpp> 
#include <string> 
#include <iostream> 

int main() 
{ 
  boost::variant<double, char, std::string> v; 
  v = 3.14; 
  std::cout << v << std::endl; 
  v = 'A'; 
  std::cout << v << std::endl; 
  v = "Hello, world!"; 
  std::cout << v << std::endl; 
} 