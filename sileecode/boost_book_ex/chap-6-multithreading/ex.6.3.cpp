#include <boost/thread.hpp> 
#include <iostream> 

int main() 
{ 
  std::cout << boost::this_thread::get_id() << std::endl; 
  std::cout << boost::thread::hardware_concurrency() << std::endl; 
} 