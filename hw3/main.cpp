#include <iostream>
#include <vector>
#include <chrono>

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    long double elapsed_time = 0.L;
    long double avg_time;
    const int ntrials = 5;

    // Level 1 BLAS - DAXPY
    for (int i = 2; i <= 512; i++) {

        for (int t = 0; t < ntrials; t++) {
            start = std::chrono::high_resolution_clock::now();

            //TODO: add daxpy

            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            elapsed_time += (duration.count() * 1.e-9);
        }

        avg_time = elapsed_time / static_cast<long double>(ntrials);

        elapsed_time = 0.L;
    }

    // Level 2 BLAS - DGEMV
    for (int i = 2; i <= 512; i++) {

        for (int t = 0; t < ntrials; t++) {
            start = std::chrono::high_resolution_clock::now();

            //TODO: add dgemv

            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            elapsed_time += (duration.count() * 1.e-9);
        }

        avg_time = elapsed_time / static_cast<long double>(ntrials);

        elapsed_time = 0.L;
    }

    // Level 3 BLAS - DGEMM
    for (int i = 2; i <= 512; i++) {

        for (int t = 0; t < ntrials; t++) {
            start = std::chrono::high_resolution_clock::now();

            //TODO: add dgemm

            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            elapsed_time += (duration.count() * 1.e-9);
        }

        avg_time = elapsed_time / static_cast<long double>(ntrials);

        elapsed_time = 0.L;
    }
}