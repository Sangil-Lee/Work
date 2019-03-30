#include <boost/tuple/tuple.hpp> 
#include <boost/tuple/tuple_io.hpp> 
#include <iostream> 

int main() 
{ 
  std::cout << boost::make_tuple("Boris", "Schaeling", 43) << std::endl; 
} 