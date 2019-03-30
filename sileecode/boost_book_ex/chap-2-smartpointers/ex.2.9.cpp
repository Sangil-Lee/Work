#include <windows.h> 
#include <boost/shared_ptr.hpp> 
#include <boost/weak_ptr.hpp> 
#include <iostream> 

DWORD WINAPI reset(LPVOID p) 
{ 
  boost::shared_ptr<int> *sh = static_cast<boost::shared_ptr<int>*>(p); 
  sh->reset(); 
  return 0; 
} 

DWORD WINAPI print(LPVOID p) 
{ 
  boost::weak_ptr<int> *w = static_cast<boost::weak_ptr<int>*>(p); 
  boost::shared_ptr<int> sh = w->lock(); 
  if (sh) 
    std::cout << *sh << std::endl; 
  return 0; 
} 

int main() 
{ 
  boost::shared_ptr<int> sh(new int(99)); 
  boost::weak_ptr<int> w(sh); 
  HANDLE threads[2]; 
  threads[0] = CreateThread(0, 0, reset, &sh, 0, 0); 
  threads[1] = CreateThread(0, 0, print, &w, 0, 0); 
  WaitForMultipleObjects(2, threads, TRUE, INFINITE); 
} 