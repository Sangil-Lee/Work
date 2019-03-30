#include <boost/tokenizer.hpp> 
#include <string> 
#include <iostream> 

int main() 
{ 
  typedef boost::tokenizer<boost::escaped_list_separator<char> > tokenizer; 
  std::string s = "Boost,\"C++ libraries\""; 
  tokenizer tok(s); 
  for (tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) 
    std::cout << *it << std::endl; 
} 