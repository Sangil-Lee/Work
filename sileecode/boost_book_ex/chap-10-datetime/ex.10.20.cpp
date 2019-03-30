#include <boost/date_time/gregorian/gregorian.hpp> 
#include <iostream> 
#include <locale> 

int main() 
{ 
  boost::gregorian::date d(2009, 1, 7); 
  boost::gregorian::date_facet *df = new boost::gregorian::date_facet(
    "%A, %d %B %Y"); 
  std::cout.imbue(std::locale(std::cout.getloc(), df)); 
  std::cout << d << std::endl; 
} 