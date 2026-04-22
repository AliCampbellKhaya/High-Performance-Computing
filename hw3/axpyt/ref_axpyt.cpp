#include "ref_axpyt.hpp"

template<typename T>
void axpy(T a, const std::vector<T> &x, std::vector<T> &y)  {
    if (x.size() != y.size()) {
        throw std::invalid_argument("Vector x must have the same size as vector y.");
    }

    for (int i = 0; i < x.size(); i++) {
        y[i] = a * x[i] + y[i];
    }
}

template void axpy(int a, const std::vector<int>& x, std::vector<int>& y);
template void axpy(double a, const std::vector<double>& x, std::vector<double>& y);
template void axpy(float a, const std::vector<float>& x, std::vector<float>& y);