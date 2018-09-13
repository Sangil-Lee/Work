#include <boost/date_time/gregorian/gregorian.hpp> 
#include <iostream> 

int main() 
{ 
  boost::gregorian::date d(2009, 1, 5); 
  boost::gregorian::day_iterator it(d); 
  std::cout << *++it << std::endl; 
  std::cout << boost::date_time::next_weekday(*it, 
    boost::gregorian::greg_weekday(boost::date_time::Friday)) << std::endl; 
} 