#include <boost/algorithm/minmax.hpp> 
#include <boost/tuple/tuple.hpp> 
#include <iostream> 

int main() 
{ 
  int i = 6; 
  int j = 5; 

  boost::tuples::tuple<const int&, const int&> t = boost::minmax(i, j); 

  std::cout << t.get<0>() << std::endl; 
  std::cout << t.get<1>() << std::endl; 
} 