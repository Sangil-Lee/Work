#include <boost/archive/text_oarchive.hpp> 
#include <iostream> 

int main() 
{ 
  boost::archive::text_oarchive oa(std::cout); 
  int i = 1; 
  oa << i; 
} 