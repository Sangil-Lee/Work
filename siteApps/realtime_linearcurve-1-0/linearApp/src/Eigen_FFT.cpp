#include <unsupported/Eigen/FFT>

using namespace Eigen;
using namespace std;

#if 0
int main()
{

	FFT<float> fft;

	vector<float> timevec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	vector<complex<float> > freqvec;

	fft.fwd( freqvec, timevec);
	fft.inv( timevec, freqvec);


	return 0;
}
#else

#define EIGEN_FFTW_DEFAULT
#include <iostream>
#include <unsupported/Eigen/FFT>

int main(int argc, char *argv[])
{
    Eigen::MatrixXf A(3,3);
    A << 2,1,2,  3,2,1,  1,2,3;
    const int nRows = A.rows();
    const int nCols = A.cols();

    std::cout << A << "\n\n";

    Eigen::MatrixXcf B(3,3);

    Eigen::FFT< float > fft;

    for (int k = 0; k < nRows; ++k) {
        Eigen::VectorXcf tmpOut(nRows);
        fft.fwd(tmpOut, A.row(k));
        B.row(k) = tmpOut;
    }
    std::cout << B << "\n\n";
    Eigen::FFT< float > fft2;  // Workaround: Using the same FFT object for a real and a complex FFT seems not to work with FFTW
    for (int k = 0; k < nCols; ++k) {
        Eigen::VectorXcf tmpOut(nCols);
        fft2.fwd(tmpOut, B.col(k));
        B.col(k) = tmpOut;
    }
    std::cout << B << '\n';

	return 0;
}

#endif
