#include <boost/circular_buffer.hpp> 
#include <iostream> 

int main() 
{ 
  typedef boost::circular_buffer<int> circular_buffer; 
  circular_buffer cb(3); 

  cb.push_back(0); 
  cb.push_back(1); 
  cb.push_back(2); 
  cb.push_back(3); 

  std::cout << cb.is_linearized() << std::endl; 

  circular_buffer::array_range ar1, ar2; 

  ar1 = cb.array_one(); 
  ar2 = cb.array_two(); 
  std::cout << ar1.second << ", " << ar2.second << std::endl; 

  for (circular_buffer::iterator it = cb.begin(); it != cb.end(); ++it) 
    std::cout << *it << std::endl; 

  cb.linearize(); 

  ar1 = cb.array_one(); 
  ar2 = cb.array_two(); 
  std::cout << ar1.second << ", " << ar2.second << std::endl; 
} 