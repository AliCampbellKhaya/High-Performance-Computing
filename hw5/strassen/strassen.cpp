// @uw.edu
// AMATH 483-583
// strassen.cpp : starter code for Strassen implementation

#include "strassen.hpp" //revert to not using hpp file to match submission requirements.

template <typename T>
std::vector<std::vector<T>> addMatrix(const std::vector<std::vector<T>> &A, const std::vector<std::vector<T>> &B)
{
    int n = A.size();
    int m = A[0].size();
    std::vector<std::vector<T>> C(n, std::vector<T>(m));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}

template <typename T>
std::vector<std::vector<T>> subtractMatrix(const std::vector<std::vector<T>> &A, const std::vector<std::vector<T>> &B)
{
    int n = A.size();
    int m = A[0].size();
    std::vector<std::vector<T>> C(n, std::vector<T>(m));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}

template <typename T>
std::vector<std::vector<T>> strassenMultiply(const std::vector<std::vector<T>> &A, const std::vector<std::vector<T>> &B)
{
// write this code
}

template <typename T>
void printMatrix(const std::vector<std::vector<T>> &matrix)
{
    int n = matrix.size();
    int m = matrix[0].size();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
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
