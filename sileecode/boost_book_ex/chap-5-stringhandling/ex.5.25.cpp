#include <boost/tokenizer.hpp> 
#include <string> 
#include <iostream> 

int main() 
{ 
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer; 
  std::string s = "Boost C++ libraries"; 
  boost::char_separator<char> sep(" ", "+", boost::keep_empty_tokens); 
  tokenizer tok(s, sep); 
  for (tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) 
    std::cout << *it << std::endl; 
} 