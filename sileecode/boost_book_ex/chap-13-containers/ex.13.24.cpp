#include <boost/multi_array.hpp> 
#include <algorithm> 
#include <iostream> 
#include <cstring> 

int main() 
{ 
  boost::multi_array<char, 2> a(boost::extents[2][9]); 

  typedef boost::multi_array<char, 2>::array_view<1>::type array_view; 
  typedef boost::multi_array_types::index_range range; 
  array_view view = a[boost::indices[0][range(0, 5)]]; 

  std::memcpy(view.origin(), "olleH", 6); 
  std::reverse(view.begin(), view.end()); 

  std::cout << view.origin() << std::endl; 

  boost::multi_array<char, 2>::reference subarray = a[1]; 
  std::memcpy(subarray.origin(), ", world!", 9); 

  std::cout << subarray.origin() << std::endl; 
} 