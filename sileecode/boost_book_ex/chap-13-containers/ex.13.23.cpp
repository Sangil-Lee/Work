#include <boost/multi_array.hpp> 
#include <iostream> 

int main() 
{ 
  boost::multi_array<char, 1> a(boost::extents[6]); 

  a[0] = 'H'; 
  a[1] = 'e'; 
  a[2] = 'l'; 
  a[3] = 'l'; 
  a[4] = 'o'; 
  a[5] = '\0'; 

  std::cout << a.origin() << std::endl; 
} 