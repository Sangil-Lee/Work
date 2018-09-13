#include <boost/filesystem.hpp> 
#include <iostream> 

int main() 
{ 
  boost::filesystem::path p("C:\\Test"); 
  try 
  { 
    if (boost::filesystem::create_directory(p)) 
    { 
      boost::filesystem::rename(p, "C:\\Test2"); 
      boost::filesystem::remove("C:\\Test2"); 
    } 
  } 
  catch (boost::filesystem::filesystem_error &e) 
  { 
    std::cerr << e.what() << std::endl; 
  } 
} 