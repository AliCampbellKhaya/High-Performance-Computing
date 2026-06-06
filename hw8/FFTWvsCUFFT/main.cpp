#include <iostream>
#include <fstream>
#include <chrono> 
#include <cmath>
#include <complex>
#include <random>
#include <cstring>

#include <fftw3.h>
#include <cuda_runtime.h>
#include <cufft.h>

__global__ void spectral_gradient_kernel(const cufftDoubleComplex* psi, cufftDoubleComplex* gx, cufftDoubleComplex* gy, cufftDoubleComplex* gz, int n);

int main() {
    auto cpu_start = std::chrono::high_resolution_clock::now();
    auto cpu_stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(cpu_stop - cpu_start);
    const int ntrials = 5;
    long double elapsed_time = 0.L;
    long double avg_time;

    fftw_init_threads();
    fftw_plan_with_nthreads(4);

    std::mt19937 gen(0);
    std::uniform_real_distribution<double> dist(1.0, 10.0);

    std::ofstream FFTWOut("hw8_FFTW.csv");
    std::ofstream CUFFTOut("hw7_CUFFT.csv");

    if (FFTWOut.is_open()) {
            FFTWOut << "n,flops\n";
    }
    if (CUFFTOut.is_open()) {
            CUFFTOut << "n,flops\n";
    }

    for (int n = 16; n <= 256; n *= 2) {
        long long n_cubed = (long long) n * n * n;
        double total_flops = (double) n_cubed * (60.0 * std::log2(n) + 6.0);

        fftw_complex *w_psi_base = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n_cubed);
        fftw_complex *w_psi = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n_cubed);
        fftw_complex *w_gx = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n_cubed);
        fftw_complex *w_gy = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n_cubed);
        fftw_complex *w_gz = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n_cubed);

        for (long long i = 0; i < n_cubed; i++) {
            w_psi_base[i][0] = dist(gen);
            w_psi_base[i][1] = dist(gen);
        }

        fftw_plan w_fwd = fftw_plan_dft_3d(n, n, n, w_psi, w_psi, FFTW_FORWARD, FFTW_MEASURE);
        fftw_plan w_inv_x = fftw_plan_dft_3d(n, n, n, w_gx, w_gx, FFTW_BACKWARD, FFTW_MEASURE);
        fftw_plan w_inv_y = fftw_plan_dft_3d(n, n, n, w_gy, w_gy, FFTW_BACKWARD, FFTW_MEASURE);
        fftw_plan w_inv_z = fftw_plan_dft_3d(n, n, n, w_gz, w_gz, FFTW_BACKWARD, FFTW_MEASURE);

        for (int t = 0; t < ntrials; t++) {
            std::memcpy(w_psi, w_psi_base, sizeof(fftw_complex) * n_cubed);

            cpu_start = std::chrono::high_resolution_clock::now();

            fftw_execute(w_fwd);

            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    for (int k = 0; k < n; ++k) {
                        long long idx = (long long)i*n*n + j*n + k;
                        double kx = 2.0 * M_PI * ((i < (n + 1) / 2) ? i : (i - n));
                        double ky = 2.0 * M_PI * ((j < (n + 1) / 2) ? j : (j - n));
                        double kz = 2.0 * M_PI * ((k < (n + 1) / 2) ? k : (k - n));
                        
                        w_gx[idx][0] = -kx * w_psi[idx][1]; w_gx[idx][1] = kx * w_psi[idx][0];
                        w_gy[idx][0] = -ky * w_psi[idx][1]; w_gy[idx][1] = ky * w_psi[idx][0];
                        w_gz[idx][0] = -kz * w_psi[idx][1]; w_gz[idx][1] = kz * w_psi[idx][0];
                    }
                }
            }


            fftw_execute(w_inv_x);
            fftw_execute(w_inv_y);
            fftw_execute(w_inv_z);

            cpu_stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(cpu_stop - cpu_start);
            elapsed_time += (duration.count() * 1.e-9);
        }

        avg_time = elapsed_time / ntrials;
        double fftw_gflops = (total_flops / avg_time) / 1e9;

        elapsed_time = 0.L;

        std::cout << "FFTW n: " << n << ", GFlops: " << fftw_gflops << std::endl;
        if (FFTWOut.is_open()) {
            FFTWOut << n << "," << fftw_gflops << "\n";
        }

        fftw_destroy_plan(w_fwd); fftw_destroy_plan(w_inv_x); fftw_destroy_plan(w_inv_y); fftw_destroy_plan(w_inv_z);
        fftw_free(w_gx); fftw_free(w_gy); fftw_free(w_gz);

        cufftDoubleComplex *c_psi_base, *c_psi, *c_gx, *c_gy, *c_gz;
        cudaMalloc((void**)&c_psi_base, sizeof(cufftDoubleComplex) * n_cubed);
        cudaMalloc((void**)&c_psi, sizeof(cufftDoubleComplex) * n_cubed);
        cudaMalloc((void**)&c_gx, sizeof(cufftDoubleComplex) * n_cubed);
        cudaMalloc((void**)&c_gy, sizeof(cufftDoubleComplex) * n_cubed);
        cudaMalloc((void**)&c_gz, sizeof(cufftDoubleComplex) * n_cubed);

        cudaMemcpy(c_psi_base, w_psi_base, sizeof(cufftDoubleComplex) * n_cubed, cudaMemcpyHostToDevice);

        cufftHandle plan_fwd, plan_inv;
        cufftPlan3d(&plan_fwd, n, n, n, CUFFT_Z2Z);
        cufftPlan3d(&plan_inv, n, n, n, CUFFT_Z2Z);

        int block_threads = 256;
        int block_grid = (n_cubed + block_threads - 1) / block_threads;

        cudaEvent_t gpu_start, gpu_stop;
        cudaEventCreate(&gpu_start);
        cudaEventCreate(&gpu_stop);

        for (int t = 0; t < ntrials; t++) {
            cudaMemcpy(c_psi, c_psi_base, sizeof(cufftDoubleComplex) * n_cubed, cudaMemcpyDeviceToDevice);

            cudaEventRecord(gpu_start);

            cufftExecZ2Z(plan_fwd, c_psi, c_psi, CUFFT_FORWARD);

            spectral_gradient_kernel<<<block_grid, block_threads>>>(c_psi, c_gx, c_gy, c_gz, n);

            cufftExecZ2Z(plan_inv, c_gx, c_gx, CUFFT_INVERSE);
            cufftExecZ2Z(plan_inv, c_gy, c_gy, CUFFT_INVERSE);
            cufftExecZ2Z(plan_inv, c_gz, c_gz, CUFFT_INVERSE);

            cudaEventRecord(gpu_stop);
            cudaEventSynchronize(gpu_stop);

            float milliseconds = 0;
            cudaEventElapsedTime(&milliseconds, gpu_start, gpu_stop);
            elapsed_time += (milliseconds * 1.e-3);
        }

        avg_time = elapsed_time / ntrials;
        double cufft_gflops = (total_flops / avg_time) / 1e9;

        elapsed_time = 0.L;

        std::cout << "CUFFT n: " << n << ", GFlops: " << cufft_gflops << std::endl;
        if (CUFFTOut.is_open()) {
            CUFFTOut << n << "," << cufft_gflops << "\n";
        }

        cufftDestroy(plan_fwd); cufftDestroy(plan_inv);
        cudaEventDestroy(gpu_start); cudaEventDestroy(gpu_stop);
        cudaFree(c_psi); cudaFree(c_gx); cudaFree(c_gy); cudaFree(c_gz); fftw_free(w_psi); 
        cudaFree(c_psi_base); fftw_free(w_psi_base);
    }

    return 0;
}

__global__ void spectral_gradient_kernel(const cufftDoubleComplex* psi, cufftDoubleComplex* gx, cufftDoubleComplex* gy, cufftDoubleComplex* gz, int n) {
    long long idx = (long long)blockIdx.x * blockDim.x + threadIdx.x;
    long long n_cubed = (long long)n * n * n;

    if (idx < n_cubed) {
        int i = idx / (n * n);
        int j = (idx % (n * n)) / n;
        int k = idx % n;

        double kx = 2.0 * M_PI * ((i < (n + 1) / 2) ? i : (i - n));
        double ky = 2.0 * M_PI * ((j < (n + 1) / 2) ? j : (j - n));
        double kz = 2.0 * M_PI * ((k < (n + 1) / 2) ? k : (k - n));

        gx[idx].x = -kx * psi[idx].y;  gx[idx].y = kx * psi[idx].x;
        gy[idx].x = -ky * psi[idx].y;  gy[idx].y = ky * psi[idx].x;
        gz[idx].x = -kz * psi[idx].y;  gz[idx].y = kz * psi[idx].x;
    }
}
