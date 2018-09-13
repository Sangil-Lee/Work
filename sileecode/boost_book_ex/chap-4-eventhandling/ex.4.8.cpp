#include <boost/signals2.hpp> 
#include <iostream> 
#include <algorithm> 
#include <vector> 

int func1() { return 1; } 
int func2() { return 2; } 

template <typename T> 
struct min_element 
{ 
  typedef T result_type; 

  template <typename InputIterator> 
  T operator()(InputIterator first, InputIterator last) const 
  { 
    std::vector<T> v(first, last); 
    return *std::min_element(v.begin(), v.end()); 
  } 
}; 

int main() 
{ 
  boost::signals2::signal<int (), min_element<int> > s; 
  s.connect(func1); 
  s.connect(func2); 
  std::cout << s() << std::endl; 
} 