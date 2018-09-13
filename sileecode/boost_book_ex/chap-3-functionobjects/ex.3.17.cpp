#include <vector> 
#include <iostream> 
#include <cstdlib> 
#include <cstring> 

int main() 
{ 
  std::vector<int(*)(const char*)> processors; 
  processors.push_back(std::atoi); 
  processors.push_back(
    reinterpret_cast<int(*)(const char*)>(std::strlen)); 

  const char data[] = "1.23"; 

  for (std::vector<int(*)(const char*)>::iterator it = 
    processors.begin(); it != processors.end(); ++it) 
    std::cout << (*it)(data) << std::endl; 
} 