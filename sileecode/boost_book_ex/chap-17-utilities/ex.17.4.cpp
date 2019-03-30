#include <boost/noncopyable.hpp> 
#include <string> 
#include <iostream> 

struct person : 
  boost::noncopyable 
{ 
  std::string name_; 
  int age_; 

  person(std::string name, int age) : name_(name), age_(age) {} 
}; 

void print(person &p) 
{ 
  std::cout << p.name_ << std::endl; 
  std::cout << p.age_ << std::endl; 
} 

int main() 
{ 
  person p("Boris", 33); 
  print(p); 
} 