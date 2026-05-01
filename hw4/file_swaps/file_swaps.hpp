#ifndef FILE_SWAPS_HPP
#define FILE_SWAPS_HPP

#include <fstream>
#include <vector>

void swapRowsInFile(std::fstream &file, int nRows, int nCols, int i, int j){

    for (int k = 0; k < nCols; k++) {
        std::streampos locRowI = (static_cast<std::streamoff>(k) * nRows + i) * sizeof(double);
        std::streampos locRowJ = (static_cast<std::streamoff>(k) * nRows + j) * sizeof(double);

        double valRowI;
        double valRowJ;

        file.seekg(locRowI);
        file.read(reinterpret_cast<char*>(&valRowI), sizeof(double));

        file.seekg(locRowJ);
        file.read(reinterpret_cast<char*>(&valRowJ), sizeof(double));

        file.clear();

        file.seekp(locRowI);
        file.write(reinterpret_cast<char*>(&valRowJ), sizeof(double));

        file.seekp(locRowJ);
        file.write(reinterpret_cast<char*>(&valRowI), sizeof(double));
    }
    file.flush();
}

void swapColsInFile(std::fstream &file, int nRows, int nCols, int i, int j){

    std::streampos locColI = (static_cast<std::streamoff>(i) * nRows) * sizeof(double);
    std::streampos locColJ = (static_cast<std::streamoff>(j) * nRows) * sizeof(double);

    std::vector<double> valColI(nRows);
    std::vector<double> valColJ(nRows);

    file.seekg(locColI);
    file.read(reinterpret_cast<char*>(valColI.data()), nRows * sizeof(double));

    file.seekg(locColJ);
    file.read(reinterpret_cast<char*>(valColJ.data()), nRows * sizeof(double));

    file.clear();

    file.seekp(locColI);
    file.write(reinterpret_cast<char*>(valColJ.data()), nRows * sizeof(double));

    file.seekp(locColJ);
    file.write(reinterpret_cast<char*>(valColI.data()), nRows * sizeof(double));

    file.flush();

}

#endif