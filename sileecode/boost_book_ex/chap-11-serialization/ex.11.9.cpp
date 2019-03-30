#include <boost/archive/text_oarchive.hpp> 
#include <boost/archive/text_iarchive.hpp> 
#include <boost/serialization/scoped_ptr.hpp> 
#include <boost/scoped_ptr.hpp> 
#include <iostream> 
#include <sstream> 

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

void save() 
{ 
  boost::archive::text_oarchive oa(ss); 
  boost::scoped_ptr<person> p(new person(31)); 
  oa << p; 
} 

void load() 
{ 
  boost::archive::text_iarchive ia(ss); 
  boost::scoped_ptr<person> p; 
  ia >> p; 
  std::cout << p->age() << std::endl; 
} 

int main() 
{ 
  save(); 
  load(); 
} 