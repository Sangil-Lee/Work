#include <boost/tuple/tuple.hpp> 
#include <boost/tuple/tuple_io.hpp> 
#include <string> 
#include <iostream> 

int main() 
{ 
  typedef boost::tuple<std::string, std::string, int> person; 
  person p("Boris", "Schaeling", 43); 
  std::cout << p << std::endl; 
} 