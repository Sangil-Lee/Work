#include <boost/regex.hpp> 
#include <locale> 
#include <iostream> 

int main() 
{ 
  std::locale::global(std::locale("German")); 
  std::string s = " Boris Schäling "; 
  boost::regex expr("\\s"); 
  std::string fmt("_"); 
  std::cout << boost::regex_replace(s, expr, fmt) << std::endl; 
} 