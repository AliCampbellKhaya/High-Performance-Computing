#ifndef CBLAS_DGEMV_HPP
#define CBLAS_DGEMV_HPP

#include <cblas.h>

void cblas_dgemv(const enum CBLASORDER order, const enum CBLAS_TRANSPOSE TransA, const int M, const int N, const double alpha, const double *A, 
                    const int lda, const double *X, const int incX, const double beta, double *Y, const int incY) {

}

#endif