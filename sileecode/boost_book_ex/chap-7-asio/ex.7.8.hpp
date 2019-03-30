#include <boost/asio.hpp> 
#include <boost/thread.hpp> 
#include <boost/bind.hpp> 
#include <boost/scoped_ptr.hpp> 
#include <boost/shared_ptr.hpp> 
#include <boost/weak_ptr.hpp> 
#include <boost/system/error_code.hpp> 

template <typename TimerImplementation = timer_impl> 
class basic_timer_service 
  : public boost::asio::io_service::service 
{ 
  public: 
    static boost::asio::io_service::id id; 

    explicit basic_timer_service(boost::asio::io_service &io_service) 
      : boost::asio::io_service::service(io_service), 
      async_work_(new boost::asio::io_service::work(async_io_service_)), 
      async_thread_(boost::bind(&boost::asio::io_service::run, 
        &async_io_service_)) 
    { 
    } 

    typedef boost::shared_ptr<TimerImplementation> implementation_type; 

    void construct(implementation_type &impl) 
    { 
      impl.reset(new TimerImplementation()); 
    } 

    void destroy(implementation_type &impl) 
    { 
      impl->destroy(); 
      impl.reset(); 
    } 

    void wait(implementation_type &impl, std::size_t seconds) 
    { 
      boost::system::error_code ec; 
      impl->wait(seconds, ec); 
      boost::asio::detail::throw_error(ec); 
    } 

    template <typename Handler> 
    class wait_operation 
    { 
      public: 
        wait_operation(implementation_type &impl, 
          boost::asio::io_service &io_service, std::size_t seconds, 
          Handler handler) 
          : impl_(impl), 
          io_service_(io_service), 
          work_(io_service), 
          seconds_(seconds), 
          handler_(handler) 
        { 
        } 

        void operator()() const 
        { 
          implementation_type impl = impl_.lock(); 
          if (impl) 
          { 
              boost::system::error_code ec; 
              impl->wait(seconds_, ec); 
              this->io_service_.post(boost::asio::detail::bind_handler(
                handler_, ec)); 
          } 
          else 
          { 
              this->io_service_.post(boost::asio::detail::bind_handler(
                handler_, boost::asio::error::operation_aborted)); 
          } 
      } 

      private: 
        boost::weak_ptr<TimerImplementation> impl_; 
        boost::asio::io_service &io_service_; 
        boost::asio::io_service::work work_; 
        std::size_t seconds_; 
        Handler handler_; 
    }; 

    template <typename Handler> 
    void async_wait(implementation_type &impl, std::size_t seconds, 
      Handler handler) 
    { 
      this->async_io_service_.post(wait_operation<Handler>(impl, 
        this->get_io_service(), seconds, handler)); 
    } 

  private: 
    void shutdown_service() 
    { 
      async_work_.reset(); 
      async_io_service_.stop(); 
      async_thread_.join(); 
    } 

    boost::asio::io_service async_io_service_; 
    boost::scoped_ptr<boost::asio::io_service::work> async_work_; 
    boost::thread async_thread_; 
}; 

template <typename TimerImplementation> 
boost::asio::io_service::id basic_timer_service<TimerImplementation>::id; 