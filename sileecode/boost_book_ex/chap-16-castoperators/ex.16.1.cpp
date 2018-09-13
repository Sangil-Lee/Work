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
  child *c = dynamic_cast<child*>(f); 
} 

int main() 
{ 
  child *c = new child; 
  func(c); 

  father *f = new child; 
  mother *m = dynamic_cast<mother*>(f); 
} 