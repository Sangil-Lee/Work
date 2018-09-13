#include <boost/archive/text_oarchive.hpp> 
#include <boost/archive/text_iarchive.hpp> 
#include <iostream> 
#include <sstream> 

std::stringstream ss; 

void save() 
{ 
  boost::archive::text_oarchive oa(ss); 
  int i = 1; 
  oa << i; 
} 

void load() 
{ 
  boost::archive::text_iarchive ia(ss); 
  int i = 0; 
  ia >> i; 
  std::cout << i << std::endl; 
} 

int main() 
{ 
  save(); 
  load(); 
} 