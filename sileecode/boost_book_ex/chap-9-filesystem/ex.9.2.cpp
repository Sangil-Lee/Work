#include <boost/filesystem.hpp> 

int main() 
{ 
  boost::filesystem::path p1("..."); 
  boost::filesystem::path p2("\\"); 
  boost::filesystem::path p3("@:"); 
} 