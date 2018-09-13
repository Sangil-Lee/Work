#include <boost/intrusive/list.hpp> 
#include <string> 
#include <iostream> 

typedef boost::intrusive::link_mode<boost::intrusive::auto_unlink> 
  auto_unlink; 

struct person : 
  public boost::intrusive::list_base_hook<auto_unlink> 
{ 
  std::string name_; 
  int age_; 

  person(std::string name, int age) : name_(name), age_(age) {} 
}; 

int main() 
{ 
  person p1("Anton", 30); 
  person p2("Boris", 33); 
  person *p3 = new person("Carl", 28); 

  typedef boost::intrusive::constant_time_size<false> constant_time_size; 
  typedef boost::intrusive::list<person, constant_time_size> person_list; 
  person_list pl; 

  pl.push_back(p1); 
  pl.push_back(p2); 
  pl.push_back(*p3); 

  delete p3; 

  for (person_list::iterator it = pl.begin(); it != pl.end(); ++it) 
    std::cout << it->name_ << std::endl; 
} 