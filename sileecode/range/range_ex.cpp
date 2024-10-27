#include <iostream>
#include <stdexcept>

template<typename T>
class Range {
public:
	T _offset;
private:
    T _first;
    T _last;
    T _step;
    //T _offset;
	uint64_t _count;

public:
    class Iterator {
	public:
		T _offset;
    private:
        T _current;
        T _step;
        T _last;

    public:
        Iterator(T current, T step, T last) : _current(current), _step(step), _last(last), _offset(4) {}

        T operator*() const { return _current; }

        Iterator& operator++() {
            _current += _step;
			Range<T>::_offset = (_offset*_current);
			std::cout << "offset: " << (_offset*_current) << std::endl;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return (_step > 0) ? (_current < other._current) : (_current > other._current);
        }
		
    };

    Range(T last) : _first(0), _last(last), _step(1), _count(0), _offset(0) {}
    Range(T first, T last) : _first(first), _last(last), _step(1), _count(0), _offset(0) {}
    Range(T first, T last, T step) : _first(first), _last(last), _step(step), _count(0) {
        if (_step == 0) {
            throw std::invalid_argument("스텝은 0이 될 수 없습니다.");
        }
    }

    Range(T first, T last, T step, T offset) : _first(first), _last(last), _step(step), _count(0), _offset(offset) {
        if (_step == 0) {
            throw std::invalid_argument("스텝은 0이 될 수 없습니다.");
        }
    }

    Iterator begin() const { return Iterator(_first, _step, _last); }
    Iterator end() const { return Iterator(_last, _step, _last); }

	uint64_t count() {return _count++;};
};

// 사용 예제
int main() {
    std::cout << "Range(5) 사용:" << std::endl;
    for (auto i : Range<int>(5)) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::cout << "Range(1, 10) 사용:" << std::endl;
    for (const auto& i : Range<int>(1, 10)) {
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

    std::cout << "Range(0, 1, 0.1) 사용:" << std::endl;
    Range<double> drange(0, 1, 0.1);
    for (const auto& i : drange) {
        std::cout << i << " count: " << drange.count() << ", ";
    }
    std::cout << std::endl;

    try {
        Range<int>(0, 10, 0);
    } catch (const std::invalid_argument& e) {
        std::cout << "예외 발생: " << e.what() << std::endl;
    }

    return 0;
}

