#include <boost/interprocess/managed_shared_memory.hpp> 
#include <boost/interprocess/sync/named_mutex.hpp> 
#include <iostream> 

int main() 
{ 
  boost::interprocess::managed_shared_memory managed_shm(
    boost::interprocess::open_or_create, "shm", 1024); 
  int *i = managed_shm.find_or_construct<int>("Integer")(); 
  boost::interprocess::named_mutex named_mtx(
    boost::interprocess::open_or_create, "mtx"); 
  named_mtx.lock(); 
  ++(*i); 
  std::cout << *i << std::endl; 
  named_mtx.unlock(); 
} 