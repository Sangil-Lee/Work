#include <boost/function.hpp> 
#include <iostream> 

void func() 
{ 
  std::cout << "Hello, world!" << std::endl; 
} 

int main() 
{ 
  boost::function<void ()> f; 
  f = func; 
  f(); 
} 