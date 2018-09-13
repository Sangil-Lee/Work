#include <boost/tuple/tuple.hpp> 
#include <boost/tuple/tuple_io.hpp> 
#include <string> 
#include <iostream> 

int main() 
{ 
  std::string s = "Boris"; 
  std::cout << boost::make_tuple(boost::ref(s), "Schaeling", 43) 
    << std::endl; 
} 