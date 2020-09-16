//Linear Regression, y = ax + b, finding (a,b)
//
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

int main()
{
    int n = 0;

    cout<<"\nEnter the no. of data pairs to be entered:\n";        //To find the size of arrays
    cin>>n;

    double x[n],y[n];

    cout<<"\nEnter the x-axis values:\n";     //Input x-values
    for (int i = 0;i < n; i++) cin>>x[i];

    cout<<"\nEnter the y-axis values:\n";     //Input y-values
    for (int i = 0;i < n;i++) cin>>y[i];

    double xsum=0,x2sum=0,ysum=0,xysum=0;     //variables for sums/sigma of xi,yi,xi^2,xiyi etc

    for (int i=0;i < n; i++) {
        xsum  += x[i];            //calculate sigma(xi)
        ysum  += y[i];            //calculate sigma(yi)
        x2sum += pow(x[i],2);     //calculate sigma(x^2i)
        xysum += x[i]*y[i];       //calculate sigma(xi*yi)
    };


	cout << "Size: " << n << endl;
	//calculate slope
    double a = (n*xysum-xsum*ysum)/(n*x2sum-xsum*xsum);

	//calculate intercept
    double b = (x2sum*ysum-xsum*xysum)/(n*x2sum-xsum*xsum);

	//an array to store the new fitted values of y    
    double y_fit[n]; 

    for (int i=0; i < n; i++) {
		//to calculate y(fitted) at given x points
        y_fit[i] = a*x[i]+b; 
	};

    cout<<"S.no"<<setw(5)<<"x"<<setw(19)<<"y(observed)"<<setw(19)<<"y(fitted)(diff)"<<endl;
    cout<<"-----------------------------------------------------------------\n";

    for (int i=0; i < n; i++) {
		//print a table of x,y(obs.) and y(fit.)    
        cout<<i+1<<"."<<setw(8)<<x[i]<<setw(15)<<y[i]<<setw(18)<<y_fit[i]<<"("<<abs(y[i]-y_fit[i]) << ")"<<endl;
	};

    cout<<"\nThe linear fit line is of the form:\n\n"<< a << "x + "<< b << endl;    //print the best fit line

    return 0;
}   
