#include <boost/multi_index_container.hpp> 
#include <boost/multi_index/sequenced_index.hpp> 
#include <boost/multi_index/ordered_index.hpp> 
#include <boost/multi_index/random_access_index.hpp> 
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
    boost::multi_index::sequenced<>, 
    boost::multi_index::ordered_non_unique< 
      boost::multi_index::member< 
        person, int, &person::age 
      > 
    >, 
    boost::multi_index::random_access<> 
  > 
> person_multi; 

int main() 
{ 
  person_multi persons; 

  persons.push_back(person("Boris", 31)); 
  persons.push_back(person("Anton", 31)); 
  persons.push_back(person("Caesar", 25)); 

  const person_multi::nth_index<1>::type &ordered_index = 
    persons.get<1>(); 
  person_multi::nth_index<1>::type::iterator lower = 
    ordered_index.lower_bound(30); 
  person_multi::nth_index<1>::type::iterator upper = 
    ordered_index.upper_bound(40); 
  for (; lower != upper; ++lower) 
    std::cout << lower->name << std::endl; 

  const person_multi::nth_index<2>::type &random_access_index = 
    persons.get<2>(); 
  std::cout << random_access_index[2].name << std::endl; 
} 