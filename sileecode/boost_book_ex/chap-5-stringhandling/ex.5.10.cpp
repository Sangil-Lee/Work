#include <boost/algorithm/string.hpp> 
#include <locale> 
#include <iostream> 

int main() 
{ 
  std::locale::global(std::locale("German")); 
  std::string s = "Boris Schäling"; 
  std::cout << boost::algorithm::replace_first_copy(s, "B", "D") << 
    std::endl; 
  std::cout << boost::algorithm::replace_nth_copy(s, "B", 0, "D") << 
    std::endl; 
  std::cout << boost::algorithm::replace_last_copy(s, "B", "D") << 
    std::endl; 
  std::cout << boost::algorithm::replace_all_copy(s, "B", "D") << 
    std::endl; 
  std::cout << boost::algorithm::replace_head_copy(s, 5, "Doris") << 
    std::endl; 
  std::cout << boost::algorithm::replace_tail_copy(s, 8, "Becker") << 
    std::endl; 
} 