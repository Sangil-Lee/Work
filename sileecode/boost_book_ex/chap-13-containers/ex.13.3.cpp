#include <boost/unordered_set.hpp> 
#include <iostream> 
#include <string> 

int main() 
{ 
  typedef boost::unordered_set<std::string> unordered_set; 
  unordered_set set; 

  set.insert("Boris"); 
  set.insert("Anton"); 
  set.insert("Caesar"); 

  for (unordered_set::iterator it = set.begin(); it != set.end(); ++it) 
    std::cout << *it << std::endl; 

  std::cout << set.size() << std::endl; 
  std::cout << set.max_size() << std::endl; 

  std::cout << (set.find("David") != set.end()) << std::endl; 
  std::cout << set.count("Boris") << std::endl; 
} 