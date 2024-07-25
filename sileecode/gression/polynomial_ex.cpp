
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>

using namespace std;

class LinearRegression {
private:
    vector<double> x;
    vector<double> y;
    double slope;
    double intercept;

public:
    LinearRegression(const vector<double>& x, const vector<double>& y) : x(x), y(y), slope(0), intercept(0) {
        calculateCoefficients();
    }

    void calculateCoefficients() {
        double x_mean = accumulate(x.begin(), x.end(), 0.0) / x.size();
        double y_mean = accumulate(y.begin(), y.end(), 0.0) / y.size();

        double numerator = 0;
        double denominator = 0;

        for (size_t i = 0; i < x.size(); ++i) {
            numerator += (x[i] - x_mean) * (y[i] - y_mean);
            denominator += (x[i] - x_mean) * (x[i] - x_mean);
        }

        slope = numerator / denominator;
        intercept = y_mean - (slope * x_mean);
    }

    double predict(double x_value) const {
        return intercept + slope * x_value;
    }

    double getSlope() const {
        return slope;
    }

    double getIntercept() const {
        return intercept;
    }
};

class PolynomialRegression {
private:
    vector<double> x;
    vector<double> y;
    vector<double> coefficients;
    int degree;

public:
    PolynomialRegression(const vector<double>& x, const vector<double>& y, int degree) : x(x), y(y), degree(degree) {
        calculateCoefficients();
    }

    void calculateCoefficients() {
        int n = x.size();
        int m = degree + 1;
        vector<vector<double>> A(m, vector<double>(m, 0));
        vector<double> B(m, 0);

        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < m; ++j) {
                for (int k = 0; k < n; ++k) {
                    A[i][j] += pow(x[k], i + j);
                }
            }
            for (int k = 0; k < n; ++k) {
                B[i] += y[k] * pow(x[k], i);
            }
        }

        coefficients = gaussianElimination(A, B);
    }

    vector<double> gaussianElimination(vector<vector<double>>& A, vector<double>& B) {
        int n = B.size();
        for (int i = 0; i < n; ++i) {
            int maxRow = i;
            for (int k = i + 1; k < n; ++k) {
                if (abs(A[k][i]) > abs(A[maxRow][i])) {
                    maxRow = k;
                }
            }

            for (int k = i; k < n; ++k) {
                swap(A[maxRow][k], A[i][k]);
            }
            swap(B[maxRow], B[i]);

            for (int k = i + 1; k < n; ++k) {
                double factor = A[k][i] / A[i][i];
                B[k] -= factor * B[i];
                for (int j = i; j < n; ++j) {
                    A[k][j] -= factor * A[i][j];
                }
            }
        }

        vector<double> result(n);
        for (int i = n - 1; i >= 0; --i) {
            result[i] = B[i] / A[i][i];
            for (int k = i - 1; k >= 0; --k) {
                B[k] -= A[k][i] * result[i];
            }
        }
        return result;
    }

    double predict(double x_value) const {
        double result = 0;
        for (int i = 0; i <= degree; ++i) {
            result += coefficients[i] * pow(x_value, i);
        }
        return result;
    }

    vector<double> getCoefficients() const {
        return coefficients;
    }
};

int main() {
    vector<double> x = {1, 2, 3, 4, 5};
    vector<double> y = {2, 3, 5, 7, 11};

    LinearRegression lr(x, y);

    cout << "Slope: " << lr.getSlope() << endl;
    cout << "Intercept: " << lr.getIntercept() << endl;

    double x_value = 6;
    cout << "Prediction for x = " << x_value << ": " << lr.predict(x_value) << endl;

    int degree = 2;
    PolynomialRegression pr(x, y, degree);

    cout << "Polynomial Coefficients: ";
    for (double coeff : pr.getCoefficients()) {
        cout << coeff << " ";
    }
    cout << endl;

    cout << "Polynomial Prediction for x = " << x_value << ": " << pr.predict(x_value) << endl;

    return 0;
}

