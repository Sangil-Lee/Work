// condition_variable::wait_for example
#include <iostream>           // std::cout
#include <thread>             // std::thread
#include <chrono>             // std::chrono::seconds
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable, std::cv_status
#include <functional>

std::condition_variable cv;

int value;

#if 0
void read_value() {
  std::cin >> value;
  cv.notify_one();
}
#else
void read_value(int value) {
  if (value < 100) return;
  cv.notify_one();
}
#endif

int main ()
{
  std::cout << "Please, enter an integer (I'll be printing dots): \n";
#if 0
  //std::thread th (read_value);
#else
#if 0
  std::thread th ([](int val) {
  			//std::cin >> value;
  			std::cout << "Cond Value: " << val << '\n';
			if (val <100) return;
			cv.notify_one();
		  }, 100);
#else
  //std::thread th(read_value, 100);
  std::thread th(read_value, 3);
#endif
#endif

  std::mutex mtx;
  std::unique_lock<std::mutex> lck(mtx);
  while (cv.wait_for(lck,std::chrono::seconds(1))==std::cv_status::timeout) {
    std::cout << '.' << std::endl;
  }
  std::cout << "You entered: " << value << '\n';
  th.join();


  return 0;
}
