#include <boost/algorithm/string.hpp> 
#include <locale> 
#include <iostream> 

int main() 
{ 
  std::locale::global(std::locale("German")); 
  std::string s = "Boris Schäling"; 
  std::cout << boost::algorithm::erase_first_copy(s, "i") << std::endl; 
  std::cout << boost::algorithm::erase_nth_copy(s, "i", 0) << std::endl; 
  std::cout << boost::algorithm::erase_last_copy(s, "i") << std::endl; 
  std::cout << boost::algorithm::erase_all_copy(s, "i") << std::endl; 
  std::cout << boost::algorithm::erase_head_copy(s, 5) << std::endl; 
  std::cout << boost::algorithm::erase_tail_copy(s, 8) << std::endl; 
} 