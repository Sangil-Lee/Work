#include <boost/variant.hpp> 

int main() 
{ 
  boost::variant<double, char> v; 
  v = 3.14; 
  v = 'A'; 
} 