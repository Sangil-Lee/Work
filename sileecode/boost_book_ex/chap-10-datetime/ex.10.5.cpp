#include <boost/date_time/gregorian/gregorian.hpp> 
#include <iostream> 

int main() 
{ 
  boost::gregorian::date d(2009, 1, 31); 
  boost::gregorian::months ms(1); 
  boost::gregorian::date d2 = d + ms; 
  std::cout << d2 << std::endl; 
  boost::gregorian::date d3 = d2 - ms; 
  std::cout << d3 << std::endl; 
} 