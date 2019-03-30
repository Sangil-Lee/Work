#include <boost/system/error_code.hpp> 
#include <iostream> 
#include <string> 

class application_category : 
  public boost::system::error_category 
{ 
public: 
  const char *name() const { return "application"; } 
  std::string message(int ev) const { return "error message"; } 
}; 

application_category cat; 

int main() 
{ 
  boost::system::error_code ec(14, cat); 
  std::cout << ec.value() << std::endl; 
  std::cout << ec.category().name() << std::endl; 
} 