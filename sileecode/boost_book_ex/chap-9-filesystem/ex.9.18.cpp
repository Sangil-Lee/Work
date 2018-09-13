#include <boost/filesystem.hpp> 
#include <iostream> 

int main() 
{ 
  boost::filesystem::path p = boost::filesystem::current_path(); 
  boost::filesystem::directory_iterator it(p); 
  while (it != boost::filesystem::directory_iterator()) 
    std::cout << *it++ << std::endl; 
} 