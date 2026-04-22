#include "ref_gemvt.hpp"

template <typename T>
void gemv(T a, const std::vector<std::vector<T>> &A, const std::vector<T> &x,
            T b, std::vector<T> &y) {

    if (A.empty()) {
        throw std::invalid_argument("Matrix A cannot be empty.");
    }
    
    if (A[0].size() != x.size()) {
        throw std::invalid_argument("Matrix A must have the same number of columns as elements in vector x.");
    }

    if (A.size() != y.size()) {
        throw std::invalid_argument("Matrix Ax must be a vector with the same dimensions as vector y.");
    }

    for (int i = 0; i < A.size(); i++) {
        T sum = 0;

        for (int j = 0; j < A[0].size(); j++) {

            sum += A[i][j] * x[j];

        }

        y[i] = a * sum + b * y[i];
    }
}

template void gemv(int a, const std::vector<std::vector<int>>& A, const std::vector<int>& x,
    int b, std::vector<int>& y);  
template void gemv(double a, const std::vector<std::vector<double>>& A, const std::vector<double>& x,
    double b, std::vector<double>& y);  
template void gemv(float a, const std::vector<std::vector<float>>& A, const std::vector<float>& x,
    float b, std::vector<float>& y);  