#include <complex>
#include <vector>
#include <cmath>
#include <iostream>

template <typename T>
class FFT {
public:
    using Complex = std::complex<T>;

    static std::vector<Complex> fft(const std::vector<Complex>& input) {
        int n = input.size();
        if (n <= 1) return input;

        std::vector<Complex> even(n / 2);
        std::vector<Complex> odd(n / 2);
        for (int i = 0; i < n / 2; ++i) {
            even[i] = input[i * 2];
            odd[i] = input[i * 2 + 1];
        }

        auto fft_even = fft(even);
        auto fft_odd = fft(odd);

        std::vector<Complex> result(n);
        for (int k = 0; k < n / 2; ++k) {
            Complex t = std::polar(T(1.0), -2 * M_PI * k / n) * fft_odd[k];
            result[k] = fft_even[k] + t;
            result[k + n / 2] = fft_even[k] - t;
        }
        return result;
    }

    static std::vector<Complex> ifft(const std::vector<Complex>& input) {
        int n = input.size();
        std::vector<Complex> conjugated_input(n);
        for (int i = 0; i < n; ++i) {
            conjugated_input[i] = std::conj(input[i]);
        }

        auto result = fft(conjugated_input);
        for (int i = 0; i < n; ++i) {
            result[i] = std::conj(result[i]) / T(n);
        }
        return result;
    }
};

int main() {
    using Complex = std::complex<double>;
    std::vector<Complex> data = {1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0};

    auto fft_result = FFT<double>::fft(data);
    std::cout << "FFT result:" << std::endl;
    for (const auto& value : fft_result) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    auto ifft_result = FFT<double>::ifft(fft_result);
    std::cout << "IFFT result:" << std::endl;
    for (const auto& value : ifft_result) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    return 0;
}
