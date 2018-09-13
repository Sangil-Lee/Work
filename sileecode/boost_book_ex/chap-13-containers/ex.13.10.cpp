#include <boost/multi_index_container.hpp> 
#include <boost/multi_index/ordered_index.hpp> 
#include <boost/multi_index/hashed_index.hpp> 
#include <boost/multi_index/identity.hpp> 
#include <boost/multi_index/mem_fun.hpp> 
#include <iostream> 
#include <string> 

class person 
{ 
public: 
  person(const std::string &n, int a) : name(n), age(a) {} 
  bool operator<(const person &p) const { return age < p.age; } 
  std::string get_name() const { return name; } 

private: 
  std::string name; 
  int age; 
}; 

typedef boost::multi_index::multi_index_container< 
  person, 
  boost::multi_index::indexed_by< 
    boost::multi_index::ordered_unique< 
      boost::multi_index::identity<person> 
    >, 
    boost::multi_index::hashed_unique< 
      boost::multi_index::const_mem_fun< 
        person, std::string, &person::get_name 
      > 
    > 
  > 
> person_multi; 

int main() 
{ 
  person_multi persons; 

  persons.insert(person("Boris", 31)); 
  persons.insert(person("Anton", 31)); 
  persons.insert(person("Caesar", 25)); 

  std::cout << persons.begin()->get_name() << std::endl; 

  const person_multi::nth_index<1>::type &hashed_index = persons.get<1>(); 
  std::cout << hashed_index.count("Boris") << std::endl; 
} 