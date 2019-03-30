#include <boost/bind.hpp> 
#include <vector> 
#include <algorithm> 

bool compare(int i, int j) 
{ 
  return i > j; 
} 

int main() 
{ 
  std::vector<int> v; 
  v.push_back(1); 
  v.push_back(3); 
  v.push_back(2); 

  std::sort(v.begin(), v.end(), compare); 
} 