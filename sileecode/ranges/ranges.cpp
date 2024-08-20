#include <iostream>
#include <vector>

using namespace std;

template <typename T> class Range {
public:
    T start;
    T last;
    T step;

public:
    Range(T start, T last, T step = 1) : start(start), last(last), step(step) {}

    class Iterator {
    private:
        T current;
        T step;

    public:
        Iterator(T start, T step) : current(start), step(step) {}

        T operator*() const {
            return current;
        }

        const Iterator& operator++() {
            current += step;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return current < other.current;
        }
    };

    Iterator begin() const {
        return Iterator(start, step);
    }

    Iterator end() const {
        return Iterator(last, step);
    }
};

int main() {
    Range<float> range(0, 10, 0.2);

    for (auto i : range) {
        cout << i << " ";
    }
    cout << endl;

    for (auto i : Range<uint32_t>(0, 0xFF)) {
        cout << hex << i << " ";
    }
    cout << endl;

    return 0;
}

