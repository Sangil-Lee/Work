#include <boost/scoped_ptr.hpp> 

int main() 
{ 
  boost::scoped_ptr<int> i(new int); 
  *i = 1; 
  *i.get() = 2; 
  i.reset(new int); 
} 