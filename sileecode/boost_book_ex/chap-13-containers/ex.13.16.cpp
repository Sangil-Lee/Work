#include <boost/circular_buffer.hpp> 
#include <iostream> 

int main() 
{ 
  typedef boost::circular_buffer<int> circular_buffer; 
  circular_buffer cb(3); 

  std::cout << cb.capacity() << std::endl; 
  std::cout << cb.size() << std::endl; 

  cb.push_back(0); 
  cb.push_back(1); 
  cb.push_back(2); 

  std::cout << cb.size() << std::endl; 

  cb.push_back(3); 
  cb.push_back(4); 
  cb.push_back(5); 

  std::cout << cb.size() << std::endl; 

  for (circular_buffer::iterator it = cb.begin(); it != cb.end(); ++it) 
    std::cout << *it << std::endl; 
} 