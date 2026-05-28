#include <iostream>
#include <fstream>
#include <chrono>
#include <cmath>
#include <vector>
#include <random>
#include <cblas.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include <cublas_v2.h>

int main() {
    auto cpu_start = std::chrono::high_resolution_clock::now();
    auto cpu_stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(cpu_stop - cpu_start);
    long double elapsed_time = 0.L;
    long double avg_time;
    const int ntrials = 5;

    const double alpha = 2.5;
    const double beta = 2.5;

    std::mt19937 gen(0);
    std::uniform_real_distribution<double> dist(1.0, 10.0);

    std::ofstream OpenBLASOut("hw7_OpenBLAS.csv");
    std::ofstream CUBLASOut("hw7_CUBLAS.csv");

    if (OpenBLASOut.is_open()) {
            OpenBLASOut << "n,mflops\n";
    }
    if (CUBLASOut.is_open()) {
            CUBLASOut << "n,mflops\n";
    }

    for (int n = 2; n <= 16384; n *= 2) {
        size_t matrix_size = static_cast<size_t>(n) * n;

        std::vector<double> A(matrix_size); 
        std::vector<double> B(matrix_size); 
        std::vector<double> C(matrix_size);

        for (int i = 0; i < n * n; i++) {
            A[i] = dist(gen);
            B[i] = dist(gen);
            C[i] = dist(gen);
        }

        for (int t = 0; t < ntrials; t++) {
            cpu_start = std::chrono::high_resolution_clock::now();

            cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, n, n, n, alpha, A.data(), n, B.data(), n, beta, C.data(), n);

            cpu_stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(cpu_stop - cpu_start);
            elapsed_time += (duration.count() * 1.e-9);
        }

        avg_time = elapsed_time / static_cast<long double>(ntrials);

        long double openblas_flops = (2.L * n * n * n) / avg_time;
        long double openblas_mflops = openblas_flops / 1e6;

        elapsed_time = 0.L;

        std::cout << "OpenBLAS n: " << n << ", MFlops: " << openblas_mflops << std::endl;
        if (OpenBLASOut.is_open()) {
            OpenBLASOut << n << "," << openblas_mflops << "\n";
        }

        double *d_A, *d_B, *d_C;
        
        cudaMalloc((void**)&d_A, matrix_size * sizeof(double));
        cudaMalloc((void**)&d_B, matrix_size * sizeof(double));
        cudaMalloc((void**)&d_C, matrix_size * sizeof(double));

        cudaMemcpy(d_A, A.data(), matrix_size * sizeof(double), cudaMemcpyHostToDevice);
        cudaMemcpy(d_B, B.data(), matrix_size * sizeof(double), cudaMemcpyHostToDevice);
        cudaMemcpy(d_C, C.data(), matrix_size * sizeof(double), cudaMemcpyHostToDevice);

        cublasHandle_t handle;
        cublasCreate(&handle);

        cudaEvent_t gpu_start, gpu_stop;
        cudaEventCreate(&gpu_start);
        cudaEventCreate(&gpu_stop);

        for (int t = 0; t < ntrials; t++) {
            cudaEventRecord(gpu_start);
            cublasDgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N, n, n, n, &alpha, d_A, n, d_B, n, &beta, d_C, n);
            cudaEventRecord(gpu_stop);
            cudaEventSynchronize(gpu_stop);

            float milliseconds = 0;
            cudaEventElapsedTime(&milliseconds, gpu_start, gpu_stop);
            elapsed_time += (milliseconds * 1.e-3);
        }

        avg_time = elapsed_time / static_cast<long double>(ntrials);

        long double cublas_flops = (2.L * n * n * n) / avg_time;
        long double cublas_mflops = cublas_flops / 1e6;

        elapsed_time = 0.L;

        std::cout << "CUBLAS n: " << n << ", MFlops: " << cublas_mflops << std::endl;
        if (CUBLASOut.is_open()) {
            CUBLASOut << n << "," << cublas_mflops << "\n";
        }

        cublasDestroy(handle);
        cudaEventDestroy(gpu_start);
        cudaEventDestroy(gpu_stop);
        cudaFree(d_A);
        cudaFree(d_B);
        cudaFree(d_C);
    }

    OpenBLASOut.close();
    CUBLASOut.close();

    return 0;
}