#include <boost/signals2.hpp> 
#include <iostream> 

boost::signals2::signal<void ()> s; 

void func() 
{ 
  std::cout << "Hello, world!" << std::endl; 
} 

void connect() 
{ 
  s.connect(func); 
} 

int main() 
{ 
  s.connect(connect); 
  s(); 
} 