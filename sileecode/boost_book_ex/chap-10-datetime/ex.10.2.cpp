#include <boost/date_time/gregorian/gregorian.hpp> 
#include <iostream> 

int main() 
{ 
  boost::gregorian::date d = boost::gregorian::day_clock::universal_day(); 
  std::cout << d.year() << std::endl; 
  std::cout << d.month() << std::endl; 
  std::cout << d.day() << std::endl; 

  d = boost::gregorian::date_from_iso_string("20100131"); 
  std::cout << d.year() << std::endl; 
  std::cout << d.month() << std::endl; 
  std::cout << d.day() << std::endl; 
} 