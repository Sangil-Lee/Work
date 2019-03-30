#include <boost/system/error_code.hpp> 
#include <cstddef> 
#include <windows.h> 

class timer_impl 
{ 
  public: 
    timer_impl() 
      : handle_(CreateEvent(NULL, FALSE, FALSE, NULL)) 
    { 
    } 

    ~timer_impl() 
    { 
      CloseHandle(handle_); 
    } 

    void destroy() 
    { 
      SetEvent(handle_); 
    } 

    void wait(std::size_t seconds, boost::system::error_code &ec) 
    { 
      DWORD res = WaitForSingleObject(handle_, seconds * 1000); 
      if (res == WAIT_OBJECT_0) 
        ec = boost::asio::error::operation_aborted; 
      else 
        ec = boost::system::error_code(); 
    } 

private: 
    HANDLE handle_; 
}; 