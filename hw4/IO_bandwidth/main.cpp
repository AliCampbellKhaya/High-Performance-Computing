#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include "IO_bandwidth.hpp"

int main() {
    std::ofstream outFile("hw4_iob_results.csv");

    std::cout << "Dimension\t Size (GB)\t Write (MB/s)\t Read (MB/s)" << std::endl;

    if (outFile.is_open()) {
            outFile << "Dimension,Size (GB),Write (MB/s),Read (MB/s)\n";
    }

    for (int n = 32; n <= 16384; n *= 2) {
        size_t num_elements = (size_t)n * n;
        size_t total_bytes = num_elements * sizeof(double);
        double size_gb = total_bytes / (1024.0 * 1024.0 * 1024.0);
        
        std::vector<double> matrix(num_elements, 1.0);
        std::string fname = "test_io.bin";

        auto start_write = std::chrono::high_resolution_clock::now();
        fileWriteMatrix(fname, matrix, n);
        auto end_write = std::chrono::high_resolution_clock::now();
        
        auto start_read = std::chrono::high_resolution_clock::now();
        fileReadMatrix(fname, matrix, n);
        auto end_read = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> diff_w = end_write - start_write;
        std::chrono::duration<double> diff_r = end_read - start_read;

        double write_bandwidth = (total_bytes / (1024.0 * 1024.0)) / diff_w.count();
        double read_bandwidth = (total_bytes / (1024.0 * 1024.0)) / diff_r.count();

        std::cout << n << "\t" << size_gb << "\t" << write_bandwidth << "\t" << read_bandwidth << std::endl;
        if (outFile.is_open()) {
            outFile << n << "," << size_gb << "," << write_bandwidth << "," << read_bandwidth << "\n";
        }
    }

    outFile.close();

    return 0;
}