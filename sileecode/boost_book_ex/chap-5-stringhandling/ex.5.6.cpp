#include <boost/algorithm/string.hpp> 
#include <locale> 
#include <iostream> 

int main() 
{ 
  std::locale::global(std::locale("German")); 
  std::string s = "Boris Schäling"; 
  std::cout << boost::algorithm::to_upper_copy(s) << std::endl; 
  std::cout << boost::algorithm::to_upper_copy(s, std::locale("German")) << 
    std::endl; 
} 