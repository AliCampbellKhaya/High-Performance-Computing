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

    for (int n = 16; n <= 8192; n++) {

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

        lapack_int zgesv = LAPACKE_zgesv(LAPACK_COL_MAJOR, n, 1, (lapack_complex_double*)a, n, ipiv.data(), (lapack_complex_double*)b, n);
    }
}