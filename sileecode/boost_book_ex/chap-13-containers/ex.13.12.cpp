#include <boost/bimap.hpp> 
#include <iostream> 
#include <string> 

int main() 
{ 
  typedef boost::bimap<std::string, int> bimap; 
  bimap persons; 

  persons.insert(bimap::value_type("Boris", 31)); 
  persons.insert(bimap::value_type("Anton", 31)); 
  persons.insert(bimap::value_type("Caesar", 25)); 

  for (bimap::iterator it = persons.begin(); it != persons.end(); ++it) 
    std::cout << it->left << " is " << it->right << " years old." 
    << std::endl; 
} 