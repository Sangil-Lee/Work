#include <string> 
#include <vector> 
#include <iostream> 

int main() 
{ 
  std::vector<std::string> strings; 
  strings.push_back("Boost"); 
  strings.push_back("C++"); 
  strings.push_back("Libraries"); 

  std::vector<int> sizes; 

  for (std::vector<std::string>::iterator it = strings.begin(); 
    it != strings.end(); ++it) 
    sizes.push_back(it->size()); 

  for (std::vector<int>::iterator it = sizes.begin(); 
    it != sizes.end(); ++it) 
    std::cout << *it << std::endl; 
} 