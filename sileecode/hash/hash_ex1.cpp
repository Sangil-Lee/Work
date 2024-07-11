#include <iostream>
#include <unordered_map>
#include <vector>

#include <Eigen/Dense>

//g++ hash_ex1.cpp -I /usr/include/eigen3 
using namespace Eigen;

struct Pos
{
  int freq;
  int angle;
  int dir;

  Pos() { }
  Pos(int freq, int angle, int dir)
  {
    this->freq = freq;
    this->angle = angle;
    this->dir = dir;
  }

  bool operator==(const Pos& other) const
  {
    if (this->freq == other.freq && this->angle == other.angle && this->dir == other.dir) return true;
    else return false;
  }

  struct HashFunction
  {
    size_t operator()(const Pos& pos) const
    {
      size_t freqHash = std::hash<int>()(pos.freq);
      size_t angleHash = std::hash<int>()(pos.angle) << 1;
      size_t dirHash = std::hash<int>()(pos.dir) << 2;
      return freqHash ^ angleHash ^ dirHash;
    }
  };
};

int main() {
    // 행렬 데이터를 unordered_map에 저장합니다.
    std::unordered_map<std::string, std::vector<std::vector<double>>> matrixMap;

    // 예시로 2x2 행렬을 저장합니다.
    std::vector<std::vector<double>> matrixA = {{1.943, 2.343}, {35.423, 40.242}};
    std::vector<std::vector<double>> matrixB = {{5.443, 6834.342}, {734.2323, 8.394}};

    // unordered_map에 행렬 데이터 추가
    matrixMap["MatrixA"] = matrixA;
    matrixMap["MatrixB"] = matrixB;

    // 행렬 데이터 조회
    std::cout << "MatrixA:" << std::endl;
    for (const auto& row : matrixMap["MatrixA"]) {
        for (double val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "MatrixB:" << std::endl;
    for (const auto& row : matrixMap["MatrixB"]) {
        for (double val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, double>>> threeDMap;

    // 예시로 (1, 2, 3) 키값에 해당하는 값을 저장
    int x = 1, y = 2, z = 3;
    double value = 42.342;

    // 3차원 키값에 해당하는 값 저장
    threeDMap[x][y][z] = value;
    std::cout << "Value at (" << x << ", " << y << ", " << z << "): " << threeDMap[x][y][z] << std::endl;

	std::unordered_map<Pos, std::vector<std::vector<double>>, Pos::HashFunction> umap;
	std::cout << matrixA[0][0] << ", " << matrixA[0][1] << std::endl;

	matrixA[0][0] = 5.67;
	matrixA[0][1] = 8.232;

	// at row 1, col 2, set value to 5
	umap[Pos(3, 1, 2)] = matrixA;

#if 1
    //for (const auto& row : umap[Pos(1,2)]) {
    for (const auto& row : umap[Pos(3, 1,2)]) {
        for (double val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
#endif

	std::unordered_map<Pos, MatrixXd, Pos::HashFunction> umatrix;
	Matrix2d mat(2,2);
	mat << 1,2,3,4;
	umatrix[Pos(1,2,3)] = mat;

	//Matrix3d mat2(3,3);
	MatrixXd mat2(2,5), mat3(2,5);
	mat2 << 1,2,3,4,5,6,7,8,9,10;
	mat3 = mat2+MatrixXd::Constant(2,5,1.5);
	mat2 *= 12.34;

	umatrix[Pos(2,2,3)] = mat2;
	umatrix[Pos(3,2,3)] = mat3;

	std::cout << umatrix[Pos(1,2,3)] * 10 << std::endl;
	std::cout << umatrix[Pos(1,2,3)](0,0) << std::endl;
	std::cout << umatrix[Pos(2,2,3)] << std::endl;
	std::cout << umatrix[Pos(3,2,3)] << std::endl;

    return 0;
}
