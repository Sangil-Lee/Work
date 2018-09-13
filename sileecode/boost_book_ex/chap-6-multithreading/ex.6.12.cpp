#include <iostream> 

int sum = 0; 

void calculate() 
{ 
  for (int i = 0; i < 1000; ++i) 
    sum += i; 
} 

void print() 
{ 
  std::cout << sum << std::endl; 
} 

void thread() 
{ 
  calculate(); 
  print(); 
} 

int main() 
{ 
  thread(); 
} 