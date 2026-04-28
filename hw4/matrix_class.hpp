// AMATH 483-583 row major Matrix class template starter
// write the methods for:
// transpose
// infinityNorm
// operator*
// operator+

#ifndef MATRIX_CLASS_HPP
#define MATRIX_CLASS_HPP

#include <vector>
#include <stdexcept>
#include <iostream>
#include <cmath>

template <typename T>
class Matrix
{
public:
    Matrix(int numRows, int numCols)
        : num_rows(numRows), num_cols(numCols), data(numRows * numCols) {}

    T &operator()(int i, int j)
    {
        return data[i * num_cols + j];
    }

    const T &operator()(int i, int j) const
    {
        return data[i * num_cols + j];
    }

    Matrix<T> operator*(const Matrix<T> &other) const
    {
        // write your code

        // temporary line so the starter compiles before implementation
        throw std::logic_error("operator* not implemented");
    }

    Matrix<T> operator+(const Matrix<T> &other) const;

    Matrix<T> transpose() const
    {
        // write your code

        // temporary line so the starter compiles before implementation
        throw std::logic_error("transpose not implemented");
    }

    int numRows() const
    {
        return num_rows;
    }

    int numCols() const
    {
        return num_cols;
    }

    T infinityNorm() const
    {
        T norm = 0;

        // write your code

        return norm;
    }

    void print() const
    {
        for (int i = 0; i < num_rows; i++)
        {
            for (int j = 0; j < num_cols; j++)
            {
                std::cout << (*this)(i, j) << " ";
            }

            std::cout << "\n";
        }
    }

private:
    int num_rows;
    int num_cols;
    std::vector<T> data;
};

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &other) const
{
    // check for errors in Matrix dimensions

    Matrix<T> result(num_rows, num_cols);

    // write your code

    return result;
}

#endif // MATRIX_CLASS_HPP

