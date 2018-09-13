#include <boost/interprocess/managed_shared_memory.hpp> 
#include <iostream> 

int main() 
{ 
  boost::interprocess::shared_memory_object::remove("Highscore"); 
  boost::interprocess::managed_shared_memory managed_shm(
    boost::interprocess::open_or_create, "Highscore", 1024); 
  int *i = managed_shm.construct<int>("Integer")(99); 
  std::cout << *i << std::endl; 
  std::pair<int*, std::size_t> p = managed_shm.find<int>("Integer"); 
  if (p.first) 
    std::cout << *p.first << std::endl; 
} 