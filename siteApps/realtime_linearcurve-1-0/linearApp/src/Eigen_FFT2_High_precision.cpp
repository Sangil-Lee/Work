#include <vector>
#include <cmath>
#include <complex>
#include <iostream>
#include <Eigen/Core>
#include <unsupported/Eigen/FFT>

#include <boost/multiprecision/cpp_dec_float.hpp>

using boost::multiprecision::cpp_dec_float_50;

using std::cout;
using std::endl;

void twoSinTest() {
  const double pi = 2.*std::acos(0.);
  cout <<"PI: " << pi << endl;
#if 0
  typedef double FFTFloat;
#else
  typedef cpp_dec_float_50 FFTFloat;
#endif
  double Fs = 1000;            // Sampling frequency
  double T = 1 / Fs;           // Sampling period
  int L = 1000;               // Length of signal
  std::vector<FFTFloat> timebuf(L);
  typedef std::complex<FFTFloat> C;
  std::vector<C> freqbuf;
  double t = 0;
  for (int i = 0; i < L; i++) {
    timebuf[i] = 0.7*std::sin(2 * pi * 50 * t) + std::sin(2 * pi * 120 * t);
    t += T;
  }
  Eigen::FFT<FFTFloat> fft;
  fft.fwd(freqbuf, timebuf);
  cout << freqbuf.size() << endl;
  for (int i = 0; i < freqbuf.size(); i++) {
    const C &f = freqbuf[i];
    cout << i << " " << f << endl;
  }
} 

int main()
{
	twoSinTest();
	return 0;
}
