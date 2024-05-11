// C++ program to illustrate the use of shared_mutex 
#include <iostream> 
#include <shared_mutex> 
#include <mutex> 
#include <thread> 
using namespace std; 
  
// creating a shared_mutex object 
shared_mutex mutx; 
int shared_data = 11; 
  
// callable with shared lock 
void readData() { 
    shared_lock<shared_mutex> lock(mutx); 
    cout << "Thread " << this_thread::get_id() << ": ";  
    cout << shared_data << endl; 
} 
  
// callable with unique_lock 
void writeData(int n) { 
    unique_lock<shared_mutex> lock(mutx); 
    shared_data = n; 
    cout << "Thread" << this_thread::get_id() << ": \n"; 
} 
  
// driver code 
int main() 
{ 
    thread t1(readData); 
    thread t2(writeData, 128); 
    thread t3(writeData, 10); 
    thread t4 (readData); 
      
    t1.join(); 
    t2.join(); 
    t3.join(); 
    t4.join(); 
    return 0; 
}
