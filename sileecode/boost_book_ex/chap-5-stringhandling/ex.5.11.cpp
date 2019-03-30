#include <boost/algorithm/string.hpp> 
#include <locale> 
#include <iostream> 

int main() 
{ 
  std::locale::global(std::locale("German")); 
  std::string s = "\t Boris Schäling \t"; 
  std::cout << "." << boost::algorithm::trim_left_copy(s) << "." << 
    std::endl; 
  std::cout << "." <<boost::algorithm::trim_right_copy(s) << "." << 
    std::endl; 
  std::cout << "." <<boost::algorithm::trim_copy(s) << "." << 
    std::endl; 
} 