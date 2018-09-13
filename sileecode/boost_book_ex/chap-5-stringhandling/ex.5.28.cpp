#include <boost/tokenizer.hpp> 
#include <string> 
#include <iostream> 

int main() 
{ 
  typedef boost::tokenizer<boost::offset_separator> tokenizer; 
  std::string s = "Boost C++ libraries"; 
  int offsets[] = { 5, 5, 9 }; 
  boost::offset_separator sep(offsets, offsets + 3); 
  tokenizer tok(s, sep); 
  for (tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) 
    std::cout << *it << std::endl; 
} 