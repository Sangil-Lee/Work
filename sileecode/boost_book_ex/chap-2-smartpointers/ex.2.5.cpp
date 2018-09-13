#include <boost/shared_ptr.hpp> 

int main() 
{ 
  boost::shared_ptr<int> i1(new int(1)); 
  boost::shared_ptr<int> i2(i1); 
  i1.reset(new int(2)); 
} 