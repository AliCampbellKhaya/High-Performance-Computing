#include <vector>
#include <stdexcept>

void dgemv(double a, const std::vector<std::vector<double>> &A, const std::vector<double> &x,
            double b, std::vector<double> &y) {

    if (A.size() != x.size()) {
        throw std::invalid_argument("Matrix A must have the same number of rows as elements in vector x.");
    }

    if (x.size() != y.size()) {
        throw std::invalid_argument("Matrix Ax must be a vector with the same dimensions as vector y.");
    }

    for (int i = 0; i < A.size(); i++) {
        double sum = 0;

        for (int j = 0; j < A[i].size(); j++) {

            sum += A[i][j] * x[j];

        }

        y[i] = a * sum + b * y[i];
    }
}