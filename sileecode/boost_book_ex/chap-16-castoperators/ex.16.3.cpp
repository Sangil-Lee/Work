#include <boost/lexical_cast.hpp> 
#include <string> 
#include <iostream> 

int main() 
{ 
  std::string s = boost::lexical_cast<std::string>(169); 
  std::cout << s << std::endl; 
  double d = boost::lexical_cast<double>(s); 
  std::cout << d << std::endl; 
} 