#include <vector>

using namespace std;

bool cubic_spline(std::vector<double>* x_series, std::vector<double>* y_series, std::vector<double> *destX, std::vector<double>* destY)
{   
    //int n = __min((int)x_series->size()-1, (int)y_series->size()-1);
    int n = min((int)x_series->size()-1, (int)y_series->size()-1);

    // Step 1.
    double *h = new double[n+1];
    double *alpha = new double[n+1];

    int i = 0;
    for(i = 0; i<=n-1; i++)
	{
        h[i] = (*x_series)[i+1] - (*x_series)[i];
    };

    // Step 2.
    for(i = 1; i<=n-1;i++)
	{
        alpha[i]= 3*((*y_series)[i+1]-(*y_series)[i])/h[i]-3*((*y_series)[i]-(*y_series)[i-1])/h[i-1];
    };

    // Step 3.
    double *l = new double[n+1];
    double *u = new double[n+1];
    double *z = new double[n+1];
    double *c = new double[n+1];
    double *b = new double[n+1];
    double *d = new double[n+1];
    
	l[0] = 1; u[0] = 0; z[0] = 0;

    // Step 4.
    for(i = 1; i<=n-1; i++)
	{
        l[i] = 2*((*x_series)[i+1] - (*x_series)[i-1]) - h[i-1]*u[i-1];
        u[i] = h[i]/l[i];
        z[i] = (alpha[i] - h[i-1]*z[i-1]) / l[i];

    };

    // Step 5.
    l[n] = 1;     z[n] = 0;     c[n] = 0;

    // Step 6.
    for(i = n-1; i>=0; i--)
	{
        c[i] = z[i] - u[i]*c[i+1];
        b[i] = ((*y_series)[i+1] - (*y_series)[i])/h[i] - h[i]*(c[i+1] + 2*c[i])/3;
        d[i] = (c[i+1] - c[i]) / (3*h[i]);

    };

    for(i = 0; i<=n-2;i++)
	{
        double x = (*x_series)[i];
        double inc = ((*x_series)[i+1] - (*x_series)[i])*0.1;

        for(; x < (*x_series)[i+1]; x+=inc)
		{
            double x_offset = x - (*x_series)[i];
            double Sx = (*y_series)[i] + b[i]*x_offset + c[i]*x_offset*x_offset + d[i]*x_offset*x_offset*x_offset;

            if(destX != NULL){
                destX->push_back(x);
            };

            if(destY != NULL){
                destY->push_back(Sx);
            };
        };
    };

    delete [] h;
    delete [] alpha;
    delete [] l;
    delete [] u;
    delete [] z;
    delete [] c;
    delete [] b;
    delete [] d;

    return true;
}
