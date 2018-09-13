#include <boost/unordered_map.hpp> 
#include <iostream> 
#include <string> 

int main() 
{ 
  typedef boost::unordered_map<std::string, int> unordered_map; 
  unordered_map map; 

  map.insert(unordered_map::value_type("Boris", 31)); 
  map.insert(unordered_map::value_type("Anton", 35)); 
  map.insert(unordered_map::value_type("Caesar", 25)); 

  for (unordered_map::iterator it = map.begin(); it != map.end(); ++it) 
    std::cout << it->first << ", " << it->second << std::endl; 

  std::cout << map.size() << std::endl; 
  std::cout << map.max_size() << std::endl; 

  std::cout << (map.find("David") != map.end()) << std::endl; 
  std::cout << map.count("Boris") << std::endl; 
} 