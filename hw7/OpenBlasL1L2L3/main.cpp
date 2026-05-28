#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>
#include "cblas_daxpy.hpp"
#include "cblas_dgemm.hpp"
#include "cblas_dgemv.hpp"

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    long double elapsed_time = 0.L;
    long double avg_time;
    const int ntrials = 5;

    const double alpha = 2.5;
    const double beta = 2.5;

    std::mt19937 gen(0);
    std::uniform_real_distribution<double> dist(1.0, 10.0);

    std::ofstream daxpyOut("hw7_daxpy.csv");
    std::ofstream dgemmOut("hw7_dgemm.csv");
    std::ofstream dgemvOut("hw7_dgemv.csv");

    if (daxpyOut.is_open()) {
            daxpyOut << "n,mflops\n";
    }
    if (dgemmOut.is_open()) {
            dgemmOut << "n,mflops\n";
    }
    if (dgemvOut.is_open()) {
            dgemvOut << "n,mflops\n";
    }

    for (int n = 2; n <= 4096; n*=2) {

        double X[n]; double Y[n];
        double A[n * n]; double B[n * n]; double C[n * n];

        for (int i = 0; i < n; i++) {
            X[i] = dist(gen);
            Y[i] = dist(gen);
        }

        for (int i = 0; i < n * n; i++) {
            A[i] = dist(gen);
            B[i] = dist(gen);
            C[i] = dist(gen);
        }

        // DAXPY

        for (int t = 0; t < ntrials; t++) {
            start = std::chrono::high_resolution_clock::now();

            try {
                cblas_daxpy(n, alpha, X, 1, Y, 1);
            }
            catch (const std::invalid_argument &e) {
                std::cerr << "Error: " << e.what() << "\n\n";
            }

            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            elapsed_time += (duration.count() * 1.e-9);
        }

        avg_time = elapsed_time / static_cast<long double>(ntrials);

        long double daxpy_flops = (2.L * n) / avg_time;
        long double daxpy_mflops = daxpy_flops / 1e6;

        elapsed_time = 0.L;

        std::cout << "Daxpy n: " << n << ", MFlops: " << daxpy_mflops << std::endl;
        if (daxpyOut.is_open()) {
            daxpyOut << n << "," << daxpy_mflops << "\n";
        }

        // DGEMV

        for (int t = 0; t < ntrials; t++) {
            start = std::chrono::high_resolution_clock::now();

            try {
                cblas_dgemv(CblasColMajor, CblasNoTrans, n, n, alpha, A, n, X, 1, beta, Y, 1);
            }
            catch (const std::invalid_argument &e) {
                std::cerr << "Error: " << e.what() << "\n\n";
            }

            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            elapsed_time += (duration.count() * 1.e-9);
        }

        avg_time = elapsed_time / static_cast<long double>(ntrials);

        long double dgemv_flops = (2.L * n) / avg_time;
        long double dgemv_mflops = dgemv_flops / 1e6;

        elapsed_time = 0.L;

        std::cout << "Dgemv n: " << n << ", MFlops: " << dgemv_mflops << std::endl;
        if (dgemvOut.is_open()) {
            dgemvOut << n << "," << dgemv_mflops << "\n";
        }

        // DGEMM

        for (int t = 0; t < ntrials; t++) {
            start = std::chrono::high_resolution_clock::now();

            try {
                cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, n, n, n, alpha, A, n, B, n, beta, C, n);
            }
            catch (const std::invalid_argument &e) {
                std::cerr << "Error: " << e.what() << "\n\n";
            }

            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            elapsed_time += (duration.count() * 1.e-9);
        }

        avg_time = elapsed_time / static_cast<long double>(ntrials);

        long double dgemm_flops = (2.L * n) / avg_time;
        long double dgemm_mflops = daxpy_flops / 1e6;

        elapsed_time = 0.L;

        std::cout << "Dgemm n: " << n << ", MFlops: " << dgemm_mflops << std::endl;
        if (dgemmOut.is_open()) {
            dgemmOut << n << "," << dgemm_mflops << "\n";
        }

    }

    daxpyOut.close(); dgemvOut.close(); dgemmOut.close();

    return 0;
}