#ifndef REF_GEMMT_HPP
#define REFGEMMT_HPP

#include <vector>
#include <stdexcept>

template <typename T>
void gemm(T a, const std::vector<std::vector<T>> &A, const std::vector<std::vector<T>>,
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

#endif