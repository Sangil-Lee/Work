#include <boost/filesystem.hpp> 
#include <iostream> 

int main() 
{ 
  boost::filesystem::path p("C:\\"); 
  p /= "Windows\\System"; 
  std::cout << p.string() << std::endl; 
} 