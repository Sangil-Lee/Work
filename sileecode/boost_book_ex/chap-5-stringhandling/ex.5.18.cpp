#include <boost/regex.hpp> 
#include <locale> 
#include <iostream> 

int main() 
{ 
  std::locale::global(std::locale("German")); 
  std::string s = "Boris Schäling"; 
  boost::regex expr("(\\w+)\\s(\\w+)"); 
  boost::smatch what; 
  if (boost::regex_search(s, what, expr)) 
  { 
    std::cout << what[0] << std::endl; 
    std::cout << what[1] << " " << what[2] << std::endl; 
  } 
} 