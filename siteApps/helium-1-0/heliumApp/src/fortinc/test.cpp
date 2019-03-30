#include <iostream>

using namespace std;

extern "C" {
float fortfunc_multiply_(int *ii, float *ff);
}

int main(void)
{
		int ii=5;
		float ff=5.5;

		float a = fortfunc_multiply_(&ii, &ff);
		cout <<"Result:" << a << endl;
		return 0;
}
