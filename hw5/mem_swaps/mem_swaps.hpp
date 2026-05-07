#ifndef MEM_SWAPS_HPP
#define MEM_SWAPS_HPP

#include <vector>
#include <iostream>

void swapRows(std::vector<double> &matrix, int nRows, int nCols, int i, int j) {

    for (int k = 0; k < nCols; k++) {
        std::streampos locRowI = (static_cast<std::streamoff>(k) * nRows + i) * sizeof(double);
        std::streampos locRowJ = (static_cast<std::streamoff>(k) * nRows + j) * sizeof(double);

        double valRowI;
        double valRowJ;
    }
}

void swapCols(std::vector<double> &matrix, int nRows, int nCols, int i, int j) {

    std::streampos locColI = (static_cast<std::streamoff>(i) * nRows) * sizeof(double);
    std::streampos locColJ = (static_cast<std::streamoff>(j) * nRows) * sizeof(double);

    std::vector<double> valColI(nRows);
    std::vector<double> valColJ(nRows);
}

#endif