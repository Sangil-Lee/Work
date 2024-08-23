#include <iostream>
#include <stdexcept>

template<typename T>
class Range {
private:
    T _first;
    T _last;
    T _step;
    T _offset;
	T _offsetsize;
	uint64_t _count;

public:
    class Iterator {
    private:
        Range& range;
        T current;

    public:
        Iterator(Range& r, T current) : range(r), current(current) {}

        T operator*() const { return current; }

        Iterator& operator++() {
            current += range._step;
			range._offset = current * range._offsetsize;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return (range._step > 0) ? (current < other.current) : (current > other.current);
        }
    };

    Range(T last) : _first(0), _last(last), _step(1), _offsetsize(0), _count(0) {}
    Range(T first, T last) : _first(first), _last(last), _step(1), _offsetsize(0), _count(0) {}
    Range(T first, T last, T step) : _first(first), _last(last), _step(step), _offsetsize(0), _count(0) {
        if (_step == 0) {
            throw std::invalid_argument("스텝은 0이 될 수 없습니다.");
        }
    }

    Iterator begin() { return Iterator(*this, _first); }
    Iterator end() { return Iterator(*this, _last); }
	T offset() const { return _offset; }
	void setoffsetsize(T size) { _offsetsize = size; }
	uint64_t count() {return _count++;};
};

// 사용 예제
int main() {
    std::cout << "Range(256) 사용:" << std::endl;
    Range<int> r(256); 
    r.setoffsetsize(4);
    for (auto i : r) {
        std::cout << i << " offset: " << std::hex << r.offset() << ", " << std::dec;
    }
    std::cout << std::endl;

    std::cout << "Range(-10, 10) 사용:" << std::endl;
    for (const auto& i : Range<int>(-10, 10)) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::cout << "Range(0, 10, 2) 사용:" << std::endl;
    for (const auto& i : Range<int>(0, 10, 2)) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::cout << "Range(10, 0, -1) 사용:" << std::endl;
    for (const auto& i : Range<int>(10, 0, -1)) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::cout << "Range(0, -10, -1) 사용:" << std::endl;
    for (const auto& i : Range<int>(0, -10, -1)) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::cout << "Range(0, 1, 0.2) 사용:" << std::endl;
    Range<double> drange(0, 1, 0.2);
	drange.setoffsetsize(0.4);
    for (const auto& i :drange) {
        std::cout << i << " offset: " << drange.offset() << " count: " << drange.count() << std::endl;
    }
    std::cout << std::endl;

    try {
        Range<int>(0, 10, 0);
    } catch (const std::invalid_argument& e) {
        std::cout << "예외 발생: " << e.what() << std::endl;
    }

    return 0;
}

