#include <boost/any.hpp> 
#include <string> 

int main() 
{ 
  boost::any a = 1; 
  a = 3.14; 
  a = true; 
  a = std::string("Hello, world!"); 
} 