#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <thread>
#include <mutex>

void reimann_sum(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.0;
    double x;

    for (int i = 0; i < n; i++) {
        x = a + i * h;
        sum += function(x) * h;
    }
}

double function(double x) {
    return log(x) - 1/8 * pow(x, 2);
}