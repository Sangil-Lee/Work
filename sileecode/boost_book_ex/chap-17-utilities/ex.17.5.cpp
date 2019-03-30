#include <boost/utility/addressof.hpp> 
#include <string> 
#include <iostream> 

struct person 
{ 
  std::string name_; 
  int age_; 

  person(std::string name, int age) : name_(name), age_(age) {} 

  int operator&() const 
  { 
    return age_; 
  } 
}; 

int main() 
{ 
  person p("Boris", 33); 
  std::cout << &p << std::endl; 
  std::cout << boost::addressof(p) << std::endl; 
} 