#ifndef MEM_SWAPS_HPP
#define MEM_SWAPS_HPP

#include <vector>
#include <algorithm>

void swapRows(std::vector<double> &matrix, int nRows, int nCols, int i, int j) {

    for (int k = 0; k < nCols; k++) {
        std::swap(matrix[k * nRows +i], matrix[k * nRows + j]);
    }
}

void swapCols(std::vector<double> &matrix, int nRows, int nCols, int i, int j) {

    double* locColI = matrix.data() + (i * nRows);
    double* locColJ = matrix.data() + (j * nRows);

    std::swap_ranges(locColI, locColI + nRows, locColJ);

}

#endif