#include <Eigen/Dense>
#include <unsupported/Eigen/FFT>
#include <complex>
#include <cmath>
#include <iostream>
#include <fstream>

unsigned const N = 1000;  // Sample Count
double const Fs  = 32;    // [Hz]
double const Ts  = 1./Fs; // [s] 
const double f0  = 5;     // [Hz]
const double f1  = 10;     // [Hz]

std::complex<double> f(std::complex<double> const & t){
    //return (std::sin(2*M_PI*f0*t));
	//
	//Add Amplitude
    //return (std::sin(2*M_PI*f0*t)*3.0);
	//
	//Add Other wave
    return std::sin(2*M_PI*f0*t)*3.0 + std::sin(2*M_PI*f1*t)*5.0;
}

int main(){
    std::ofstream xrec("xrec.txt");
    Eigen::VectorXcd time(N);
    Eigen::VectorXcd f_values(N);
    Eigen::VectorXd freq(N);
    for(int u = 0; u < N; ++u){
        time(u) = u * Ts;
        //f_values(u) = f(time(u)); // Y_Val need real scale
        f_values(u) = f(time(u)); // Count
        f_values(u) = f_values(u)/(0.5*N);
        freq(u) = Fs * u / double(N);
    }

    Eigen::FFT<double> fft;
    Eigen::VectorXcd f_freq(N);
    fft.fwd(f_freq, f_values);

    //for(int u = 0; u < N; ++u){
	//Remove Nyquist Freq.
    for(int u = 0; u < N/2; ++u){
        xrec << freq(u) << " " << std::abs(f_freq(u)) << "\n"; 
    }
}

//GNU Plot Script
//set key off
//set grid
//set output "figure.png"
//set xlabel "Frequency [Hz]"
//plot [-1:34] [-10:500] "xrec.txt" with impulses, "xrec.txt" with points pt 4
