#include <boost/filesystem.hpp> 
#include <iostream> 

int main() 
{ 
  boost::filesystem::path p("C:\\Windows\\System"); 
  for (boost::filesystem::path::iterator it = p.begin(); 
    it != p.end(); ++it) 
    std::cout << *it << std::endl; 
} 