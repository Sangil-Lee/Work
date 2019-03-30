#include <boost/date_time/gregorian/gregorian.hpp> 
#include <iostream> 

int main() 
{ 
  boost::gregorian::date d(2010, 1, 30); 
  std::cout << d.year() << std::endl; 
  std::cout << d.month() << std::endl; 
  std::cout << d.day() << std::endl; 
  std::cout << d.day_of_week() << std::endl; 
  std::cout << d.end_of_month() << std::endl; 
} 