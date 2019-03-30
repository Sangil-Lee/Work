#include <boost/filesystem.hpp> 
#include <iostream> 

int main() 
{ 
  try 
  { 
    std::cout << boost::filesystem::absolute("photo.jpg", "D:\\") << 
      std::endl; 
  } 
  catch (boost::filesystem::filesystem_error &e) 
  { 
    std::cerr << e.what() << std::endl; 
  } 
} 