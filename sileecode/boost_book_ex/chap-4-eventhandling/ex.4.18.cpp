#include <boost/signals2.hpp> 
#include <iostream> 

boost::signals2::signal<void ()> s; 

void func() 
{ 
  std::cout << "Hello, world!" << std::endl; 
} 

int main() 
{ 
  typedef boost::signals2::keywords::mutex_type<
    boost::signals2::dummy_mutex> dummy_mutex; 
  boost::signals2::signal_type<void (), dummy_mutex>::type s; 
  s.connect(func); 
  s(); 
} 