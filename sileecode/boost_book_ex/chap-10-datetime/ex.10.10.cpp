#include <boost/date_time/posix_time/posix_time.hpp> 
#include <boost/date_time/gregorian/gregorian.hpp> 
#include <iostream> 

int main() 
{ 
  boost::posix_time::ptime pt(boost::gregorian::date(2009, 1, 5), 
    boost::posix_time::time_duration(12, 0, 0)); 
  boost::gregorian::date d = pt.date(); 
  std::cout << d << std::endl; 
  boost::posix_time::time_duration td = pt.time_of_day(); 
  std::cout << td << std::endl; 
} 