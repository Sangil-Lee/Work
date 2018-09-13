#include <boost/filesystem/fstream.hpp> 
#include <iostream> 

int main() 
{ 
  boost::filesystem::path p("test.txt"); 
  boost::filesystem::ofstream ofs(p); 
  ofs << "Hello, world!" << std::endl; 
} 