#include <vector>
#include <stdexcept>

/*
DGEMV performs n * (3 + (2 * m)) floating point operations
Where n is the number of rows in matrix A and m is the number of columns in matrix A
*/
void dgemv(double a, const std::vector<std::vector<double>> &A, const std::vector<double> &x,
            double b, std::vector<double> &y) {

    if (A[0].size() != x.size()) {
        throw std::invalid_argument("Matrix A must have the same number of columns as elements in vector x.");
    }

    if (A.size() != y.size()) {
        throw std::invalid_argument("Matrix Ax must be a vector with the same dimensions as vector y.");
    }

    for (int i = 0; i < A.size(); i++) {
        double sum = 0;

        for (int j = 0; j < A[0].size(); j++) {

            sum += A[i][j] * x[j];

        }

        y[i] = a * sum + b * y[i];
    }
}