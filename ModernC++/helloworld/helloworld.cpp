#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#if 1
template <typename T> struct product{
    product(T& storage) : value(storage){}
    template <typename V>
    void operator()(V &v)
    {
        value *= v;
    };
    T& value;
};
template <typename T> struct sum{
    sum(T& storage) : value(storage){}
    template <typename V>
    void operator()(V &v)
    {
        value += v;
    };
    T& value;
};

struct mod {

    mod(int m): modulas(m) {};
    int operator()(int v) { return (v%modulas); };
    int modulas;
};
#endif

int main()
{
    vector<string> msg {"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!"};
    
    //for (const string& word : msg)
    for (auto word : msg)
    {
        cout << word << " ";
    }
    cout << endl;

    vector <int> cardinal {1,2,3,4,5,6,7,8,9,10};
    vector <int> cardinal2 {0,0,0,0,0,0,0,0,0,0};
    vector <int> cardinal3 {0,0,0,0,0,0,0,0,0,0};

    int sum_ = 1;
    int product_ = 1;
    for_each(cardinal.begin(), cardinal.end(), sum<int>(sum_));
    for_each(cardinal.begin(), cardinal.end(), product<int>(product_));

    cout << "Total: Sum =  " << sum_ << " Product: "<< product_<< endl;

    int totalelement = 1;
    for_each(cardinal.begin(), cardinal.end(), [&totalelement](int i) {totalelement *=i;});

    cout << "Total: "<< totalelement<< endl;

    int modulas = 8;
    transform(cardinal.begin(), cardinal.end(), cardinal2.begin(), mod(modulas));

    for(auto data: cardinal2)
        cout << "Mod: " << data << ", ";
    
    cout << endl;

    transform(cardinal.begin(), cardinal.end(), cardinal3.begin(), [modulas](int v)-> int {return v%modulas;});

    for(auto data: cardinal3)
        cout << "Mod: " << data << ", ";
    
    cout << endl;
}
