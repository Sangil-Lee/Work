#include <locale> 
#include <iostream> 

int main() 
{ 
  std::locale::global(std::locale("German_Switzerland")); 
  std::locale loc; 
  std::cout << loc.name() << std::endl; 
} 