#ifndef MEM_SWAPS_HPP
#define MEM_SWAPS_HPP

#include <vector>
#include <iostream>

void swapRows(std::vector<double> &matrix, int nRows, int nCols, int i, int j) {

    for (int k = 0; k < nCols; k++) {
        double* locRowI = matrix.data() + (k * nRows + i);
        double* locRowJ = matrix.data() + (k * nRows + j);

        double valRowI = locRowI[0];
        double valRowJ = locRowJ[0];

        locRowI[0] = valRowJ;
        locRowJ[0] = valRowI;
    }
}

void swapCols(std::vector<double> &matrix, int nRows, int nCols, int i, int j) {

    double* locColI = matrix.data() + (i * nRows);
    double* locColJ = matrix.data() + (j * nRows);

    std::vector<double> valColI(locColI, locColI + nRows);
    std::vector<double> valColJ(locColJ, locColJ + nRows);

    for (int i = 0; i < nRows; i++) {
        locColI[i] = valColJ[i];
        locColJ[i] = valColI[i];
    }

}

#endif