#include <boost/filesystem.hpp> 
#include <iostream> 
#include <ctime> 

int main() 
{ 
  boost::filesystem::path p("C:\\Windows\\win.ini"); 
  try 
  { 
    std::time_t t = boost::filesystem::last_write_time(p); 
    std::cout << std::ctime(&t) << std::endl; 
  } 
  catch (boost::filesystem::filesystem_error &e) 
  { 
    std::cerr << e.what() << std::endl; 
  } 
} 