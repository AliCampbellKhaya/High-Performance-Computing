// @uw.edu
// AMATH 483-583
// strassen.cpp : starter code for Strassen implementation

#include <iostream>
#include <vector>
#include <cmath>

template <typename T>
std::vector<std::vector<T>> addMatrix(const std::vector<std::vector<T>> &A, const std::vector<std::vector<T>> &B) {
    int n = A.size();
    int m = A[0].size();
    std::vector<std::vector<T>> C(n, std::vector<T>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}

template <typename T>
std::vector<std::vector<T>> subtractMatrix(const std::vector<std::vector<T>> &A, const std::vector<std::vector<T>> &B) {
    int n = A.size();
    int m = A[0].size();
    std::vector<std::vector<T>> C(n, std::vector<T>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}

template <typename T>
std::vector<std::vector<T>> strassenMultiply(const std::vector<std::vector<T>> &A, const std::vector<std::vector<T>> &B) {
    int n = A.size();
    
    if (n == 1) {
        return {{A[0][0] * B[0][0]}};
    }

    int m = n / 2;

    std::vector<std::vector<T>> A11(m, std::vector<T>(m));
    std::vector<std::vector<T>> A12(m, std::vector<T>(m));
    std::vector<std::vector<T>> A21(m, std::vector<T>(m));
    std::vector<std::vector<T>> A22(m, std::vector<T>(m));

    std::vector<std::vector<T>> B11(m, std::vector<T>(m));
    std::vector<std::vector<T>> B12(m, std::vector<T>(m));
    std::vector<std::vector<T>> B21(m, std::vector<T>(m));
    std::vector<std::vector<T>> B22(m, std::vector<T>(m));
    
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j+m];
            A21[i][j] = A[i+m][j];
            A22[i][j] = A[i+m][j+m];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j+m];
            B21[i][j] = B[i+m][j];
            B22[i][j] = B[i+m][j+m];
        }
    }

    std::vector<std::vector<T>> M1 = strassenMultiply(addMatrix(A11, A22), addMatrix(B11, B22));
    std::vector<std::vector<T>> M2 = strassenMultiply(addMatrix(A21, A22), B11);
    std::vector<std::vector<T>> M3 = strassenMultiply(A11, subtractMatrix(B12, B22));
    std::vector<std::vector<T>> M4 = strassenMultiply(A22, subtractMatrix(B21, B11));
    std::vector<std::vector<T>> M5 = strassenMultiply(addMatrix(A11, A12), B22);
    std::vector<std::vector<T>> M6 = strassenMultiply(subtractMatrix(A21, A11), addMatrix(B11, B12));
    std::vector<std::vector<T>> M7 = strassenMultiply(subtractMatrix(A12, A22), addMatrix(B21, B22));

    std::vector<std::vector<T>> C11 = addMatrix(M1, subtractMatrix(M4, addMatrix(M5, M7)));
    std::vector<std::vector<T>> C12 = addMatrix(M3, M5);
    std::vector<std::vector<T>> C21 = addMatrix(M2, M4);
    std::vector<std::vector<T>> C22 = subtractMatrix(M1, addMatrix(M2, addMatrix(M3, M6)));

    std::vector<std::vector<T>> C(n, std::vector<T>(n));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            C[i][j] = C11[i][j];
            C[i][j+m] = C12[i][j];
            C[i+m][j] = C21[i][j];
            C[i+m][j+m] = C22[i][j];
        }
    }
}

template <typename T>
void printMatrix(const std::vector<std::vector<T>> &matrix) {
    int n = matrix.size();
    int m = matrix[0].size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

// int
template std::vector<std::vector<int>> addMatrix<int>(const std::vector<std::vector<int>> &A, const std::vector<std::vector<int>> &B);
template std::vector<std::vector<int>> subtractMatrix<int>(const std::vector<std::vector<int>> &A, const std::vector<std::vector<int>> &B);
template std::vector<std::vector<int>> strassenMultiply<int>(const std::vector<std::vector<int>> &A, const std::vector<std::vector<int>> &B);
template void printMatrix<int>(const std::vector<std::vector<int>> &matrix);
// double
template std::vector<std::vector<double>> addMatrix<double>(const std::vector<std::vector<double>> &A, const std::vector<std::vector<double>> &B);
template std::vector<std::vector<double>> subtractMatrix<double>(const std::vector<std::vector<double>> &A, const std::vector<std::vector<double>> &B);
template std::vector<std::vector<double>> strassenMultiply<double>(const std::vector<std::vector<double>> &A, const std::vector<std::vector<double>> &B);
template void printMatrix<double>(const std::vector<std::vector<double>> &matrix);
