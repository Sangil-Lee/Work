#include <boost/multi_index_container.hpp> 
#include <boost/multi_index/hashed_index.hpp> 
#include <boost/multi_index/member.hpp> 
#include <iostream> 
#include <string> 

struct person 
{ 
  std::string name; 
  int age; 

  person(const std::string &n, int a) : name(n), age(a) {} 
}; 

typedef boost::multi_index::multi_index_container< 
  person, 
  boost::multi_index::indexed_by< 
    boost::multi_index::hashed_non_unique< 
      boost::multi_index::member< 
        person, std::string, &person::name 
      > 
    >, 
    boost::multi_index::hashed_non_unique< 
      boost::multi_index::member< 
        person, int, &person::age 
      > 
    > 
  > 
> person_multi; 

void set_age(person &p) 
{ 
  p.age = 32; 
} 

int main() 
{ 
  person_multi persons; 

  persons.insert(person("Boris", 31)); 
  persons.insert(person("Anton", 35)); 
  persons.insert(person("Caesar", 25)); 

  person_multi::iterator it = persons.find("Boris"); 
  persons.modify(it, set_age); 

  const person_multi::nth_index<1>::type &age_index = persons.get<1>(); 
  std::cout << age_index.count(32) << std::endl; 
} 