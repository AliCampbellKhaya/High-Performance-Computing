#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <chrono>
#include "file_swaps.hpp"

#include <string>

int main (int argc, char* argv[]){

    int numRows, numCols = argc;
    std::string filename = argv[1];

    int i = 0; int j = 0; //temporary for compilatiom
    

    // Generate the matrix
    std::vector<double> matrix(numRows * numCols);
    // init matrix elements in column major order
    // write the matrix to a file
    std::fstream file(filename, std::ios::out | std::ios::binary );
    file.write(reinterpret_cast<char*>(& matrix[0]), numRows * numCols * sizeof(double));
    file.close();
    // Open the file in read - write mode for swapping
    std::fstream fileToSwap(filename, std::ios::in | std::ios::out | std::ios::binary );
    // Get random indices i and j for row swapping
    // Measure the time required for row swapping using file I/O
    auto startTime = std::chrono::high_resolution_clock::now();
    // Swap rows i and j in the file version of the matrix
    swapRowsInFile(fileToSwap, numRows, numCols, i, j);
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = endTime - startTime;
    // Close the file after swapping
    fileToSwap.close();
    // ...
    // after each problem size delete the test file
    std::remove(filename.c_str());
    // ...
}