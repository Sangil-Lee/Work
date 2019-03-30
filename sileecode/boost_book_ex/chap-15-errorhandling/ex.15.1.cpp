#include <boost/system/error_code.hpp> 
#include <boost/asio.hpp> 
#include <iostream> 
#include <string> 

int main() 
{ 
  boost::system::error_code ec; 
  std::string hostname = boost::asio::ip::host_name(ec); 
  std::cout << ec.value() << std::endl; 
} 