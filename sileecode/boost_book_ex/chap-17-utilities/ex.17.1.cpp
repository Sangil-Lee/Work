#include <boost/checked_delete.hpp> 
#include <boost/intrusive/list.hpp> 
#include <string> 
#include <iostream> 

struct person : 
  public boost::intrusive::list_base_hook<> 
{ 
  std::string name_; 
  int age_; 

  person(std::string name, int age) : name_(name), age_(age) {} 
}; 

int main() 
{ 
  person *p = new person("Anton", 30); 

  typedef boost::intrusive::list<person> person_list; 
  person_list pl; 

  pl.push_back(*p); 

  pl.pop_back_and_dispose(boost::checked_delete<person>); 
  std::cout << pl.size() << std::endl; 
} 