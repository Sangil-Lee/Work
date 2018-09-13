#include <boost/filesystem.hpp> 
#include <iostream> 

int main() 
{ 
  boost::filesystem::path p("C:\\"); 
  try 
  { 
    boost::filesystem::file_status s = boost::filesystem::status(p); 
    std::cout << boost::filesystem::is_directory(s) << std::endl; 
  } 
  catch (boost::filesystem::filesystem_error &e) 
  { 
    std::cerr << e.what() << std::endl; 
  } 
} 