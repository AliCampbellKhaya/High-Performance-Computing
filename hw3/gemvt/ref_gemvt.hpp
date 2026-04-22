#ifndef REF_GEMVT_HPP
#define REF_GEMVT_HPP

#include <vector>
#include <stdexcept>

template <typename T>
void gemv(T a, const std::vector<std::vector<T>> &A, const std::vector<T> &x,
            T b, std::vector<T> &y) {
    
    if (A[0].size() != x.size()) {
        throw std::invalid_argument("Matrix A must have the same number of columns as elements in vector x.");
    }

    if (A.size() != y.size()) {
        throw std::invalid_argument("Matrix Ax must be a vector with the same dimensions as vector y.");
    }

    for (int i = 0; i < A.size(); i++) {
        double sum = 0;

        for (int j = 0; j < A[0].size(); j++) {

            sum += A[i][j] * x[j];

        }

        y[i] = a * sum + b * y[i];
    }
}

#endif