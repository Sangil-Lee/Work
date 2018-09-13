#include <boost/signals2.hpp> 
#include <boost/bind.hpp> 
#include <iostream> 
#include <memory> 

class world 
{ 
  public: 
    void hello() const 
    { 
      std::cout << "Hello, world!" << std::endl; 
    } 
}; 

int main() 
{ 
  boost::signals2::signal<void ()> s; 
  { 
    std::auto_ptr<world> w(new world()); 
    s.connect(boost::bind(&world::hello, w.get())); 
  } 
  std::cout << s.num_slots() << std::endl; 
  s(); 
} 