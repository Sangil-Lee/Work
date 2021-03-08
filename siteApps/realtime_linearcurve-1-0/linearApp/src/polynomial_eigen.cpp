#include <iostream>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

VectorXd polynomial(VectorXd xvals, VectorXd yvals, int order)
{
	assert(xvals.size() == yvals.size());
	assert(order >= 1 && order <= xvals.size()-1 );

	MatrixXd A(xvals.size(), order + 1);

	for(int i = 0; i < xvals.size(); i++) {
		A(i, 0) = 1.0;
	};

	for(int j = 0; j < xvals.size(); j++) {
		for(int i = 0; i < order; i++)
		{
			A(j, i+1) = A(j,i)*xvals(j);
		};
	};

	auto Q = A.householderQr();
	auto coeff = Q.solve(yvals);

	return coeff;
}

double polynomial_calc(VectorXd coeffs, double xval)
{
	double result = 0.0;
	for(int i = 0; i < coeffs.size(); i++)
		result += coeffs(i) * pow(xval, i);

	return result;
}

int main()
{
#if 0
  VectorXd xvals(5);
  VectorXd yvals(5);
  // x waypoint coordinates
  xvals << 1.4, 2.6, 3.2, 5.8, 3.7;
  // y waypoint coordinates
  yvals << 7.2, 8.3, 9.5, 11.4, 8.4;
#endif

  int degree, N;
  cout.precision(4);                        //set precision
  cout.setf(ios::fixed);
  cout<<"\nEnter the no. of data pairs to be entered:\n";        //To find the size of arrays that will store x,y, and z values
  cin>>N;

  VectorXd xvals(N), yvals(N);
  //double x[N],y[N];
  cout<<"\nEnter the x-axis values:\n";                //Input x-values
  for (int i=0;i<N;i++)
	  cin>>xvals[i];

  cout<<"\nEnter the y-axis values:\n";                //Input y-values
  for (int i=0;i<N;i++)
	  cin>>yvals[i];

  cout<<"\nWhat degree of Polynomial do you want to use for the fit?\n";
    cin>>degree;                                // n is the degree of Polynomial 
 

  // TODO: use `polyfit` to fit a third order polynomial to the (x, y)
  //auto coeffs = polyfit(xvals,yvals,3); 
  //
  // polynomial coefficients.
  auto coeffs = polynomial(xvals, yvals, degree); 

  cout << "Coeff: " << coeffs << endl;
  cout << endl;
  //cout << "Coeff: " << coeffs << ", Coeff[0]:" << coeffs(0) << ", Coeff[1]:" << coeffs(1) << ", Coeff[2]:" << coeffs(2) << endl;

  for(int i = 0; i < yvals.size(); i++)
  {
	  double val = polynomial_calc(coeffs, xvals(i));
	  printf("Y-Val[%d](%f) - Poly_Y-Val[%d](%f) = %f\n", i, i, yvals(i), val,  yvals(i) - val);
  };

  return 0;
}
