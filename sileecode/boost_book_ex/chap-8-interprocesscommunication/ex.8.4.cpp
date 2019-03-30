#include <boost/interprocess/shared_memory_object.hpp> 
#include <iostream> 

int main() 
{ 
  bool removed = boost::interprocess::shared_memory_object::remove(
    "Highscore"); 
  std::cout << removed << std::endl; 
} 