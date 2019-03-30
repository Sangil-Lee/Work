#include <boost/asio.hpp> 
#include <cstddef> 

template <typename Service> 
class basic_timer 
  : public boost::asio::basic_io_object<Service> 
{ 
  public: 
    explicit basic_timer(boost::asio::io_service &io_service) 
      : boost::asio::basic_io_object<Service>(io_service) 
    { 
    } 

    void wait(std::size_t seconds) 
    { 
      return this->service.wait(this->implementation, seconds); 
    } 

    template <typename Handler> 
    void async_wait(std::size_t seconds, Handler handler) 
    { 
      this->service.async_wait(this->implementation, seconds, handler); 
    } 
}; 