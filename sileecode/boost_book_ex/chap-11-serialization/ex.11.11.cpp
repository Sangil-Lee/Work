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
  person(int age) : age_(age) {} 
  int age() const { return age_; } 

private: 
  friend class boost::serialization::access; 

  template <typename Archive> 
  void serialize(Archive &ar, const unsigned int version) 
  { 
    ar & age_; 
  } 

  int age_; 
}; 

class developer : public person 
{ 
public: 
  developer() {} 
  developer(int age, const std::string &language) 
    : person(age), language_(language) {} 
  std::string language() const { return language_; } 

private: 
  friend class boost::serialization::access; 

  template <typename Archive> 
  void serialize(Archive &ar, const unsigned int version) 
  { 
    ar & boost::serialization::base_object<person>(*this); 
    ar & language_; 
  } 

  std::string language_; 
}; 

void save() 
{ 
  boost::archive::text_oarchive oa(ss); 
  developer d(31, "C++"); 
  oa << d; 
} 

void load() 
{ 
  boost::archive::text_iarchive ia(ss); 
  developer d; 
  ia >> d; 
  std::cout << d.age() << std::endl; 
  std::cout << d.language() << std::endl; 
} 

int main() 
{ 
  save(); 
  load(); 
} 