#ifndef REF_GEMM_HPP
#define REF_GEMM_HPP

#include <vector>

template <typename T>
void mm_jki (T a, const std::vector<T>& A, const std::vector<T>& B, T b,
    std::vector<T>& C, int m, int p, int n){

    for (int i = 0; i < m * n; i++) {
        C[i] *= b;
    }

    for (int j = 0; j < n; j++) {
        for (int k = 0; k < p; k++) {
            T B2 = a * B[k * n + j];
            for (int i = 0; i < m; i++) {
                C[i * n + j] += A[i * p + k] * B2;
            }
        }
    }

}

template <typename T>
void mm_kij (T a, const std::vector<T>& A, const std::vector<T>& B, T b,
    std::vector<T>& C, int m, int p, int n){

    for (int i = 0; i < m * n; i++) {
        C[i] = C[i] * b;
    }

    for (int k = 0; k < p; k++) {
        for (int i = 0; i < m; i++) {
            T A2 = a * A[i * p + k];
            for (int j = 0; j < n; j++) {
                C[i * n + j] += A2 * B[k * n + j];
            }
        }
    }

}



#endif