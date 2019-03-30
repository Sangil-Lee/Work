#include <boost/archive/text_oarchive.hpp> 
#include <boost/archive/text_iarchive.hpp> 
#include <iostream> 
#include <fstream> 

void save() 
{ 
  std::ofstream file("archiv.txt"); 
  boost::archive::text_oarchive oa(file); 
  int i = 1; 
  oa << i; 
} 

void load() 
{ 
  std::ifstream file("archiv.txt"); 
  boost::archive::text_iarchive ia(file); 
  int i = 0; 
  ia >> i; 
  std::cout << i << std::endl; 
} 

int main() 
{ 
  save(); 
  load(); 
} 