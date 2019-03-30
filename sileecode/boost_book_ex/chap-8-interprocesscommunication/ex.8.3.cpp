#include <boost/interprocess/shared_memory_object.hpp> 
#include <boost/interprocess/mapped_region.hpp> 
#include <iostream> 

int main() 
{ 
  boost::interprocess::shared_memory_object shdmem(
    boost::interprocess::open_or_create, "Highscore", 
    boost::interprocess::read_write); 
  shdmem.truncate(1024); 
  boost::interprocess::mapped_region region(shdmem, 
    boost::interprocess::read_write); 
  int *i1 = static_cast<int*>(region.get_address()); 
  *i1 = 99; 
  boost::interprocess::mapped_region region2(shdmem, 
    boost::interprocess::read_only); 
  int *i2 = static_cast<int*>(region2.get_address()); 
  std::cout << *i2 << std::endl; 
} 