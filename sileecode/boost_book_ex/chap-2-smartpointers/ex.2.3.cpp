#include <boost/scoped_array.hpp> 

int main() 
{ 
  boost::scoped_array<int> i(new int[2]); 
  *i.get() = 1; 
  i[1] = 2; 
  i.reset(new int[3]); 
} 