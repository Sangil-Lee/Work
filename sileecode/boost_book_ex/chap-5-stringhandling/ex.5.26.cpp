#include <boost/tokenizer.hpp> 
#include <string> 
#include <iostream> 

int main() 
{ 
  typedef boost::tokenizer<boost::char_separator<wchar_t>, 
    std::wstring::const_iterator, std::wstring> tokenizer; 
  std::wstring s = L"Boost C++ libraries"; 
  boost::char_separator<wchar_t> sep(L" "); 
  tokenizer tok(s, sep); 
  for (tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) 
    std::wcout << *it << std::endl; 
} 