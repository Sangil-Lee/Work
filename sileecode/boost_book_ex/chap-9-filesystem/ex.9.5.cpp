#include <boost/filesystem.hpp> 
#include <iostream> 

int main() 
{ 
  boost::filesystem::path p("C:\\Windows\\System"); 
  std::cout << p.root_name() << std::endl; 
  std::cout << p.root_directory() << std::endl; 
  std::cout << p.root_path() << std::endl; 
  std::cout << p.relative_path() << std::endl; 
  std::cout << p.parent_path() << std::endl; 
  std::cout << p.filename() << std::endl; 
} 