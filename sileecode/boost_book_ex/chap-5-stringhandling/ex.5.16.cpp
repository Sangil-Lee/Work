#include <boost/algorithm/string.hpp> 
#include <boost/algorithm/string/regex.hpp> 
#include <locale> 
#include <iostream> 

int main() 
{ 
  std::locale::global(std::locale("German")); 
  std::string s = "Boris Schäling"; 
  boost::iterator_range<std::string::iterator> r = 
    boost::algorithm::find_regex(s, boost::regex("\\w\\s\\w")); 
  std::cout << r << std::endl; 
} 