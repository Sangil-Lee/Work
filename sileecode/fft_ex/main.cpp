#include <stdio.h>
#include <malloc.h>
#include <math.h> 
#include <string.h>
#include <complex>
#include <vector>
//#include "FFT.h"
#include <cmath>
#include <algorithm>
#include <iostream>

#define PI 3.14159265

using namespace std;

class FFT
{
    public:
        typedef std::complex<double> Complex;
        
        /* Initializes FFT. n must be a power of 2. */
        FFT(int n, bool inverse = false);
        /* Computes Discrete Fourier Transform of given buffer. */
        std::vector<Complex> transform(const std::vector<Complex>& buf);
        static double getIntensity(Complex c);
        static double getPhase(Complex c);
        
    private:
        int n, lgN;
        bool inverse;
        std::vector<Complex> omega;
        std::vector<Complex> result;
        
        void bitReverseCopy(const std::vector<Complex>& src,
                std::vector<Complex>& dest) const;
};



FFT::FFT(int n, bool inverse)
    : n(n), inverse(inverse), result(vector<Complex>(n))
{
    lgN = 0;
    for (int i = n; i > 1; i >>= 1)
        ++lgN;
    omega.resize(lgN);
    int m = 1;
    for (int s = 0; s < lgN; ++s)
    {
        m <<= 1;
        if (inverse)
            omega[s] = exp(Complex(0, 2.0 * PI / m));
        else
            omega[s] = exp(Complex(0, -2.0 * PI / m));
    }
}

std::vector<FFT::Complex> FFT::transform(const vector<Complex>& buf)
{
    bitReverseCopy(buf, result);
    int m = 1;
    for (int s = 0; s < lgN; ++s)
    {
        m <<= 1;
        for (int k = 0; k < n; k += m)
        {
            Complex current_omega = 1;
            for (int j = 0; j < (m >> 1); ++j)
            {
                Complex t = current_omega * result[k + j + (m >> 1)];
                Complex u = result[k + j];
                result[k + j] = u + t;
                result[k + j + (m >> 1)] = u - t;
                current_omega *= omega[s];
            }
        }
    }
    if (inverse == false)
        for (int i = 0; i < n; ++i)
            result[i] /= n;
    return result;
}

double FFT::getIntensity(Complex c)
{
    return abs(c);
}

double FFT::getPhase(Complex c)
{
    return arg(c);
}

void FFT::bitReverseCopy(const vector<Complex>& src, vector<Complex>& dest)
        const
{
    for (int i = 0; i < n; ++i)
    {
        int index = i, rev = 0;
        for (int j = 0; j < lgN; ++j)
        {
            rev = (rev << 1) | (index & 1);
            index >>= 1;
        }
        dest[rev] = src[i];
    }
}

void TestFFT(){
	printf("\n TestFFT \n");

	int length = 4;
	FFT fft(length );

	vector<complex<double>> src;
	vector<complex<double>> rightResult;

	src.push_back( complex<double> (1,10));
	src.push_back( complex<double> (2,20));
	src.push_back( complex<double> (3,30));
	src.push_back( complex<double> (4,40));

	rightResult.push_back( complex<double> (10,100));
	rightResult.push_back( complex<double> (-22,-18));
	rightResult.push_back( complex<double> (-2,-20));
	rightResult.push_back( complex<double> (18,-22));

	vector<complex<double>> result = fft.transform(src);
	for(int i=0; i<result.size(); i++) result[i]*=length; 

	for(int i=0; i<result.size(); i++){
		if( (float)result[i].imag() == (float)rightResult[i].imag()
			&&(float)result[i].real() == (float)rightResult[i].real()) printf("[OK] \n");
		else {
			printf("[ERROR] \n");
		}
		printf("rightResult %d real %f imag %f  \n",i,rightResult[i].real(),rightResult[i].imag());
		printf("result      %d real %f imag %f  \n",i,result[i].real(),result[i].imag());		
	}
}

int main(void) {	
	
	TestFFT();

	return 0;
}
