#ifndef REF_AXPYT_HPP
#define REF_AXPYT_HPP

#include <vector>
#include <stdexcept>

template <typename T>
void axpy(T a, const std::vector<T> &x, std::vector<T> &y)  {
    if (x.size() != y.size()) {
        throw std::invalid_argument("Vector x must have the same size as vector y.");
    }

    for (int i = 0; i < x.size(); i++) {
        y[i] = a * x[i] + y[i];
    }
}

#endif