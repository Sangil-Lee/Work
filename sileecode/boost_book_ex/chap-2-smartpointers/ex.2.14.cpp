#include <vector> 

template <typename T> 
T *create() 
{ 
  return new T; 
} 

int main() 
{ 
  std::vector<int*> v; 
  v.push_back(create<int>()); 
} 