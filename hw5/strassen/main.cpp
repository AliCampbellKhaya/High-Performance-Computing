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
#include "strassen.cpp"

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

    std::ofstream outFile("hw5_strassen.csv");

    long double elapsedTime = 0.L;

    long double avgMultTime;

    std::cout << "n\t Mult Time";
    if (outFile.is_open()) {
        outFile << "n, Mult Time";
    }

    for (int n = 2; n <= 512; n *= 2) {

        std::cout << "Test for n: " << n << std::endl;

        int padded_size = std::pow(2, std::ceil(std::log2(n)));

        std::vector<std::vector<double>> matrixA(padded_size, std::vector<double>(padded_size));
        std::vector<std::vector<double>> matrixB(padded_size, std::vector<double>(padded_size));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                matrixA[i][j] = dist(gen);
                matrixB[i][j] = dist(gen);
            }
        }

        for (int t = 0; t < ntrials; t++) {
            start = std::chrono::high_resolution_clock::now();
            strassenMultiply(matrixA, matrixB);
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            elapsedTime += (duration.count() * 1.e-9);
        }

        avgMultTime = elapsedTime / static_cast<long double>(ntrials);

        elapsedTime = 0.L;

        std::cout << "n: " << n << " Mult Time: " << avgMultTime << std::endl;
        if (outFile.is_open()) {
            outFile << n << "," << avgMultTime << "\n";
        }

    }

    outFile.close();
    return 0;
}