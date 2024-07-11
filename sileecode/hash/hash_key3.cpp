#include <iostream>
#include <unordered_map>
#include <vector>

int main() {
    // 3차원 키값을 사용하는 unordered_map 선언
    std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, int>>> threeDMap;

    // 예시로 (1, 2, 3) 키값에 해당하는 값을 저장
    int x = 1, y = 2, z = 3;
    int value = 42;

    // 3차원 키값에 해당하는 값 저장
    threeDMap[x][y][z] = value;

    // 3차원 키값에서 값 조회
    std::cout << "Value at (" << x << ", " << y << ", " << z << "): " << threeDMap[x][y][z] << std::endl;

    return 0;
}

