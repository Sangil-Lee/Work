#include <boost/intrusive/set.hpp> 
#include <string> 
#include <iostream> 

struct person 
{ 
  std::string name_; 
  int age_; 
  boost::intrusive::set_member_hook<> set_hook_; 

  person(std::string name, int age) : name_(name), age_(age) {} 

  bool operator<(const person &p) const 
  { 
    return age_ < p.age_; 
  } 
}; 

int main() 
{ 
  person p1("Anton", 30); 
  person p2("Boris", 33); 
  person p3("Carl", 28); 

  typedef boost::intrusive::member_hook<person, 
    boost::intrusive::set_member_hook<>, &person::set_hook_> 
    person_member_hook; 
  typedef boost::intrusive::set<person, person_member_hook> person_set; 
  person_set ps; 

  ps.insert(p1); 
  ps.insert(p2); 
  ps.insert(p3); 

  for (person_set::iterator it = ps.begin(); it != ps.end(); ++it) 
    std::cout << it->name_ << std::endl; 
} 