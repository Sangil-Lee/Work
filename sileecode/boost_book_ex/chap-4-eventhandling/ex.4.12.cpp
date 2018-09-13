#include <boost/signals2.hpp> 
#include <iostream> 

void func() 
{ 
  std::cout << "Hello, world!" << std::endl; 
} 

int main() 
{ 
  boost::signals2::signal<void ()> s; 
  boost::signals2::connection c = s.connect(func); 
  boost::signals2::shared_connection_block b1(c, false); 
  { 
    boost::signals2::shared_connection_block b2(c); 
    std::cout << b1.blocking() << std::endl; 
    s(); 
  } 
  s(); 
} 