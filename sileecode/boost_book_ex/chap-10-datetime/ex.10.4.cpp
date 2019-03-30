#include <boost/date_time/gregorian/gregorian.hpp> 
#include <iostream> 

int main() 
{ 
  boost::gregorian::date_duration dd(4); 
  std::cout << dd.days() << std::endl; 
  boost::gregorian::weeks ws(4); 
  std::cout << ws.days() << std::endl; 
  boost::gregorian::months ms(4); 
  std::cout << ms.number_of_months() << std::endl; 
  boost::gregorian::years ys(4); 
  std::cout << ys.number_of_years() << std::endl; 
} 