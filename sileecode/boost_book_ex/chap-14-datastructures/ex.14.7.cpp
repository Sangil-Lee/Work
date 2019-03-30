#include <boost/tuple/tuple.hpp> 
#include <boost/tuple/tuple_comparison.hpp> 
#include <string> 
#include <iostream> 

int main() 
{ 
  typedef boost::tuple<std::string, std::string, int> person; 
  person p1 = boost::make_tuple("Boris", "Schaeling", 43); 
  person p2 = boost::make_tuple("Boris", "Becker", 43); 
  std::cout << (p1 != p2) << std::endl; 
} 