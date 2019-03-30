#include <boost/operators.hpp> 
#include <iostream> 

struct person : 
  public boost::less_than_comparable<person> 
{ 
  std::string name_; 
  int age_; 

  person(std::string name, int age) : name_(name), age_(age) {} 

  bool operator<(const person &rhs) const 
  { 
    return age_ < rhs.age_; 
  } 
}; 

int main() 
{ 
  person p1("Anton", 30); 
  person p2("Boris", 33); 

  std::cout << (p2 > p1) << std::endl; 
} 