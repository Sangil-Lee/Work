#include <boost/shared_ptr.hpp> 
#include <boost/make_shared.hpp> 

int main() 
{ 
  boost::shared_ptr<int> i1 = boost::make_shared<int>(1); 
  boost::shared_ptr<int> i2(i1); 
  i1.reset(new int(2)); 
} 