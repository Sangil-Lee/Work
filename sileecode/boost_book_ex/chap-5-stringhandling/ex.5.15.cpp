#include <boost/algorithm/string.hpp> 
#include <locale> 
#include <iostream> 
#include <vector> 

int main() 
{ 
  std::locale::global(std::locale("German")); 
  std::string s = "Boris Schäling"; 
  std::vector<std::string> v; 
  boost::algorithm::split(v, s, boost::algorithm::is_space()); 
  std::cout << v.size() << std::endl; 
} 