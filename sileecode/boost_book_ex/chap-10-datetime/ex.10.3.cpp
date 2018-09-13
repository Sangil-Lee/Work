#include <boost/date_time/gregorian/gregorian.hpp> 
#include <iostream> 

int main() 
{ 
  boost::gregorian::date d1(2008, 1, 31); 
  boost::gregorian::date d2(2008, 8, 31); 
  boost::gregorian::date_duration dd = d2 - d1; 
  std::cout << dd.days() << std::endl; 
} 