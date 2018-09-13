#include <windows.h> 
#include <boost/filesystem.hpp> 
#include <iostream> 

int main() 
{ 
  try 
  { 
    std::cout << boost::filesystem::current_path() << std::endl; 
    SetCurrentDirectoryA("C:\\"); 
    std::cout << boost::filesystem::current_path() << std::endl; 
  } 
  catch (boost::filesystem::filesystem_error &e) 
  { 
    std::cerr << e.what() << std::endl; 
  } 
} 