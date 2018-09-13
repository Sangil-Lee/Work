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
  s(); 
  c.disconnect(); 
} 