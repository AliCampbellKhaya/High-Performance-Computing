#include <iostream>
#include <vector>
#include "refBLAS.hpp"

int main() {
    // Library test for DAXPY
    std::vector<double> x = {1.0, 2.0};
    std::vector<double> y = {3.0, 4.0};
    daxpy(2.0, x, y);

    std::cout << "Library Test (DAXPY): y[0] = " << y[0] << " (Expected 5.0)" << std::endl;
    
    // Library test for GEMM
    std::vector<std::vector<int>> A = {{1, 0}, {0, 1}};
    std::vector<std::vector<int>> B = {{5, 6}, {7, 8}};
    std::vector<std::vector<int>> C = {{0, 0}, {0, 0}};
    gemm<int>(1, A, B, 0, C);

    std::cout << "Library Test (GEMM <int>): C[1][1] = " << C[1][1] << " (Expected 8)" << std::endl;

    return 0;
}