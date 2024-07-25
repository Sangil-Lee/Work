
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

int main() {
    vector<double> x = {1, 2, 3, 4, 5};
    vector<double> y = {2, 3, 5, 7, 11};

    LinearRegression lr(x, y);

    cout << "Slope: " << lr.getSlope() << endl;
    cout << "Intercept: " << lr.getIntercept() << endl;

    double x_value = 6;
    cout << "Prediction for x = " << x_value << ": " << lr.predict(x_value) << endl;

    return 0;
}

