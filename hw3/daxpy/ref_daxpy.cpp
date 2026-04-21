#include <vector>
#include <stdexcept>

/*
Each iteration of DAXPY performs 2 floating point operations (1 + and 1 *)
So total operations is 2n where n is the size of matrix x/y
*/
void daxpy(double a, const std::vector<double> &x, std::vector<double> &y)  {
    
    if (x.size() != y.size()) {
        throw std::invalid_argument("Vector x must have the same size as vector y.");
    }

    for (int i = 0; i < x.size(); i++) {
        y[i] = a * x[i] + y[i];
    }

    
}