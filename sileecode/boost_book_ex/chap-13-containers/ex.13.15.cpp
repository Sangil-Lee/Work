#include <boost/bimap.hpp> 
#include <boost/bimap/unconstrained_set_of.hpp> 
#include <boost/bimap/support/lambda.hpp> 
#include <iostream> 
#include <string> 

int main() 
{ 
  typedef boost::bimap<std::string, 
    boost::bimaps::unconstrained_set_of<int> > bimap; 
  bimap persons; 

  persons.insert(bimap::value_type("Boris", 31)); 
  persons.insert(bimap::value_type("Anton", 31)); 
  persons.insert(bimap::value_type("Caesar", 25)); 

  bimap::left_map::iterator it = persons.left.find("Boris"); 
  persons.left.modify_key(it, boost::bimaps::_key = "Doris"); 

  std::cout << it->first << std::endl; 
} 