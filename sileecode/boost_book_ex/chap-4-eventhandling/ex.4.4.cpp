#include <boost/signals2.hpp> 
#include <iostream> 

void func1() 
{ 
  std::cout << "Hello" << std::flush; 
} 

void func2() 
{ 
  std::cout << ", world!" << std::endl; 
} 

int main() 
{ 
  boost::signals2::signal<void ()> s; 
  s.connect(1, func2); 
  s.connect(0, func1); 
  s(); 
} 