#include <boost/date_time/local_time/local_time.hpp> 
#include <iostream> 

int main() 
{ 
  boost::local_time::time_zone_ptr tz(
    new boost::local_time::posix_time_zone("CET+1")); 
  boost::posix_time::ptime pt(boost::gregorian::date(2009, 1, 5), 
    boost::posix_time::time_duration(12, 0, 0)); 
  boost::local_time::local_date_time dt(pt, tz); 
  std::cout << dt.local_time() << std::endl; 
  boost::local_time::time_zone_ptr tz2(
    new boost::local_time::posix_time_zone("EET+2")); 
  std::cout << dt.local_time_in(tz2).local_time() << std::endl; 
} 