#include <boost/array.hpp> 
#include <string> 

int main() 
{ 
  typedef boost::array<std::string, 3> array; 
  array a = { "Boris", "Anton", "Caesar" }; 
} 