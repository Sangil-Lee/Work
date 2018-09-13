#include <boost/numeric/conversion/cast.hpp> 
#include <iostream> 

int main() 
{ 
  try 
  { 
    int i = 0x10000; 
    short s = boost::numeric_cast<short>(i); 
    std::cout << s << std::endl; 
  } 
  catch (boost::numeric::bad_numeric_cast &e) 
  { 
    std::cerr << e.what() << std::endl; 
  } 
} 