#include <boost/tuple/tuple.hpp> 
#include <boost/tuple/tuple_io.hpp> 
#include <string> 
#include <iostream> 

int main() 
{ 
  typedef boost::tuple<std::string, std::string, int> person; 
  person p = boost::make_tuple("Boris", "Schaeling", 43); 
  p.get<1>() = "Becker"; 
  std::cout << p << std::endl; 
} 