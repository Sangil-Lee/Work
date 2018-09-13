#include <boost/next_prior.hpp> 
#include <boost/array.hpp> 
#include <algorithm> 
#include <iostream> 

int main() 
{ 
  typedef boost::array<int, 4> array; 
  array a = { { 0, 1, 2, 3 } }; 

  array::iterator it = std::find(a.begin(), a.end(), 2); 
  array::iterator prior = boost::prior(it, 2); 
  array::iterator next = boost::next(it); 

  std::cout << *prior << std::endl; 
  std::cout << *it << std::endl; 
  std::cout << *next << std::endl; 
} 