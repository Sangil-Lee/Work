#include <boost/bimap.hpp> 
#include <boost/bimap/multiset_of.hpp> 
#include <iostream> 
#include <string> 

int main() 
{ 
  typedef boost::bimap<boost::bimaps::set_of<std::string>, 
    boost::bimaps::multiset_of<int> > bimap; 
  bimap persons; 

  persons.insert(bimap::value_type("Boris", 31)); 
  persons.insert(bimap::value_type("Anton", 31)); 
  persons.insert(bimap::value_type("Caesar", 25)); 

  std::cout << persons.left.count("Boris") << std::endl; 
  std::cout << persons.right.count(31) << std::endl; 
} 