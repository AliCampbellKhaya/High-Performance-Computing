#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
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

    int ntrial = 5;

    for (int n = 16; n <= 4096; n *= 2) {

        for (int t = 0; t < ntrial; t++) {

            std::pair<int, int> rowIndices = getRandomIndices(n);
            int i = rowIndices.first;
            int j = rowIndices.second;
        }
    }

    return 1;
}