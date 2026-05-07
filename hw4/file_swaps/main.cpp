#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <chrono>
#include <random>
#include <string>
#include "file_swaps.hpp"

int main(){

    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    const int ntrials = 5;

    std::mt19937 gen(0);
    std::uniform_real_distribution<double> dist(1.0, 10.0);

    const std::string filename = "matrix.bin";

    std::ofstream outFile("hw4_fileswaps.csv");

    long double rowSwapElapsedTime = 0.L;
    long double colSwapElapsedTime = 0.L;

    long double avgRowSwapTime;
    long double avgColSwapTime;

    std::cout << "n\t Row Swap Time\t Col SwapTime\n";
    if (outFile.is_open()) {
        outFile << "n, Row Swap Time, Col Swap Time\n";
    }
    

    for (int n = 16; n <= 8192; n *= 2) {

        std::cout << "Test for n: " << n << std::endl;

        std::vector<double> matrix(n * n);

        for (int i = 0; i < n * n; i++) {
            matrix[i] = dist(gen);
        }

        std::fstream file(filename, std::ios::out | std::ios::binary);
        file.write(reinterpret_cast<char*>(&matrix[0]), n * n * sizeof(double));
        file.close();

        for (int t = 0; t < ntrials; t++) {
            std::fstream fileToSwap(filename, std::ios::in | std::ios::out | std::ios::binary);

            // Always swap the first and last columns for testing
            int i = 0; 
            int j = n - 1;

            start = std::chrono::high_resolution_clock::now();
            swapRowsInFile(fileToSwap, n, n, i, j);
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            rowSwapElapsedTime += (duration.count() * 1.e-9);

            start = std::chrono::high_resolution_clock::now();
            swapColsInFile(fileToSwap, n, n, i, j);
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            colSwapElapsedTime += (duration.count() * 1.e-9);
            fileToSwap.close();
        }

        avgRowSwapTime = rowSwapElapsedTime / static_cast<long double>(ntrials);
        avgColSwapTime = colSwapElapsedTime / static_cast<long double>(ntrials);

        rowSwapElapsedTime = 0.L;
        colSwapElapsedTime = 0.L;

        std::cout << "n: " << n << " Row Swap Time: " << avgRowSwapTime << " Col Swap Time: " << avgColSwapTime << std::endl;
        if (outFile.is_open()) {
            outFile << n << "," << avgRowSwapTime << "," << avgColSwapTime << "\n";
        }

        std::remove(filename.c_str());
    }

    outFile.close();
    return 0;
}