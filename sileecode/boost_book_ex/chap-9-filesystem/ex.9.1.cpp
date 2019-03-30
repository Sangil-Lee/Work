#include <boost/filesystem.hpp> 

int main() 
{ 
  boost::filesystem::path p1("C:\\"); 
  boost::filesystem::path p2("C:\\Windows"); 
  boost::filesystem::path p3(L"C:\\Boost C++ \u5E93"); 
} 