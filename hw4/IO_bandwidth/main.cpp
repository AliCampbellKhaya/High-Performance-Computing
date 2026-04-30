#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include "IO_bandwidth.hpp"

int main() {
    printf("N\t\tSize(GB)\tWrite(MB/s)\tRead(MB/s)\n");
    printf("----------------------------------------------------------\n");

    std::ofstream outFile("hw4_iob_results.txt");

    std::cout << "N\t Size (GB)\t Write (MB/s)\t Read (MB/s)" << std::endl;

    if (outFile.is_open()) {
            outFile << "N\t Size (GB)\t Write (MB/s)\t Read (MB/s)" << std::endl;
    }

    for (int N = 32; N <= 16384; N *= 2) {
        size_t num_elements = (size_t)N * N;
        size_t total_bytes = num_elements * sizeof(double);
        double size_gb = total_bytes / (1024.0 * 1024.0 * 1024.0);
        
        std::vector<double> matrix(num_elements, 1.0);
        std::string fname = "test_io.bin";

        auto start_write = std::chrono::high_resolution_clock::now();
        fileWriteMatrix(fname, matrix, N);
        auto end_write = std::chrono::high_resolution_clock::now();
        
        auto start_read = std::chrono::high_resolution_clock::now();
        fileReadMatrix(fname, matrix, N);
        auto end_read = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> diff_w = end_write - start_write;
        std::chrono::duration<double> diff_r = end_read - start_read;

        double write_bandwidth = (total_bytes / (1024.0 * 1024.0)) / diff_w.count();
        double read_bandwidth = (total_bytes / (1024.0 * 1024.0)) / diff_r.count();

        std::cout << N << "\t" << size_gb << "\t" << write_bandwidth << "\t" << read_bandwidth << std::endl;
        if (outFile.is_open()) {
            outFile << N << "\t" << size_gb << "\t" << write_bandwidth << "\t" << read_bandwidth << std::endl;
        }
    }

    outFile.close();

    return 0;
}