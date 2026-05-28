#include <iostream>
#include <vector>
#include <fstream>
#include <cuda.h>
#include <cuda_runtime.h>

int main() {
    const size_t max_bytes = 2ULL * 1024 * 1024 * 1024;

    std::ofstream H2DOut("hw7_H2D_bandwidth.csv");
    std::ofstream D2HOut("hw7_D2H_bandwidth.csv");

    if (H2DOut.is_open()) {
        H2DOut << "bytes,bandwidth\n";
    }
    if (D2HOut.is_open()) {
        D2HOut << "bytes,bandwidth\n";
    }

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    std::vector<char> host_buffer(max_bytes, 'A');
    char* device_buffer;
    cudaMalloc((void**)&device_buffer, max_bytes);

    for (size_t bytes = 1; bytes <= max_bytes; bytes *= 2) {
        // Host To Device
        cudaEventRecord(start);
        cudaMemcpy(device_buffer, host_buffer.data(), bytes, cudaMemcpyHostToDevice);
        cudaEventRecord(stop);
        cudaEventSynchronize(stop);

        float milliseconds = 0;
        cudaEventElapsedTime(&milliseconds, start, stop);
        float h2d_elapsed_time = (milliseconds * 1.e-3);
        double h2d_bandwidth = static_cast<double>(bytes) / h2d_elapsed_time;

        std::cout << "H2D Bytes: " << bytes << " Bandwidth: " << h2d_bandwidth << std::endl;
        if (H2DOut.is_open()) {
            H2DOut << bytes << "," << h2d_bandwidth << "\n";
        }

        // Device To Host
        cudaEventRecord(start);
        cudaMemcpy(host_buffer.data(), device_buffer, bytes, cudaMemcpyDeviceToHost);
        cudaEventRecord(stop);
        cudaEventSynchronize(stop);

        milliseconds = 0;
        cudaEventElapsedTime(&milliseconds, start, stop);
        float d2h_elapsed_time = (milliseconds * 1.e-3);
        double d2h_bandwidth = static_cast<double>(bytes) / d2h_elapsed_time;

        std::cout << "D2H Bytes: " << bytes << " Bandwidth: " << d2h_bandwidth << std::endl;
        if (D2HOut.is_open()) {
            D2HOut << bytes << "," << d2h_bandwidth << "\n";
        }
    }

    cudaEventDestroy(start);
    cudaEventDestroy(stop);

    cudaFree(device_buffer);

    H2DOut.close();
    D2HOut.close();

    return 0;
}