#include <boost/foreach.hpp> 
#include <boost/array.hpp> 
#include <iostream> 

int main() 
{ 
  boost::array<int, 4> a = { { 0, 1, 2, 3 } }; 

  BOOST_FOREACH(int &i, a) 
    i *= i; 

  BOOST_REVERSE_FOREACH(int i, a) 
  { 
    std::cout << i << std::endl; 
  } 
} 