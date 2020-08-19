#include <iostream>
#include <random>
#include <boost/math/interpolators/cubic_b_spline.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/math/tools/roots.hpp>

int main()
{
    // We begin with an array of samples:
    std::vector<double> v(500);
    // And decide on a stepsize:
    double step = 0.01;

    // Initialize the vector with a function we'd like to interpolate:
    for (size_t i = 0; i < v.size(); ++i)
    {
        v[i] = sin(i*step);
    }
    // We could define an arbitrary start time, but for now we'll just use 0:
    boost::math::cubic_b_spline<double> spline(v.data(), v.size(), 0 /* start time */, step);

    // Now we can evaluate the spline wherever we please.
    std::mt19937 gen;
    boost::random::uniform_real_distribution<double> absissa(0, v.size()*step);
    for (size_t i = 0; i < 10; ++i)
    {
        double x = absissa(gen);
        std::cout << "sin(" << x << ") = " << sin(x) << ", spline interpolation gives " << spline(x) << std::endl;
        std::cout << "cos(" << x << ") = " << cos(x) << ", spline derivative interpolation gives " << spline.prime(x) << std::endl;
    }

    // The next example is less trivial:
    // We will try to figure out when the population of the United States crossed 100 million.
    // Since the census is taken every 10 years, the data is equally spaced, so we can use the cubic b spline.
    // Data taken from https://en.wikipedia.org/wiki/United_States_Census
    // An eye
    // We'll start at the year 1860:
    double t0 = 1860;
    double time_step = 10;
    std::vector<double> population{31443321,  /* 1860 */
                                   39818449,  /* 1870 */
                                   50189209,  /* 1880 */
                                   62947714,  /* 1890 */
                                   76212168,  /* 1900 */
                                   92228496,  /* 1910 */
                                   106021537, /* 1920 */
                                   122775046, /* 1930 */
                                   132164569, /* 1940 */
                                   150697361, /* 1950 */
                                   179323175};/* 1960 */

    // An eyeball estimate indicates that the population crossed 100 million around 1915.
    // Let's see what interpolation says:
    boost::math::cubic_b_spline<double> p(population.data(), population.size(), t0, 10);

    // Now create a function which has a zero at p = 100,000,000:
    auto f = [=](double t){ return p(t) - 100000000; };

    // Boost includes a bisection algorithm, which is robust, though not as fast as some others 
    // we provide, but lets try that first.  We need a termination condition for it, which
    // takes the two endpoints of the range and returns either true (stop) or false (keep going),
    // we could use a predefined one such as boost::math::tools::eps_tolerance<double>, but that
    // won't stop until we have full double precision which is overkill, since we just need the 
    // endpoint to yield the same month.  While we're at it, we'll keep track of the number of
    // iterations required too, though this is strictly optional:

    auto termination = [](double left, double right)
    {
       double left_month = std::round((left - std::floor(left)) * 12 + 1);
       double right_month = std::round((right - std::floor(right)) * 12 + 1);
       return (left_month == right_month) && (std::floor(left) == std::floor(right));
    };
    std::uintmax_t iterations = 1000;
    auto result =  boost::math::tools::bisect(f, 1910.0, 1920.0, termination, iterations);
    auto time = result.first;  // termination condition ensures that both endpoints yield the same result
    auto month = std::round((time - std::floor(time))*12  + 1);
    auto year = std::floor(time);
    std::cout << "The population of the United States surpassed 100 million on the ";
    std::cout << month << "th month of " << year << std::endl;
    std::cout << "Found in " << iterations << " iterations" << std::endl;

    // Since the cubic B spline offers the first derivative, we could equally have used Newton iterations,
    // this takes "number of bits correct" as a termination condition - 20 should be plenty for what we need,
    // and once again, we track how many iterations are taken:

    auto f_n = [=](double t) { return std::make_pair(p(t) - 100000000, p.prime(t)); };
    iterations = 1000;
    time = boost::math::tools::newton_raphson_iterate(f_n, 1910.0, 1900.0, 2000.0, 20, iterations);
    month = std::round((time - std::floor(time))*12  + 1);
    year = std::floor(time);
    std::cout << "The population of the United States surpassed 100 million on the ";
    std::cout << month << "th month of " << year << std::endl;
    std::cout << "Found in " << iterations << " iterations" << std::endl;

}
