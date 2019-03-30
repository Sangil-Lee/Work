#include <locale> 
#include <iostream> 
#include <cstring> 

int main() 
{ 
  std::cout << std::strcoll("ä", "z") << std::endl; 
  std::locale::global(std::locale("German")); 
  std::cout << std::strcoll("ä", "z") << std::endl; 
} 