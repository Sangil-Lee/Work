#include <boost/algorithm/string.hpp> 
#include <locale> 
#include <iostream> 
#include <clocale> 

int main() 
{ 
  std::setlocale(LC_ALL, "German"); 
  std::string s = "Boris Schäling"; 
  std::cout << boost::algorithm::to_upper_copy(s) << std::endl; 
  std::cout << boost::algorithm::to_upper_copy(s, std::locale("German")) << 
    std::endl; 
} 