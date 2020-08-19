Eigen has an unsupported spline module where you can fit a spline given some
data. Once a spline is fitted you can use it as a function to obtain points
inbetween. The following example fits 1D data but can be modified to fit
N-dimensional data.

## Usage

     git clone https://gist.github.com/chutsu/815c7c916c329eec85f34690a012f7cb spline_example
     g++ -I/usr/include/eigen3 spline_example.cpp -o spline_example
     ./spline_example
