#include <boost/filesystem.hpp> 
#include <iostream> 

int main() 
{ 
  boost::filesystem::path p("C:\\Windows\\win.ini"); 
  boost::system::error_code ec; 
  boost::uintmax_t filesize = boost::filesystem::file_size(p, ec); 
  if (!ec) 
    std::cout << filesize << std::endl; 
  else 
    std::cout << ec << std::endl; 
} 