#include <boost/archive/text_oarchive.hpp> 
#include <boost/archive/text_iarchive.hpp> 
#include <boost/serialization/array.hpp> 
#include <boost/array.hpp> 
#include <iostream> 
#include <sstream> 

std::stringstream ss; 

void save() 
{ 
  boost::archive::text_oarchive oa(ss); 
  boost::array<int, 3> a = { 0, 1, 2 }; 
  oa << boost::serialization::make_array(a.data(), a.size()); 
} 

void load() 
{ 
  boost::archive::text_iarchive ia(ss); 
  boost::array<int, 3> a; 
  ia >> boost::serialization::make_array(a.data(), a.size()); 
  std::cout << a[0] << ", " << a[1] << ", " << a[2] << std::endl; 
} 

int main() 
{ 
  save(); 
  load(); 
} 