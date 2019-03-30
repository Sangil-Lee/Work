#include <boost/unordered_set.hpp> 
#include <string> 

struct person 
{ 
  std::string name; 
  int age; 

  person(const std::string &n, int a) 
    : name(n), age(a) {} 

  bool operator==(const person &p) const 
  { 
    return name == p.name && age == p.age; 
  } 
}; 

std::size_t hash_value(person const &p) 
{ 
  std::size_t seed = 0; 
  boost::hash_combine(seed, p.name); 
  boost::hash_combine(seed, p.age); 
  return seed; 
} 

int main() 
{ 
  typedef boost::unordered_set<person> unordered_set; 
  unordered_set set; 

  set.insert(person("Boris", 31)); 
  set.insert(person("Anton", 35)); 
  set.insert(person("Caesar", 25)); 
} 