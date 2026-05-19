#ifndef GRAPH_LENGTH_MPI_HPP
#define GRAPH_LENGTH_MPI_HPP

#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <mpi.h>

/*
f(x) = ln(x) - 1/8 * x^2 on [1,6]
L = int_a^b sqrt(1 + (f'(x))^2) dx
*/

double function(double x) {
    return (1.0 / x) + (x / 4.0);
}

double reimann_sum(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.0;

    for (int i = 0; i < n; i++) {
        double x = a + i * h;
        sum += function(x) * h;
    }

    return sum;
}


#endif