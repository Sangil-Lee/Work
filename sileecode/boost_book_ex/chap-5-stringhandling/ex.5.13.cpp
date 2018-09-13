#include <boost/algorithm/string.hpp> 
#include <locale> 
#include <iostream> 

int main() 
{ 
  std::locale::global(std::locale("German")); 
  std::string s = "123456789Boris Schäling123456789"; 
  std::cout << "." << boost::algorithm::trim_left_copy_if(s, 
    boost::algorithm::is_digit()) << "." << std::endl; 
  std::cout << "." <<boost::algorithm::trim_right_copy_if(s, 
    boost::algorithm::is_digit()) << "." << std::endl; 
  std::cout << "." <<boost::algorithm::trim_copy_if(s, 
    boost::algorithm::is_digit()) << "." << std::endl; 
} 