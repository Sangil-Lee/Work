#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <list>
#include <mutex>
#include <thread>

using namespace std;

// Timer
// =====

template <typename Result>
class Timer
{
    public:
    typedef std::chrono::milliseconds interval_type;
    typedef Result result_type;
    typedef std::list<result_type> results_type;

    private:
    typedef std::function<Result ()> function_type;
    typedef std::future<result_type> future_type;
    typedef std::mutex mutex;

    public:
    template <typename Callable, typename... Arguments>
    Timer(Callable&& callable, Arguments&&... arguments)
    :   m_interval(interval_type::zero()),
        m_function(std::bind(
            std::move(callable),
            std::forward<Arguments>(arguments)...))
    {}

    Timer(const Timer&) = delete;
    Timer& operator = (const Timer&) = delete;

    ~Timer() {  stop(); }

    bool running() const { return m_interval != interval_type::zero(); }

    template <typename Interval>
    void start(const Interval& interval, Interval delay = Interval::zero()) {
        m_interval = std::chrono::duration_cast<interval_type>(interval);
        m_thread = std::thread([this, delay]() {
            std::this_thread::sleep_for(delay);
            if( ! this->running()) this->m_results.push_back(this->m_function());
            else {
                m_future = std::async(std::launch::async, this->m_function);
                while(true) {
                    std::this_thread::sleep_for(this->m_interval);
                    if(this->running()) {
                        if(this->m_future.wait_for(interval_type::zero()) == std::future_status::ready) {
                            try {
                                std::lock_guard<mutex> guard(this->m_result_mutex);
                                this->m_results.push_back(this->m_future.get());
                            }
                            catch(const std::exception &) {}
                            m_future = std::async(std::launch::async, this->m_function);
                        }
                    }
                    else {
                        this->m_future.wait();
                        try {
                            std::lock_guard<mutex> guard(this->m_result_mutex);
                            this->m_results.push_back(this->m_future.get());
                        }
                        catch(const std::exception &) {}
                        break;
                    }
                }
            }
        });
    }

    void stop() {
        m_interval = interval_type::zero();
        if(m_thread.joinable())
            m_thread.join();
    }

    // Results
    // =======

    bool empty() const { return m_results.empty(); }

    results_type results() const {
        results_type results;
        {
            std::lock_guard<mutex> guard(m_result_mutex);
            results.swap(m_results);
        }
        return results;
    }

    result_type result() {
        std::lock_guard<mutex> guard(m_result_mutex);
        result_type result = m_results.front();
        m_results.pop_front();
        return result;
    }

    private:
    interval_type m_interval;
    function_type m_function;
    std::thread m_thread;
    future_type m_future;

    mutable mutex m_result_mutex;
    mutable results_type m_results;
};

template <>
class Timer<void>
{
    // Implementation details for the simpler case omitted.
};


// Test
// ====
template <typename result=std::chrono::system_clock>
class CallBack{
public:
    using clock_type = std::chrono::system_clock;
    std::chrono::time_point<clock_type> prev;
    result operator()(){
        auto now =  clock_type::now();
        std::cout << " diff " <<  std::chrono::duration_cast<std::chrono::milliseconds>(now - prev).count() << "ms\n" ;
        prev = now;
        return(minc++);
    }
    CallBack(int count):minc(count)
    {
	std::cout << "Count: " << minc << std::endl;
    }
private:
    int minc;
};

char f() {
    return '.';
}

int test(int& cnt)
{
	return(cnt++);
}

int main()
{
    using std::chrono::milliseconds;

//    Timer<char> timer(f);
    int cnt = 10;
    //Timer<int> timer(test);
    //Timer<int> timer(test, std::ref(cnt) );

    Timer<int> timer(CallBack<int>{cnt} );
    timer.start(milliseconds(1000));
    for(unsigned i = 0; i <= 10; ++i) {
        std::this_thread::sleep_for(milliseconds(1000));
//        std::this_thread::sleep_for(milliseconds(100));
        if(i == 10) timer.stop();
        auto results = timer.results();
        for(const auto& r : results)
            std::cout << r << ",";
        std::cout << '\n';
    }
}
