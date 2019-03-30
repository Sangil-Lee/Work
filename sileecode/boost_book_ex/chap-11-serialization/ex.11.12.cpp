#include <boost/archive/text_oarchive.hpp> 
#include <boost/archive/text_iarchive.hpp> 
#include <boost/serialization/string.hpp> 
#include <boost/serialization/export.hpp> 
#include <iostream> 
#include <sstream> 
#include <string> 

std::stringstream ss; 

class person 
{ 
public: 
  person() {} 
  person(int age) : age_(age) {} 
  virtual int age() const { return age_; } 

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

BOOST_CLASS_EXPORT(developer) 

void save() 
{ 
  boost::archive::text_oarchive oa(ss); 
  person *p = new developer(31, "C++"); 
  oa << p; 
  delete p; 
} 

void load() 
{ 
  boost::archive::text_iarchive ia(ss); 
  person *p; 
  ia >> p; 
  std::cout << p->age() << std::endl; 
  delete p; 
} 

int main() 
{ 
  save(); 
  load(); 
} 