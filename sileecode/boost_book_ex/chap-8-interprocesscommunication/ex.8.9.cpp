#include <boost/interprocess/managed_shared_memory.hpp> 
#include <iostream> 

int main() 
{ 
  boost::interprocess::shared_memory_object::remove("Highscore"); 
  boost::interprocess::managed_shared_memory managed_shm(
    boost::interprocess::open_or_create, "Highscore", 1024); 
  int *i = managed_shm.find_or_construct<int>("Integer")(99); 
  std::cout << *i << std::endl; 
  managed_shm.destroy<int>("Integer"); 
  std::pair<int*, std::size_t> p = managed_shm.find<int>("Integer"); 
  std::cout << p.first << std::endl; 
} 