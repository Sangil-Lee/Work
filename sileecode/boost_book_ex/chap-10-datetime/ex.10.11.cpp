#include <boost/date_time/posix_time/posix_time.hpp> 
#include <boost/date_time/gregorian/gregorian.hpp> 
#include <iostream> 

int main() 
{ 
  boost::posix_time::ptime pt = 
    boost::posix_time::second_clock::universal_time(); 
  std::cout << pt.date() << std::endl; 
  std::cout << pt.time_of_day() << std::endl; 

  pt = boost::posix_time::from_iso_string("20090105T120000"); 
  std::cout << pt.date() << std::endl; 
  std::cout << pt.time_of_day() << std::endl; 
} 