#include <boost/tuple/tuple.hpp> 
#include <string> 
#include <iostream> 

int main() 
{ 
  typedef boost::tuple<std::string, std::string, int> person; 
  person p = boost::make_tuple("Boris", "Schaeling", 43); 
  std::cout << p.get<0>() << std::endl; 
  std::cout << boost::get<0>(p) << std::endl; 
} 