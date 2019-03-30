#include <boost/thread.hpp> 
#include <iostream> 

boost::mutex mutex; 

void func() 
{ 
  static boost::thread_specific_ptr<bool> tls; 
  if (!tls.get()) 
  { 
    tls.reset(new bool(true)); 
    boost::lock_guard<boost::mutex> lock(mutex); 
    std::cout << "done" << std::endl; 
  } 
} 

void thread() 
{ 
  func(); 
  func(); 
} 

int main() 
{ 
  boost::thread t[3]; 

  for (int i = 0; i < 3; ++i) 
    t[i] = boost::thread(thread); 

  for (int i = 0; i < 3; ++i) 
    t[i].join(); 
} 