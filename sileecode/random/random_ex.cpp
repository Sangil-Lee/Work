#include <random>
#include <iostream>

template <typename T>
class RandomNumberGenerator {
public:
    RandomNumberGenerator(T min, T max) : dist_(min, max) {
        std::random_device rd;
        gen_ = std::mt19937(rd());
    }

    T generate() {
        return dist_(gen_);
    }

private:
    std::mt19937 gen_;
    std::uniform_real_distribution<T> dist_;
};

int main() {
    // RandomNumberGenerator 테스트 코드
    RandomNumberGenerator<double> rng(0.0, 10.0);
    std::cout << "Random numbers generated:" << std::endl;
    for (int i = 0; i < 10; ++i) {
        std::cout << rng.generate() << " ";
    }
    std::cout << std::endl;

    return 0;
}

