#include <iostream>
#include <cmath>
#include <thread>
#include <mutex>
#include <mpi.h>

/*
f(x) = ln(x) - 1/8 * x^2 on [1,6]
L = int_a^b sqrt(1 + (f'(x))^2) dx
*/

double reimann_sum(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.0;
    double x;

    for (int i = 0; i < n; i++) {
        x = a + i * h;
        sum += function(x) * h;
    }

    return sum;
}

double function(double x) {
    return log(x) - 1/8 * pow(x, 2);
}