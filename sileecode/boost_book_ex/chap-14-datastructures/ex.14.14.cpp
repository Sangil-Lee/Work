#include <boost/any.hpp> 
#include <iostream> 

int main() 
{ 
  try 
  { 
    boost::any a = 1; 
    std::cout << boost::any_cast<float>(a) << std::endl; 
  } 
  catch (boost::bad_any_cast &e) 
  { 
    std::cerr << e.what() << std::endl; 
  } 
} 