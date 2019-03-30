#include <boost/signals2.hpp> 
#include <iostream> 
#include <vector> 
#include <algorithm> 

int func1() { return 1; } 
int func2() { return 2; } 

template <typename T> 
struct min_element 
{ 
  typedef T result_type; 

  template <typename InputIterator> 
  T operator()(InputIterator first, InputIterator last) const 
  { 
    return T(first, last); 
  } 
}; 

int main() 
{ 
  boost::signals2::signal<int (), min_element<std::vector<int> > > s; 
  s.connect(func1); 
  s.connect(func2); 
  std::vector<int> v = s(); 
  std::cout << *std::min_element(v.begin(), v.end()) << std::endl; 
} 