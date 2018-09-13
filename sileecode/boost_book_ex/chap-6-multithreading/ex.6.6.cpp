#include <boost/thread.hpp> 
#include <iostream> 

void wait(int seconds) 
{ 
  boost::this_thread::sleep(boost::posix_time::seconds(seconds)); 
} 

boost::timed_mutex mutex; 

void thread() 
{ 
  for (int i = 0; i < 5; ++i) 
  { 
    wait(1); 
    boost::unique_lock<boost::timed_mutex> lock(mutex, boost::try_to_lock); 
    if (!lock.owns_lock()) 
    { 
      lock.timed_lock(boost::get_system_time() + 
        boost::posix_time::seconds(1)); 
    } 
    std::cout << "Thread " << boost::this_thread::get_id() << ": " << i << 
      std::endl; 
    boost::timed_mutex *m = lock.release(); 
    m->unlock(); 
  } 
} 

int main() 
{ 
  boost::thread t1(thread); 
  boost::thread t2(thread); 
  t1.join(); 
  t2.join(); 
} 