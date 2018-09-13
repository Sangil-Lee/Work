#include <boost/exception/all.hpp> 
#include <boost/lexical_cast.hpp> 
#include <boost/shared_array.hpp> 
#include <exception> 
#include <string> 
#include <iostream> 

typedef boost::error_info<struct tag_errmsg, std::string> errmsg_info; 

class allocation_failed : 
  public std::exception 
{ 
public: 
  allocation_failed(std::size_t size) 
    : what_("allocation of " + boost::lexical_cast<std::string>(size) + 
      " bytes failed") 
  { 
  } 

  virtual const char *what() const throw() 
  { 
    return what_.c_str(); 
  } 

private: 
  std::string what_; 
}; 

boost::shared_array<char> allocate(std::size_t size) 
{ 
  if (size > 1000) 
    BOOST_THROW_EXCEPTION(allocation_failed(size)); 
  return boost::shared_array<char>(new char[size]); 
} 

void save_configuration_data() 
{ 
  try 
  { 
    boost::shared_array<char> a = allocate(2000); 
    // saving configuration data ... 
  } 
  catch (boost::exception &e) 
  { 
    e << errmsg_info("saving configuration data failed"); 
    throw; 
  } 
} 

int main() 
{ 
  try 
  { 
    save_configuration_data(); 
  } 
  catch (boost::exception &e) 
  { 
    std::cerr << boost::diagnostic_information(e); 
  } 
} 