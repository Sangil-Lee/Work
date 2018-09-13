#include <boost/interprocess/managed_shared_memory.hpp> 
#include <boost/bind.hpp> 
#include <iostream> 

void construct_objects(
  boost::interprocess::managed_shared_memory &managed_shm) 
{ 
  managed_shm.construct<int>("Integer")(99); 
  managed_shm.construct<float>("Float")(3.14); 
} 

int main() 
{ 
  boost::interprocess::shared_memory_object::remove("Highscore"); 
  boost::interprocess::managed_shared_memory managed_shm(
    boost::interprocess::open_or_create, "Highscore", 1024); 
  managed_shm.atomic_func(boost::bind(construct_objects, 
    boost::ref(managed_shm))); 
  std::cout << *managed_shm.find<int>("Integer").first << std::endl; 
  std::cout << *managed_shm.find<float>("Float").first << std::endl; 
} 