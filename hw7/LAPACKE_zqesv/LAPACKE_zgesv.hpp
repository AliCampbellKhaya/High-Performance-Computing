#include <lapacke.h>

lapack_int LAPACKE_zgesv(int matrix_order, lapack_int n, lapack_int nrhs, lapack_complex_double* a, lapack_int lda, lapack_int* ipiv,
                            lapack_complex_double* b, lapack_int ldb);