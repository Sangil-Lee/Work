// C++ program to use std::promise and std::future to 
// communicate between threads. 
#include <future> 
#include <iostream> 
#include <stdexcept> 
#include <thread> 
  
using namespace std; 
  
// Function to perform some computation and set the result 
// in a promise 
void RetrieveValue(promise<int>& result) 
{ 
    try { 
        int ans = 21095022; 
  
        // Set the result in the promise 
        result.set_value(ans); 
    } 
    catch (...) { 
        // if an error occurs set the exception 
        result.set_exception(current_exception()); 
    } 
} 
  
int main() 
{ 
    // Step 1: Creating a std::promise object 
    promise<int> myPromise; 
  
    // Step 2: Associate a std::future with the promise 
    future<int> myFuture = myPromise.get_future(); 
  
    // Step 3: Launching a thread to perform computation and 
    // set the result in the promise 
    thread computationThread(RetrieveValue, ref(myPromise)); 
  
    // Step 4: Retrieve the value or handle the exception in 
    // the original thread 
    try { 
        int result = myFuture.get(); 
        cout << "Result: " << result << endl; 
    } 
    catch (const exception& e) { 
        cerr << "Exception is: " << e.what() << endl; 
    } 
  
    // thread finishes 
    computationThread.join(); 
  
    return 0; 
}
