#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>
#include "daxpy/ref_daxpy.hpp"
#include "dgemm/ref_dgemm.hpp"
#include "dgemv/ref_dgemv.hpp"

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    long double elapsed_time = 0.L;
    long double avg_time;
    long double flops;
    const int ntrials = 5;

    const double a = 2.5;
    const double b = 2.5;

    std::mt19937 gen(0);
    std::uniform_real_distribution<double> dist(1.0, 10.0);

    std::ofstream outFile("hw3_results.txt");

    // Level 1 BLAS - DAXPY
    std::cout << "Level 1 BLAS - DAXPY \n";
    for (int n = 2; n <= 512; n++) {
        std::vector<double> x(n);
        std::vector<double> y(n);

        for (int i = 0; i < n; i++) {
            x[i] = dist(gen);
            y[i] = dist(gen);
        }

        for (int t = 0; t < ntrials; t++) {
            start = std::chrono::high_resolution_clock::now();

            try {
                daxpy(a, x, y);
            }
            catch (const std::invalid_argument &e) {
                std::cerr << "Error: " << e.what() << "\n\n";
            }

            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            elapsed_time += (duration.count() * 1.e-9);
        }

        avg_time = elapsed_time / static_cast<long double>(ntrials);

        flops = (2.L * n) / avg_time;

        elapsed_time = 0.L;

        std::cout << "n: " << n << ", Flops: " << flops << std::endl;
        if (outFile.is_open()) {
            outFile << "n: " << n << ", Flops: " << flops << std::endl;
        }
    }

    // Level 2 BLAS - DGEMV
    std::cout << "Level 2 BLAS - DGEMV \n";
    for (int n = 2; n <= 512; n++) {

        std::vector<std::vector<double>> A(n, std::vector<double>(n));
        std::vector<double> x(n);
        std::vector<double> y(n);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                A[i][j] = dist(gen);
            }
            x[i] = dist(gen);
            y[i] = dist(gen);
        }

        for (int t = 0; t < ntrials; t++) {
            start = std::chrono::high_resolution_clock::now();

            try {
                dgemv(a, A, x, b, y);
            }
            catch (const std::invalid_argument &e) {
                std::cerr << "Error: " << e.what() << "\n\n";
            }

            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            elapsed_time += (duration.count() * 1.e-9);
        }

        avg_time = elapsed_time / static_cast<long double>(ntrials);

        flops = (n * (3 + (2 * n))) / avg_time;

        elapsed_time = 0.L;
        std::cout << "n: " << n << ", Flops: " << flops << std::endl;
        if (outFile.is_open()) {
            outFile << "n: " << n << ", Flops: " << flops << std::endl;
        }
    }

    // Level 3 BLAS - DGEMM
    std::cout << "Level 3 BLAS - DGEMM \n";
    for (int n = 2; n <= 512; n++) {

        std::vector<std::vector<double>> A(n, std::vector<double>(n));
        std::vector<std::vector<double>> B(n, std::vector<double>(n));
        std::vector<std::vector<double>> C(n, std::vector<double>(n));

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                A[i][j] = dist(gen);
                B[i][j] = dist(gen);
                C[i][j] = dist(gen);
            }
        }

        for (int t = 0; t < ntrials; t++) {
            start = std::chrono::high_resolution_clock::now();

            try {
                dgemm(a, A, B, b, C);
            }
            catch (const std::invalid_argument &e) {
                std::cerr << "Error: " << e.what() << "\n\n";
            }

            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            elapsed_time += (duration.count() * 1.e-9);
        }

        avg_time = elapsed_time / static_cast<long double>(ntrials);

        flops = (n * (n * (3 +(2 * n)))) / avg_time;

        elapsed_time = 0.L;
        std::cout << "n: " << n << ", Flops: " << flops << std::endl;
        if (outFile.is_open()) {
            outFile << "n: " << n << ", Flops: " << flops << std::endl;
        }
    }

    outFile.close();

    return 0;
}