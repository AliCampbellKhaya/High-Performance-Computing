#include <iostream>
#include <complex>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include <chrono>
#include <limits>
#include <fstream>
#include <cblas.h>
#include <lapacke.h>

int main() {
    const double machine_eps = std::numeric_limits<double>::epsilon();

    std::ofstream LapackOut("hw7_lapack.csv");

    if (LapackOut.is_open()) {
        LapackOut << "n,log_residual,log_norm_error\n";
    }

    for (int n = 16; n <= 8192; n *= 2) {

        int ma = n;
        int na = n;

        std::complex<double>* a = (std::complex<double>*)malloc(sizeof(std::complex<double>) * ma * na);
        std::complex<double>* b = (std::complex<double>*)malloc(sizeof(std::complex<double>) * ma);
        std::complex<double>* z = (std::complex<double>*)malloc(sizeof(std::complex<double>) * na);

        std::vector<lapack_int> ipiv(n);

        srand(0);
        int k = 0;

        for (int j = 0; j < na; j++) {
            for (int i = 0; i < ma; i++) {
                a[k] = 0.5 - (double)rand() / (double)RAND_MAX + std::complex<double>(0, 1) * (0.5 - (double)rand() / (double)RAND_MAX);

                if (i == j) {
                    a[k] *= static_cast<double>(ma);
                }

                k++;
            }
        }

        srand(1);

        for (int i = 0; i < ma; i++) {
            b[i] = 0.5 - (double)rand() / (double)RAND_MAX + std::complex<double>(0, 1) * (0.5 - (double)rand() / (double)RAND_MAX);
        }

        std::complex<double>* a2 = (std::complex<double>*)malloc(sizeof(std::complex<double>) * ma * na);
        std::complex<double>* b2 = (std::complex<double>*)malloc(sizeof(std::complex<double>) * ma);
        std::memcpy(a2, a, sizeof(std::complex<double>) * ma * na);
        std::memcpy(b2, b, sizeof(std::complex<double>) * ma);

        lapack_int zgesv = LAPACKE_zgesv(LAPACK_COL_MAJOR, n, 1, (lapack_complex_double*)a, n, ipiv.data(), (lapack_complex_double*)b, n);

        std::memcpy(z, b, sizeof(std::complex<double>) * n);

        double norm_A_inf = LAPACKE_zlange(LAPACK_COL_MAJOR, 'I', n, n, (lapack_complex_double*)a2, n);
        double norm_z_2 = cblas_dznrm2(n, (void*)z, 1);

        std::complex<double> alpha(-1.0, 0.0);
        std::complex<double> beta(1.0, 0.0);

        cblas_zgemv(CblasColMajor, CblasNoTrans, n, n, (void*)&alpha, (void*)a2, n, (void*)z, 1, (void*)&beta, (void*)b2, 1);

        double norm_residual_2 = cblas_dznrm2(n, (void*)b2, 1);
        double log_residual = std::log10(norm_residual_2);

        double norm_error = norm_residual_2 / (norm_A_inf * norm_z_2 * machine_eps);
        double log_norm_error = std::log10(norm_error);

        std::cout << "N: " << n << "Log Residual: " << log_residual << "Log Norm Error " << log_norm_error << std::endl;
        if (LapackOut.is_open()) {
            LapackOut << n << "," << log_residual << "," << log_norm_error << "\n";
        }

        free(a); free(b); free(z); free(a2); free(b2);
    }

    LapackOut.close();
    
    return 0;
}