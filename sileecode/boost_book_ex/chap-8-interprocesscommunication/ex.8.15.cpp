#include <boost/interprocess/managed_shared_memory.hpp> 
#include <boost/interprocess/sync/interprocess_mutex.hpp> 
#include <boost/interprocess/sync/interprocess_condition.hpp> 
#include <boost/interprocess/sync/scoped_lock.hpp> 
#include <iostream> 

int main() 
{ 
  try 
  { 
    boost::interprocess::managed_shared_memory managed_shm(
      boost::interprocess::open_or_create, "shm", 1024); 
    int *i = managed_shm.find_or_construct<int>("Integer")(0); 
    boost::interprocess::interprocess_mutex *mtx = managed_shm
      .find_or_construct<boost::interprocess::interprocess_mutex>("mtx")(); 
    boost::interprocess::interprocess_condition *cnd = managed_shm
      .find_or_construct<boost::interprocess::interprocess_condition>(
      "cnd")(); 
    boost::interprocess::scoped_lock<
      boost::interprocess::interprocess_mutex> lock(*mtx); 
    while (*i < 10) 
    { 
      if (*i % 2 == 0) 
      { 
        ++(*i); 
        cnd->notify_all(); 
        cnd->wait(lock); 
      } 
      else 
      { 
        std::cout << *i << std::endl; 
        ++(*i); 
        cnd->notify_all(); 
        cnd->wait(lock); 
      } 
    } 
    cnd->notify_all(); 
  } 
  catch (...) 
  { 
  } 
  boost::interprocess::shared_memory_object::remove("shm"); 
} 