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
#include "mem_swaps.hpp"

std::pair<int, int> getRandomIndices(int n) {
    int i = std::rand() % n;
    int j = std::rand() % (n-1);

    if (j >= 1) {
        j++;
    }

    return std::make_pair(i, j);
}

int main() {

    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    const int ntrials = 5;

    std::mt19937 gen(0);
    std::uniform_real_distribution<double> dist(1.0, 10.0);

    std::ofstream outFile("hw5_fileswaps.csv");

    long double rowSwapElapsedTime = 0.L;
    long double colSwapElapsedTime = 0.L;

    long double avgRowSwapTime;
    long double avgColSwapTime;

    std::cout << "n\t Row Swap Time\t Col SwapTime\n";
    if (outFile.is_open()) {
        outFile << "n, Row Swap Time, Col Swap Time\n";
    }

    for (int n = 16; n <= 4096; n *= 2) {

        std::cout << "Test for n: " << n << std::endl;

        std::vector<double> matrix(n * n);
        for (int i = 0; i < n * n; i++) {
            matrix[i] = dist(gen);
        }

        for (int t = 0; t < ntrials; t++) {
            std::pair<int, int> rowIndices = getRandomIndices(n);
            int i = rowIndices.first;
            int j = rowIndices.second;

            start = std::chrono::high_resolution_clock::now();
            swapRows(matrix, n, n, i, j);
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            rowSwapElapsedTime += (duration.count() * 1.e-9);

            std::pair<int, int> colIndices = getRandomIndices(n);
            int i = colIndices.first;
            int j = colIndices.second;

            start = std::chrono::high_resolution_clock::now();
            swapCols(matrix, n, n, i, j);
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            colSwapElapsedTime += (duration.count() * 1.e-9);
        }

        avgRowSwapTime = rowSwapElapsedTime / static_cast<long double>(ntrials);
        avgColSwapTime = colSwapElapsedTime / static_cast<long double>(ntrials);

        rowSwapElapsedTime = 0.L;
        colSwapElapsedTime = 0.L;

        std::cout << "n: " << n << " Row Swap Time: " << avgRowSwapTime << " Col Swap Time: " << avgColSwapTime << std::endl;
        if (outFile.is_open()) {
            outFile << n << "," << avgRowSwapTime << "," << avgColSwapTime << "\n";
        }

    }

    outFile.close();
    return 0;
}