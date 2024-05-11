#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> sortedNumbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int target = 7;

    bool found = std::binary_search(sortedNumbers.begin(), sortedNumbers.end(), target);

    if (found) {
        std::cout << "Element " << target << " found." << std::endl;
    } else {
        std::cout << "Element " << target << " not found." << std::endl;
    }

	auto print = [](const int& x) { std::cout << x << ","; };
	auto newline = []() {std::cout << std::endl;};

	std::vector<int> data{1, 2, 3, 4, 5, 6, 7, 8};
	std::transform(data.begin(), data.end(),
			data.begin(), [](int v) { return v*2; });
	// data = {2, 4, 6, 8, 10, 12, 14, 16}
	std::for_each(data.begin(), data.end(), print);
	newline();

	std::vector<int> add{8, 7, 6, 5, 4, 3, 2, 1};
	std::transform(data.begin(), data.end(), add.begin(),
			data.begin(), [](int left, int right) {
			return left+right; });
	// data = {10, 11, 12, 13, 14, 15, 16, 17}
	for_each(data.begin(), data.end(), print);
	newline();

	std::vector<int> out;
	std::transform(data.begin(), data.end(),
			std::back_inserter(out), [](int v) { return v*2; });
	// out = {2, 4, 6, 8, 10, 12, 14, 16}
	for_each(out.begin(), out.end(), print);
	newline();


	struct MyStruct {
    void some_modifying_operation() {}
};

    return 0;
}
