#include <boost/cast.hpp> 

struct father 
{ 
  virtual ~father() {}; 
}; 

struct mother 
{ 
  virtual ~mother() {}; 
}; 

struct child : 
  public father, 
  public mother 
{ 
}; 

void func(father *f) 
{ 
  child *c = boost::polymorphic_downcast<child*>(f); 
} 

int main() 
{ 
  child *c = new child; 
  func(c); 

  father *f = new child; 
  mother *m = boost::polymorphic_cast<mother*>(f); 
} 