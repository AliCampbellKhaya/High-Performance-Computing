#include <vector>
#include <stdexcept>

void daxpy(double a, const std::vector<double> &x, std::vector<double> &y)  {
    
    if (x.size() != y.size()) {
        throw std::invalid_argument("It is not possible to add vectors of different sizes.");
    }

    for (int i = 0; i < x.size(); i++) {
        y[i] = a * x[i] + y[i];
    }
    
}