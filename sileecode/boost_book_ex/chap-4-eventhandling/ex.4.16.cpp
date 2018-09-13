#include <boost/signals2.hpp> 
#include <iostream> 

boost::signals2::signal<void ()> s; 

void func() 
{ 
  std::cout << "Hello, world!" << std::endl; 
} 

void disconnect() 
{ 
  s.disconnect(func); 
} 

int main() 
{ 
  s.connect(disconnect); 
  s.connect(func); 
  s(); 
} 