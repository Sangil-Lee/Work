#include <boost/date_time/local_time/local_time.hpp> 
#include <iostream> 

int main() 
{ 
  boost::local_time::time_zone_ptr tz(
    new boost::local_time::posix_time_zone("CET+0")); 
  boost::posix_time::ptime pt1(boost::gregorian::date(2009, 1, 5), 
    boost::posix_time::time_duration(12, 0, 0)); 
  boost::local_time::local_date_time dt1(pt1, tz); 
  boost::posix_time::ptime pt2(boost::gregorian::date(2009, 1, 5), 
    boost::posix_time::time_duration(18, 0, 0)); 
  boost::local_time::local_date_time dt2(pt2, tz); 
  boost::local_time::local_time_period tp(dt1, dt2); 
  std::cout << tp.contains(dt1) << std::endl; 
  std::cout << tp.contains(dt2) << std::endl; 
} 