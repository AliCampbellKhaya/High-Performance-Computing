#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>
#include <cblas.h>

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

        std::vector<double> X(n); 
        std::vector<double> Y(n);
        std::vector<double> A(n * n); 
        std::vector<double> B(n * n); 
        std::vector<double> C(n * n);

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

            cblas_daxpy(n, alpha, X.data(), 1, Y.data(), 1);

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

            cblas_dgemv(CblasColMajor, CblasNoTrans, n, n, alpha, A.data(), n, X.data(), 1, beta, Y.data(), 1);

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

            cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, n, n, n, alpha, A.data(), n, B.data(), n, beta, C.data(), n);

            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            elapsed_time += (duration.count() * 1.e-9);
        }

        avg_time = elapsed_time / static_cast<long double>(ntrials);

        long double dgemm_flops = (2.L * n) / avg_time;
        long double dgemm_mflops = dgemm_flops / 1e6;

        elapsed_time = 0.L;

        std::cout << "Dgemm n: " << n << ", MFlops: " << dgemm_mflops << std::endl;
        if (dgemmOut.is_open()) {
            dgemmOut << n << "," << dgemm_mflops << "\n";
        }

    }

    daxpyOut.close(); dgemvOut.close(); dgemmOut.close();

    return 0;
}