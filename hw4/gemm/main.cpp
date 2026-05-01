#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>
#include "gemm.hpp"

template <typename T>
void test(std::fstream &outFile) {
    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    long double kij_elapsed_time = 0.L;
    long double kij_avg_time;
    long double kij_flops;
    long double jki_elapsed_time = 0.L;
    long double jki_avg_time;
    long double jki_flops;
    const int ntrials = 5;

    T a = 2.5;
    T b = 2.5;

    std::mt19937 gen(0);
    std::uniform_real_distribution<T> dist(1.0, 10.0);

    std::cout << "gemm \n";
    if (outFile.is_open()) {
            outFile << "gemm\n";
            outFile << "order,dimension,flops";
    }

    for (int n = 2; n <= 512; n++) {

        std::vector<T> A(n * n);
        std::vector<T> B(n * n);
        std::vector<T> C(n * n);

        for (int i = 0; i < n * n; i++) {
            A[i] = dist(gen);
            B[i] = dist(gen);
            C[i] = dist(gen);
        }

        for (int t = 0; t < ntrials; t++) {
            start = std::chrono::high_resolution_clock::now();

            try {
                mm_kij(a, A, B, b, C, n, n ,n);
            }
            catch (const std::invalid_argument &e) {
                std::cerr << "Error: " << e.what() << "\n\n";
            }

            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            kij_elapsed_time += (duration.count() * 1.e-9);

            try {
                mm_jki(a, A, B, b, C, n, n, n);
            }
            catch (const std::invalid_argument &e) {
                std::cerr << "Error: " << e.what() << "\n\n";
            }

            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            jki_elapsed_time += (duration.count() * 1.e-9);
        }

        kij_avg_time = kij_elapsed_time / static_cast<long double>(ntrials);

        kij_flops = (n * (n * (3 +(2 * n)))) / kij_avg_time;

        kij_elapsed_time = 0.L;
        std::cout << "(kij) n: " << n << ", Flops: " << kij_flops << std::endl;

        jki_avg_time = jki_elapsed_time / static_cast<long double>(ntrials);

        jki_flops = (n * (n * (3 +(2 * n)))) / jki_avg_time;

        jki_elapsed_time = 0.L;
        std::cout << "(jki) n: " << n << ", Flops: " << jki_flops << std::endl;

        if (outFile.is_open()) {
            outFile << "kij," << n << "," << kij_flops << "\n";
            outFile << "jki," << n << "," << jki_flops << "\n";
        }
    }

    
}

int main() {
    std::fstream outFile("hw4_gemm_results_O3.csv");;
    test<float>(outFile);
    test<double>(outFile);
    outFile.close();
    return 0;
}