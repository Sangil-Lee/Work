#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>

class X
{
    public:
        void doWhat(int x) { std::cout <<"param: "<< x << std::endl;}
        bool IsOK() const {return true;}
};
class CallWhatIfOk
{
    public:
        CallWhatIfOk(int p, int p2): param(p), param2(p2) {}

        void operator()(X& x) const
        {   
			std::cout<<"param: " << param <<"," << param2 << std::endl;
			if (x.IsOK()) {
				x.doWhat(param);
			}
		}
    private:
        int param;
        int param2;
};

int main()
{
    std::vector<X>      myVec(1);

    std::for_each(  myVec.begin(), myVec.end(),
                    std::bind2nd(std::mem_fun_ref(&X::doWhat),4)
                 );


#if 1
    std::for_each(  myVec.begin(), myVec.end(),
                    CallWhatIfOk(4, 10)
                 );
#endif
}
