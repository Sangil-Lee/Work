#include <boost/algorithm/string.hpp> 
#include <locale> 
#include <iostream> 

int main() 
{ 
  std::locale::global(std::locale("German")); 
  std::string s = "Boris Schäling"; 
  std::cout << boost::algorithm::starts_with(s, "Boris") << std::endl; 
  std::cout << boost::algorithm::ends_with(s, "Schäling") << std::endl; 
  std::cout << boost::algorithm::contains(s, "is") << std::endl; 
  std::cout << boost::algorithm::lexicographical_compare(s, "Boris") << 
    std::endl; 
} 