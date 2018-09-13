#include <boost/variant.hpp> 
#include <string> 
#include <iostream> 

int main() 
{ 
  boost::variant<double, char, std::string> v; 
  v = 3.14; 
  std::cout << boost::get<double>(v) << std::endl; 
  v = 'A'; 
  std::cout << boost::get<char>(v) << std::endl; 
  v = "Hello, world!"; 
  std::cout << boost::get<std::string>(v) << std::endl; 
} 