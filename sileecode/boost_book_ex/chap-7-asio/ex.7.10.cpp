#include <boost/asio.hpp> 
#include <iostream> 
#include "basic_timer.hpp" 
#include "timer_impl.hpp" 
#include "basic_timer_service.hpp" 

void wait_handler(const boost::system::error_code &ec) 
{ 
  std::cout << "5 s." << std::endl; 
} 

typedef basic_timer<basic_timer_service<> > timer; 

int main() 
{ 
  boost::asio::io_service io_service; 
  timer t(io_service); 
  t.async_wait(5, wait_handler); 
  io_service.run(); 
} 