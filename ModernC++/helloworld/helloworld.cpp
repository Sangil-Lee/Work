#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

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

template <typename T>
void fill(vector<int> &v, T done)
{
    int i = 0;
    while (!done())
        v.push_back(i++);
};

class gorp {
    vector<int> values;
    int m_;
public:
    gorp(int mod):m_(mod){};
    gorp& put(int v){
        values.push_back(v);
        return *this;
    };

    int extras(){
        int count = 0;
        for_each(values.begin(), values.end(), [=, &count](int v) { count += (v%m_); } );
        return count;
    };
};

struct foo {
    foo():i(0) {}
    void amazing() {
        [=] { i = 8;}();
    };
public:
    int i;
};

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

#if 0
    []() { cout << "Lambda Hello World!! " << endl; };
#else
    []() { cout << "Lambda Hello World!! " << endl; }();
#endif

    [](int a) { cout << a << "*6 = " << a*6 << endl; }(7);

    int i = 7;
    [i](int &v) { v *= 6;}(i);
    cout << i << endl;

    totalelement = 1;

    vector <int> vec1;
    vec1.push_back(1);
    vec1.push_back(2);
    vec1.push_back(4);
    vec1.push_back(8);

    for_each(vec1.begin(), vec1.end(), [&totalelement](int i) { totalelement *= i;});
    cout << "totalelement: " << totalelement <<endl;

    vector<int> stuff;
#if 0
    fill(stuff, [&stuff]()->bool { return stuff.size() >= 8;} );
#else
    fill(stuff, [&stuff]()->bool { 
        int sum = 0;
        for_each(stuff.begin(), stuff.end(), [&](int i)->int { return sum += i;});
        return (sum >= 10);
    });
#endif

    for_each(stuff.begin(), stuff.end(), [](int i) {cout << i << ","; }); cout << endl;


    int v = 42;
    auto func = [=]() { cout << v << endl;};
    v = 8;
    func();

    int a = 10;
#if 0
    auto two_i = [=]() -> int{ a *= 2; return i;};
#else
    auto two_i = [=]() mutable -> int { a *=2; return a;};
#endif

    cout << "2i: " << two_i() << ", i: " << a << endl;

    gorp g(4);
    g.put(3).put(7).put(8).put(2);

    cout << "Extras: " << g.extras() << endl;

    foo f;
    f.amazing();
    cout << "f.i = " << f.i << endl;


//Capture Scope
    int ii = 8;
    {
        int jj = 2;
        auto f = [=]{cout << ii /jj << endl;};
        f();
    }

    auto f1 = [ii]() { 
        int jj = 2;
        auto m = [=] {cout << ii/jj << endl;};
        m();
    };
    f1();

#if 0
    // Compile error, for variable scope
    int aa = 8;
    auto f2 = []() {
        int bb = 2;
        auto m = []() {cout << aa / bb << endl;};
        m();
    };
    f2();
#else

    int aa = 8;
    auto f2 = [aa]() mutable {
        int bb = 2;
        auto m2 = [&, bb]() mutable { aa /= bb;};
        m2();
        cout << "inner ->" << aa << endl;
    };

    f2();
    cout << "outer ->" << aa << endl;
#endif

    typedef int (*f_type)(int);
    f_type ff = [](int i) -> int {return i +20;};

    cout << ff(8) << endl;

    std::function <int(std::string const &)> fc;

    fc = [](std::string const &s) -> int { cout << s << endl; return s.size();};

    int size = fc("http://itguru.tstory.com");
    cout << size << endl;

    std::function<int(int)> f3;
    std::function<int(int)> f4 = [&](int i) -> int {
        cout << i << " ";
        if (i>5) {
            return f3(i-2);
        }
    };

    f3 = [&](int i) -> int {
       cout << i << " "; 
       return f4(++i);
    };
    f3(10);
    cout << endl;

    std::function<int(int)> fact;
    fact =[&fact](int n)->int {
        if(n == 0)
            return 1;
        else return (n * fact(n-1));
    };

    cout << "Factorial: " << fact(10) << endl;
}
