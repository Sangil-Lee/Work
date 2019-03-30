#include <windows.h> 
#include <boost/signals2.hpp> 
#include <iostream> 

boost::signals2::signal<void (int)> s; 
CRITICAL_SECTION cs; 

void print(int i) 
{ 
  EnterCriticalSection(&cs); 
  std::cout << i << std::endl; 
  LeaveCriticalSection(&cs); 
} 

DWORD WINAPI loop(LPVOID) 
{ 
  for (int i = 0; i < 100; ++i) 
    s(i); 
  return 0; 
} 

int main() 
{ 
  s.connect(print); 

  InitializeCriticalSection(&cs); 

  HANDLE threads[2]; 
  threads[0] = CreateThread(0, 0, loop, 0, 0, 0); 
  threads[1] = CreateThread(0, 0, loop, 0, 0, 0); 
  WaitForMultipleObjects(2, threads, TRUE, INFINITE); 

  DeleteCriticalSection(&cs); 
} 