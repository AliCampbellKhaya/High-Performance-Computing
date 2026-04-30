#ifndef REF_IO_BANDWIDTH_HPP
#define IO_BANDWIDTH_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

void fileWriteMatrix(std::string& filename, std::vector<double>& matrix, int n) {

    std::fstream file(filename, std::ios::out | std::ios::binary);
    if (file.is_open()) { 
        file.write(reinterpret_cast<char *>(&matrix[0]), sizeof(double) * n * n);
    }
    file.close();
}

void fileReadMatrix(std::string& filename, std::vector<double>& matrix, int n) {

    std::fstream file(filename, std::ios::in | std::ios::binary);
    if (file.is_open()) { 
        file.read(reinterpret_cast<char*>(&matrix[0]), sizeof(double) * n * n);
    }
    file.close();
}

#endif