#include <boost/multi_array.hpp> 
#include <algorithm> 
#include <iostream> 
#include <cstring> 

int main() 
{ 
  char c[18] = 
  { 
    'o', 'l', 'l', 'e', 'H', '\0', '\0', '\0', '\0', 
    ',', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' 
  }; 

  boost::multi_array_ref<char, 2> a(c, boost::extents[2][9]); 

  typedef boost::multi_array<char, 2>::array_view<1>::type array_view; 
  typedef boost::multi_array_types::index_range range; 
  array_view view = a[boost::indices[0][range(0, 5)]]; 

  std::reverse(view.begin(), view.end()); 
  std::cout << view.origin() << std::endl; 

  boost::multi_array<char, 2>::reference subarray = a[1]; 
  std::cout << subarray.origin() << std::endl; 
} 