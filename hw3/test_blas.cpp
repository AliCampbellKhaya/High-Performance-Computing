#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include "axpyt/ref_axpyt.hpp"
#include "gemvt/ref_gemvt.hpp"
#include "gemmt/ref_gemmt.hpp"

bool close(double a, double b) {
    return std::abs(a - b) < 1e-9;
}

int main() {
    std::cout << "--- Starting BLAS Template Tests ---\n";

    // 1. TEST AXPY (Integer)
    // 2 * [1, 2] + [10, 10] = [12, 14]
    std::vector<int> x1 = {1, 2};
    std::vector<int> y1 = {10, 10};
    axpy<int>(2, x1, y1);
    if (y1[0] == 12 && y1[1] == 14) {
        std::cout << "[PASS] AXPY <int>\n";
    } else {
        std::cout << "[FAIL] AXPY <int> Result: " << y1[0] << ", " << y1[1] << "\n";
    }

    // 2. TEST GEMV (Double)
    // A = [[1, 2], [3, 4]], x = [5, 6], a=1, b=0, y=[0, 0]
    // Result: [1*5 + 2*6, 3*5 + 4*6] = [17, 39]
    std::vector<std::vector<double>> A2 = {{1.0, 2.0}, {3.0, 4.0}};
    std::vector<double> x2 = {5.0, 6.0};
    std::vector<double> y2 = {0.0, 0.0};
    gemv<double>(1.0, A2, x2, 0.0, y2);
    if (close(y2[0], 17.0) && close(y2[1], 39.0)) {
        std::cout << "[PASS] GEMV <double>\n";
    } else {
        std::cout << "[FAIL] GEMV <double>\n";
    }

    // 3. TEST GEMM (Integer)
    // A = [[1, 2], [3, 4]], B = [[5, 6], [7, 8]], C = [[0, 0], [0, 0]]
    // AB = [[19, 22], [43, 50]]
    std::vector<std::vector<int>> A3 = {{1, 2}, {3, 4}};
    std::vector<std::vector<int>> B3 = {{5, 6}, {7, 8}};
    std::vector<std::vector<int>> C3 = {{0, 0}, {0, 0}};
    gemm<int>(1, A3, B3, 0, C3);
    if (C3[0][0] == 19 && C3[1][1] == 50) {
        std::cout << "[PASS] GEMM <int>\n";
    } else {
        std::cout << "[FAIL] GEMM <int>\n";
    }

    // 4. TEST ERROR HANDLING
    // Testing mismatching sizes in AXPY
    std::cout << "Testing Error Catching: ";
    try {
        std::vector<double> xe = {1, 2, 3};
        std::vector<double> ye = {1, 2};
        axpy(1.0, xe, ye);
        std::cout << "[FAIL] Did not catch AXPY size mismatch\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "[PASS] Caught expected error: " << e.what() << "\n";
    }

    std::cout << "--- Tests Complete ---\n";
    return 0;
}