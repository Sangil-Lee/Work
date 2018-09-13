#include <boost/regex.hpp> 
#include <locale> 
#include <iostream> 

int main() 
{ 
  std::locale::global(std::locale("German")); 
  std::string s = "Boris Schäling"; 
  boost::regex expr("(\\w+)\\s(\\w+)"); 
  std::string fmt("\\2 \\1"); 
  std::cout << boost::regex_replace(s, expr, fmt) << std::endl; 
} 