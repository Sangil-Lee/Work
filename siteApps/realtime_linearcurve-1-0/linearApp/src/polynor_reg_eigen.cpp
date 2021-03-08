#include <iostream>
#include "Eigen/Dense"

using namespace Eigen;
/////////////////////////////////////////////////////////////////////////////////////////////
// Fit a polynomial.
// Adapted from
// https://github.com/JuliaMath/Polynomials.jl/blob/master/src/Polynomials.jl#L676-L716

Eigen::VectorXd polyfit(Eigen::VectorXd xvals, Eigen::VectorXd yvals,int order) 
{
  assert(xvals.size() == yvals.size());
  assert(order >= 1 && order <= xvals.size() - 1);
  Eigen::MatrixXd A(xvals.size(), order + 1);

  for (int i = 0; i < xvals.size(); i++) {
    A(i, 0) = 1.0;
  }

  for (int j = 0; j < xvals.size(); j++) {
    for (int i = 0; i < order; i++) {
      A(j, i + 1) = A(j, i) * xvals(j);
    }
  }

  auto Q = A.householderQr();
  auto result = Q.solve(yvals);
  return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Evaluate a polynomial.
double polyeval(Eigen::VectorXd coeffs, double x) {
  double result = 0.0;
  for (int i = 0; i < coeffs.size(); i++) {
    result += coeffs[i] * pow(x, i);
  }
  return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////   Test block

int main() 
{
#if 0
  Eigen::VectorXd xvals(6);
  Eigen::VectorXd yvals(6);
  // x waypoint coordinates
  xvals << 9.261977, -2.06803, -19.6663, -36.868, -51.6263, -66.3482;
  // y waypoint coordinates
  yvals << 5.17, -2.25, -15.306, -29.46, -42.85, -57.6116;
#else
  Eigen::VectorXd xvals(5);
  Eigen::VectorXd yvals(5);
  // x waypoint coordinates
  xvals << 1.4, 2.6, 3.2, 5.8, 3.7;
  // y waypoint coordinates
  yvals << 7.2, 8.3, 9.5, 11.4, 8.4;
#endif
 

  // TODO: use `polyfit` to fit a third order polynomial to the (x, y)
  // coordinates.
  auto coeff = polyfit(xvals,yvals,2); 
  //auto coeff = polyfit(xvals,yvals,3); 

  std::cout << "Coeff: " << coeff << ", Coeff[0]:" << coeff[0] << ", Coeff[1]:" << coeff[1] << ", Coeff[2]:" << coeff[2] << std::endl;
  std::cout << std::endl;


  for (double x = 0; x <= 20; x += 1.0) {
    // TODO: use `polyeval` to evaluate the x values.

    std::cout << polyeval(coeff,x) << std::endl; 
 }
  
}
