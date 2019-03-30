#include <boost/date_time/gregorian/gregorian.hpp> 
#include <iostream> 
#include <locale> 
#include <string> 
#include <vector> 

int main() 
{ 
  std::locale::global(std::locale("German")); 
  std::string months[12] = { "Januar", "Februar", "März", "April", 
    "Mai", "Juni", "Juli", "August", "September", "Oktober", 
    "November", "Dezember" }; 
  std::string weekdays[7] = { "Sonntag", "Montag", "Dienstag", 
    "Mittwoch", "Donnerstag", "Freitag", "Samstag" }; 
  boost::gregorian::date d(2009, 1, 7); 
  boost::gregorian::date_facet *df = new boost::gregorian::date_facet(
    "%A, %d. %B %Y"); 
  df->long_month_names(std::vector<std::string>(months, months + 12)); 
  df->long_weekday_names(std::vector<std::string>(weekdays, weekdays + 7)); 
  std::cout.imbue(std::locale(std::cout.getloc(), df)); 
  std::cout << d << std::endl; 
} 