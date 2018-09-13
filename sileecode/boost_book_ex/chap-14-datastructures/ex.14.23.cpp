#include <boost/dynamic_bitset.hpp> 
#include <iostream> 

int main() 
{ 
  boost::dynamic_bitset<> db(3, 4); 

  db.push_back(true); 

  std::cout << db.size() << std::endl; 
  std::cout << db.count() << std::endl; 
  std::cout << db.any() << std::endl; 
  std::cout << db.none() << std::endl; 

  std::cout << db[0].flip() << std::endl; 
  std::cout << ~db[3] << std::endl; 
  std::cout << db << std::endl; 
} 