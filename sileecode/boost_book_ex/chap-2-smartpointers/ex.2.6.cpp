#include <boost/shared_ptr.hpp> 
#include <windows.h> 

int main() 
{ 
  boost::shared_ptr<void> h(OpenProcess(PROCESS_SET_INFORMATION, FALSE, 
    GetCurrentProcessId()), CloseHandle); 
  SetPriorityClass(h.get(), HIGH_PRIORITY_CLASS); 
} 