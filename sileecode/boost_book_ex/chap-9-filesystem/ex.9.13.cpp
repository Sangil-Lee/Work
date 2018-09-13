#include <boost/filesystem.hpp> 
#include <iostream> 

int main() 
{ 
  boost::filesystem::path p("C:\\"); 
  try 
  { 
    boost::filesystem::space_info s = boost::filesystem::space(p); 
    std::cout << s.capacity << std::endl; 
    std::cout << s.free << std::endl; 
    std::cout << s.available << std::endl; 
  } 
  catch (boost::filesystem::filesystem_error &e) 
  { 
    std::cerr << e.what() << std::endl; 
  } 
} 