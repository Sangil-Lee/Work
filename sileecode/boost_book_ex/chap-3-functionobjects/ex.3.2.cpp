#include <iostream> 
#include <vector> 
#include <algorithm> 
#include <functional> 

class add 
  : public std::binary_function<int, int, void> 
{ 
public: 
  void operator()(int i, int j) const 
  { 
    std::cout << i + j << std::endl; 
  } 
}; 

int main() 
{ 
  std::vector<int> v; 
  v.push_back(1); 
  v.push_back(3); 
  v.push_back(2); 

  std::for_each(v.begin(), v.end(), std::bind1st(add(), 10)); 
} 