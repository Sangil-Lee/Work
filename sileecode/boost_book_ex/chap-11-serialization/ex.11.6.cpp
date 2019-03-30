#include <boost/archive/text_oarchive.hpp> 
#include <boost/archive/text_iarchive.hpp> 
#include <boost/serialization/string.hpp> 
#include <iostream> 
#include <sstream> 
#include <string> 

std::stringstream ss; 

class person 
{ 
public: 
  person() {} 
  person(int age, const std::string &name) : age_(age), name_(name) {} 
  int age() const { return age_; } 
  std::string name() const { return name_; } 

private: 
  friend class boost::serialization::access; 

  template <typename Archive> 
  friend void serialize(Archive &ar, person &p, const unsigned int version); 

  int age_; 
  std::string name_; 
}; 

template <typename Archive> 
void serialize(Archive &ar, person &p, const unsigned int version) 
{ 
  ar & p.age_; 
  ar & p.name_; 
} 

void save() 
{ 
  boost::archive::text_oarchive oa(ss); 
  person p(31, "Boris"); 
  oa << p; 
} 

void load() 
{ 
  boost::archive::text_iarchive ia(ss); 
  person p; 
  ia >> p; 
  std::cout << p.age() << std::endl; 
  std::cout << p.name() << std::endl; 
} 

int main() 
{ 
  save(); 
  load(); 
} 