#include <boost/shared_array.hpp> 
#include <iostream> 

int main() 
{ 
  boost::shared_array<int> i1(new int[2]); 
  boost::shared_array<int> i2(i1); 
  i1[0] = 1; 
  std::cout << i2[0] << std::endl; 
} 