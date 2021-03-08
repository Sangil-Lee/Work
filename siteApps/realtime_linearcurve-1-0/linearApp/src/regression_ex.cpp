#include<iostream>
#include<iomanip>
#include<cmath>
using namespace std;

void linear(float x[], float y[1], int n){
    float sumx=0,sumy=0,sumxy=0,sumx2=0;
    double a,b;
    for(int i=0;i<=n-1;i++)
    {
        sumx=sumx +x[i];
        sumx2=sumx2 +x[i]*x[i];
        sumy=sumy +y[i];
        sumxy=sumxy +x[i]*y[i];

    }
    a=((sumx2*sumy -sumx*sumxy)*1.0/(n*sumx2-sumx*sumx)*1.0);
    b=((n*sumxy-sumx*sumy)*1.0/(n*sumx2-sumx*sumx)*1.0);
    printf("\n\nThe line is Y=%3.3f +%3.3f X",a,b);
}

void exponential(float x[], float y[], int n){
    float Y[n],sumx=0,sumy=0,sumxy=0,sumx2=0;
    double a,b,A;
    for(int i=0;i<=n-1;i++)
    {
        Y[i]=log(y[i]);
    }
    for(int i=0;i<=n-1;i++)
    {
        sumx=sumx +x[i];
        sumx2=sumx2 +x[i]*x[i];
        sumy=sumy +Y[i];
        sumxy=sumxy +x[i]*Y[i];

    }
    A=((sumx2*sumy -sumx*sumxy)*1.0/(n*sumx2-sumx*sumx)*1.0);
    b=((n*sumxy-sumx*sumy)*1.0/(n*sumx2-sumx*sumx)*1.0);
    a=exp(A);
    printf("\n\n The curve is Y= %4.3fe^%4.3fX",a,b);
}

void poly(float x[], float y[], int N){
    int n;
    cout << "\nPlease enter the degress:\n";
    cin >> n;

    double X[2*n+1];                        //Array that will store the values of sigma(xi),sigma(xi^2),sigma(xi^3)....sigma(xi^2n)
    for (int i=0;i<2*n+1;i++)
    {
        X[i]=0;
        for (int j=0;j<N;j++)
            X[i]=X[i]+pow(x[j],i);        //consecutive positions of the array will store N,sigma(xi),sigma(xi^2),sigma(xi^3)....sigma(xi^2n)
    }
    double B[n+1][n+2],a[n+1];            //B is the Normal matrix(augmented) that will store the equations, 'a' is for value of the final coefficients
    for (int i=0;i<=n;i++)
        for (int j=0;j<=n;j++)
            B[i][j]=X[i+j];            //Build the Normal matrix by storing the corresponding coefficients at the right positions except the last column of the matrix
    double Y[n+1];                    //Array to store the values of sigma(yi),sigma(xi*yi),sigma(xi^2*yi)...sigma(xi^n*yi)
    for (int i=0;i<n+1;i++)
    {
        Y[i]=0;
        for (int j=0;j<N;j++)
            Y[i]=Y[i]+pow(x[j],i)*y[j];        //consecutive positions will store sigma(yi),sigma(xi*yi),sigma(xi^2*yi)...sigma(xi^n*yi)
    }
    for (int i=0;i<=n;i++)
        B[i][n+1]=Y[i];                //load the values of Y as the last column of B(Normal Matrix but augmented)
    n=n+1;                //n is made n+1 because the Gaussian Elimination part below was for n equations, but here n is the degree of polynomial and for n degree we get n+1 equations

    for (int i=0;i<n;i++)                    //From now Gaussian Elimination starts(can be ignored) to solve the set of linear equations (Pivotisation)
        for (int k=i+1;k<n;k++)
            if (B[i][i]<B[k][i])
                for (int j=0;j<=n;j++)
                {
                    double temp=B[i][j];
                    B[i][j]=B[k][j];
                    B[k][j]=temp;
                }

    for (int i=0;i<n-1;i++)            //loop to perform the gauss elimination
        for (int k=i+1;k<n;k++)
        {
            double t=B[k][i]/B[i][i];
            for (int j=0;j<=n;j++)
                B[k][j]=B[k][j]-t*B[i][j];    //make the elements below the pivot elements equal to zero or elimnate the variables
        }
    for (int i=n-1;i>=0;i--)                //back-substitution
    {                        //x is an array whose values correspond to the values of x,y,z..
        a[i]=B[i][n];                //make the variable to be calculated equal to the rhs of the last equation
        for (int j=0;j<n;j++)
            if (j!=i)            //then subtract all the lhs values except the coefficient of the variable whose value                                   is being calculated
                a[i]=a[i]-B[i][j]*a[j];
        a[i]=a[i]/B[i][i];            //now finally divide the rhs by the coefficient of the variable to be calculated
    }

    cout<<"\n\nThe curve is:Y=";
    for (int i=0;i<n;i++)
        cout<<" + ("<<a[i]<<")"<<"x^"<<i;
    cout<<"\n";

}

int main() {
    //cout.precision(4);                        //set precision
    //cout.setf(ios::fixed);
    int N;
    cout << "\nEnter the no. of data pairs to be entered:\n";        //To find the size of arrays that will store x,y, and z values
    cin >> N;
    float x[N],y[N];
    cout << "\nEnter the x-axis values:\n";                //Input x-values
    for (int i = 0;i < N;i++)
        cin >> x[i];
    cout<<"\nEnter the y-axis values:\n";                //Input y-values
    for (int i = 0;i < N;i++)
        cin>>y[i];
    char selection;
    cout << "\nPlease select your regression analysis type:\nA. Linear Regression\nB. Exponential Regression\n"
            "C. Polynomial Fit Regression\n";  //Let user select regression type
    cin >> selection;
     if(selection == 'A' || selection == 'a'){          //apply linear regression
         //linearFunction(x,y);
         linear(x, y, N);
     }
     else if(selection == 'B' || selection == 'b'){       //apply Exponential regression
         //Exponential Regression(x,y)
         exponential(x, y, N);
    }
    else if(selection == 'C' || selection == 'c'){             //apply PolynomialFit Regression
         //PolynomialFit Function(x,y);
        poly(x, y, N);
    }
    else{
        cout << "error";
    }
    return 0;
}

