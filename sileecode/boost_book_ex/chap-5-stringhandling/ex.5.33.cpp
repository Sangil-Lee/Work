#include <boost/format.hpp> 
#include <iostream> 

int main() 
{ 
  try 
  { 
    std::cout << boost::format("%|+| %2% %1%") % 99 % 100 << std::endl; 
  } 
  catch (boost::io::format_error &ex) 
  { 
    std::cout << ex.what() << std::endl; 
  } 
} 