#include <boost/system/error_code.hpp> 
#include <boost/asio.hpp> 
#include <iostream> 
#include <string> 

int main() 
{ 
  boost::system::error_code ec; 
  std::string hostname = boost::asio::ip::host_name(ec); 
  boost::system::error_condition ecnd = ec.default_error_condition(); 
  std::cout << ecnd.value() << std::endl; 
  std::cout << ecnd.category().name() << std::endl; 
} 