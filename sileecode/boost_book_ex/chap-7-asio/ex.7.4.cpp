#include <boost/asio.hpp> 
#include <boost/thread.hpp> 
#include <iostream> 

void handler1(const boost::system::error_code &ec) 
{ 
  std::cout << "5 s." << std::endl; 
} 

void handler2(const boost::system::error_code &ec) 
{ 
  std::cout << "5 s." << std::endl; 
} 

boost::asio::io_service io_service1; 
boost::asio::io_service io_service2; 

void run1() 
{ 
  io_service1.run(); 
} 

void run2() 
{ 
  io_service2.run(); 
} 

int main() 
{ 
  boost::asio::deadline_timer timer1(io_service1, 
    boost::posix_time::seconds(5)); 
  timer1.async_wait(handler1); 
  boost::asio::deadline_timer timer2(io_service2, 
    boost::posix_time::seconds(5)); 
  timer2.async_wait(handler2); 
  boost::thread thread1(run1); 
  boost::thread thread2(run2); 
  thread1.join(); 
  thread2.join(); 
} 