#include <boost/date_time/gregorian/gregorian.hpp> 
#include <iostream> 

int main() 
{ 
  boost::gregorian::date d1(2009, 1, 30); 
  boost::gregorian::date d2(2009, 10, 31); 
  boost::gregorian::date_period dp(d1, d2); 
  std::cout << dp.contains(d1) << std::endl; 
  std::cout << dp.contains(d2) << std::endl; 
} 