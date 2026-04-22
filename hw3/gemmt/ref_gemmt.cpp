#include "ref_gemmt.hpp"

template <typename T>
void gemm(T a, const std::vector<std::vector<T>> &A, const std::vector<std::vector<T>> &B,
            T b, std::vector<std::vector<T>> &C) {
    if (A[0].size() != B.size()) {
        throw std::invalid_argument("Matrix A must have the same number of columns as rows in matrix B.");
    }

    if (A.size() != C.size() || B[0].size() != C[0].size()) {
        throw std::invalid_argument("Matrix AB must have the same dimensions as matrix C.");
    }

    for (int i = 0; i < A.size(); i++) {
        
        for (int j = 0; j < B[0].size(); j++) {
            double sum = 0;

            for (int k = 0; k < A[0].size(); k++) {
                sum += A[i][k] * B[k][j];

            }
                
            C[i][j] = a * sum + b * C[i][j];

        }

        
    }
}

template void gemm(int a, const std::vector<std::vector<int>> &A, const std::vector<std::vector<int>> &B,
    int b, std::vector<std::vector<int>> &C);
template void gemm(double a, const std::vector<std::vector<double>> &A, const std::vector<std::vector<double>> &B,
    double b, std::vector<std::vector<double>> &C);
template void gemm(float a, const std::vector<std::vector<float>> &A, const std::vector<std::vector<float>> &B,
    float b, std::vector<std::vector<float>> &C);