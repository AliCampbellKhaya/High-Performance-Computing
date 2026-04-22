#include <vector>
#include <stdexcept>
#include "ref_dgemm.hpp"

/*
DGEMM performs n * (m * (3 + (p * 2))) floating point operations
Where n is the number of rows in matrix A, m is the number of columns in matrix B,
and p is the number of columns in matrix A
*/

void dgemm(double a, const std::vector<std::vector<double>> &A, const std::vector<std::vector<double>> &B,
            double b, std::vector<std::vector<double>> &C) {

    if (A.empty() || B.empty() || C.empty()) {
        throw std::invalid_argument("Matrix A, B or C cannot be empty.");
    }

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