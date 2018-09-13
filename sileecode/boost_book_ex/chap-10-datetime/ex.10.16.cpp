#include <boost/date_time/local_time/local_time.hpp> 
#include <iostream> 

int main() 
{ 
  boost::posix_time::ptime pt(boost::gregorian::date(2009, 1, 05), 
    boost::posix_time::time_duration(12, 0, 0)); 
  boost::posix_time::time_iterator it(pt, 
    boost::posix_time::time_duration(6, 30, 0)); 
  std::cout << *++it << std::endl; 
  std::cout << *++it << std::endl; 
} 