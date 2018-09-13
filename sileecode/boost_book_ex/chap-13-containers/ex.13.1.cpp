#include <boost/array.hpp> 
#include <iostream> 
#include <string> 
#include <algorithm> 

int main() 
{ 
  typedef boost::array<std::string, 3> array; 
  array a; 

  a[0] = "Boris"; 
  a.at(1) = "Anton"; 
  *a.rbegin() = "Caesar"; 

  std::sort(a.begin(), a.end()); 

  for (array::const_iterator it = a.begin(); it != a.end(); ++it) 
    std::cout << *it << std::endl; 

  std::cout << a.size() << std::endl; 
  std::cout << a.max_size() << std::endl; 
} 