#include <windows.h> 

class windows_handle 
{ 
  public: 
    windows_handle(HANDLE h) : handle_(h) {} 

    ~windows_handle() 
    { 
      CloseHandle(handle_); 
    } 

    HANDLE handle() const 
    { 
      return handle_; 
    } 

  private: 
    HANDLE handle_; 
}; 

int main() 
{ 
  windows_handle h(OpenProcess(PROCESS_SET_INFORMATION, FALSE, 
    GetCurrentProcessId())); 
  SetPriorityClass(h.handle(), HIGH_PRIORITY_CLASS); 
} 