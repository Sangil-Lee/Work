#include <boost/signals2.hpp> 
#include <boost/shared_ptr.hpp> 
#include <iostream> 

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
    boost::shared_ptr<world> w(new world()); 
    s.connect(boost::signals2::signal<void ()>::slot_type( 
      &world::hello, w.get()).track(w)); 
  } 
  std::cout << s.num_slots() << std::endl; 
  s(); 
} 