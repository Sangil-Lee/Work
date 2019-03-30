#include <boost/date_time/posix_time/posix_time.hpp> 
#include <boost/cstdint.hpp> 
#include <iostream> 

int main() 
{ 
  boost::posix_time::ptime start = 
    boost::posix_time::microsec_clock::local_time(); 

  boost::uint64_t sum = 0; 
  for (int i = 0; i < 1000000000; ++i) 
    sum += i; 

  boost::posix_time::ptime end = 
    boost::posix_time::microsec_clock::local_time(); 
  std::cout << end - start << std::endl; 

  std::cout << sum << std::endl; 
} 