#include <boost/date_time/posix_time/posix_time.hpp> 
#include <iostream> 

int main() 
{ 
  boost::posix_time::time_duration td(16, 30, 0); 
  std::cout << td.hours() << std::endl; 
  std::cout << td.minutes() << std::endl; 
  std::cout << td.seconds() << std::endl; 
  std::cout << td.total_seconds() << std::endl; 
} 