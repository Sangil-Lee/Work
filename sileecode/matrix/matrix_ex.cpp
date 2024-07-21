#include <vector>
#include <iostream>

class Matrix {
public:
    Matrix(int rows, int cols) : rows_(rows), cols_(cols), data_(rows, std::vector<double>(cols, 0.0)) {}

    Matrix(const std::vector<std::vector<double>>& data) : rows_(data.size()), cols_(data[0].size()), data_(data) {}

    int getRows() const { return rows_; }
    int getCols() const { return cols_; }

    std::vector<double>& operator[](int row) { return data_[row]; }
    const std::vector<double>& operator[](int row) const { return data_[row]; }

    Matrix operator+(const Matrix& other) const {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            throw std::invalid_argument("Matrix dimensions must match for addition");
        }
        Matrix result(rows_, cols_);
        for (int i = 0; i < rows_; ++i) {
            for (int j = 0; j < cols_; ++j) {
                result[i][j] = data_[i][j] + other[i][j];
            }
        }
        return result;
    }

    Matrix operator-(const Matrix& other) const {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            throw std::invalid_argument("Matrix dimensions must match for subtraction");
        }
        Matrix result(rows_, cols_);
        for (int i = 0; i < rows_; ++i) {
            for (int j = 0; j < cols_; ++j) {
                result[i][j] = data_[i][j] - other[i][j];
            }
        }
        return result;
    }

    Matrix operator*(const Matrix& other) const {
        if (cols_ != other.rows_) {
            throw std::invalid_argument("Matrix dimensions must match for multiplication");
        }
        Matrix result(rows_, other.cols_);
        for (int i = 0; i < rows_; ++i) {
            for (int j = 0; j < other.cols_; ++j) {
                for (int k = 0; k < cols_; ++k) {
                    result[i][j] += data_[i][k] * other[k][j];
                }
            }
        }
        return result;
    }

    void print() const {
        for (const auto& row : data_) {
            for (const auto& elem : row) {
                std::cout << elem << " ";
            }
            std::cout << std::endl;
        }
    }

private:
    int rows_;
    int cols_;
    std::vector<std::vector<double>> data_;
};

int main() {
    Matrix mat1({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    Matrix mat2({{9, 8, 7}, {6, 5, 4}, {3, 2, 1}});

	mat1[1][2] = 9.3;

    std::cout << "Matrix 1:" << std::endl;
    mat1.print();

    std::cout << "Matrix 2:" << std::endl;
    mat2.print();

    Matrix sum = mat1 + mat2;
    std::cout << "Sum of Matrix 1 and Matrix 2:" << std::endl;
    sum.print();

    Matrix diff = mat1 - mat2;
    std::cout << "Difference of Matrix 1 and Matrix 2:" << std::endl;
    diff.print();

    Matrix prod = mat1 * mat2;
    std::cout << "Product of Matrix 1 and Matrix 2:" << std::endl;
    prod.print();

    return 0;
}

