#include <boost/date_time/local_time/local_time.hpp> 
#include <iostream> 

int main() 
{ 
  boost::local_time::time_zone_ptr tz(
    new boost::local_time::posix_time_zone("CET+1")); 
  boost::posix_time::ptime pt(boost::gregorian::date(2009, 1, 5), 
    boost::posix_time::time_duration(12, 0, 0)); 
  boost::local_time::local_date_time dt(pt, tz); 
  std::cout << dt.utc_time() << std::endl; 
  std::cout << dt << std::endl; 
  std::cout << dt.local_time() << std::endl; 
  std::cout << dt.zone_name() << std::endl; 
} 