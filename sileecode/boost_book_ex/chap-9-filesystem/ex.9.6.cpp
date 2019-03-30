#include <boost/filesystem.hpp> 
#include <iostream> 

int main() 
{ 
  boost::filesystem::path p("photo.jpg"); 
  std::cout << p.stem() << std::endl; 
  std::cout << p.extension() << std::endl; 
} 