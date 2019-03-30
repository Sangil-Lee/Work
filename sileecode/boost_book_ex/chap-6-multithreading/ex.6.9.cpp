#include <boost/thread.hpp> 
#include <iostream> 

boost::mutex mutex; 

void func() 
{ 
  static bool done = false; 
  boost::lock_guard<boost::mutex> lock(mutex); 
  if (!done) 
  { 
    done = true; 
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