#include <boost/lambda/lambda.hpp> 
#include <boost/lambda/if.hpp> 
#include <iostream> 
#include <vector> 
#include <algorithm> 

int main() 
{ 
  std::vector<int> v; 
  v.push_back(1); 
  v.push_back(3); 
  v.push_back(2); 

  std::for_each(v.begin(), v.end(), 
    boost::lambda::if_then(boost::lambda::_1 > 1, 
    std::cout << boost::lambda::_1 << "\n")); 
} 