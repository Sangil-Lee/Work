#include <boost/asio.hpp> 
#include <boost/system/system_error.hpp> 
#include <iostream> 

int main() 
{ 
  try 
  { 
    std::cout << boost::asio::ip::host_name() << std::endl; 
  } 
  catch (boost::system::system_error &e) 
  { 
    boost::system::error_code ec = e.code(); 
    std::cerr << ec.value() << std::endl; 
    std::cerr << ec.category().name() << std::endl; 
  } 
} 