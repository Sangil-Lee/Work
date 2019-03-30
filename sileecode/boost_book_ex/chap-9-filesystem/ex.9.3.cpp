#include <boost/filesystem.hpp> 
#include <iostream> 

int main() 
{ 
  boost::filesystem::path p("C:\\Windows\\System"); 

  std::wcout << p.native() << std::endl; 

  std::cout << p.string() << std::endl; 
  std::wcout << p.wstring() << std::endl; 

  std::cout << p.generic_string() << std::endl; 
  std::wcout << p.generic_wstring() << std::endl; 
} 