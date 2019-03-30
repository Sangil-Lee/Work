#include <boost/filesystem.hpp> 
#include <iostream> 

int main() 
{ 
  boost::filesystem::path p("C:/Windows/System"); 
  std::cout << p.make_preferred() << std::endl; 
} 