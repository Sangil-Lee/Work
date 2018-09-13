#include <boost/tuple/tuple.hpp> 
#include <boost/tuple/tuple_io.hpp> 
#include <string> 
#include <iostream> 

int main() 
{ 
  typedef boost::tuple<std::string&, std::string&, int&> person; 

  std::string firstname = "Boris"; 
  std::string surname = "Schaeling"; 
  int shoesize = 43; 
  person p = boost::tie(firstname, surname, shoesize); 
  surname = "Becker"; 
  std::cout << p << std::endl; 
} 