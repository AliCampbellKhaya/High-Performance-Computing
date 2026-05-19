#ifndef GRAPH_LENGTH_THREADS_HPP
#define GRAPH_LENGTH_THREADS_HPP

#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <vector>
#include <thread>
#include <mutex>

double function(double x) {
    return (1.0 / x) + (x / 4.0);
}

void partial_reimann_sum(double &sum, std::mutex &mtx, double a, double h, int n, int num_threads, int i) {
    double local_sum = 0.0;
    int start = (n / num_threads) * i;
    int end = (i == num_threads - 1) ? n : (n / num_threads) * (i + 1);

    for (int j = start; j < end; j++) {
        double x = a + j * h;
        local_sum += function(x);
    }
    local_sum *= h;

    mtx.lock();
    sum += local_sum;
    mtx.unlock();
}

double parallel_reimann_sum(double a, double b, int n, int num_threads) {
    double h = (b - a) / n;
    double sum = 0.0;
    std::vector<std::thread> threads(num_threads);

    std::mutex mtx;

    for (int i = 0; i < num_threads; i++) {
        threads[i] = std::thread(partial_reimann_sum, std::ref(sum), std::ref(mtx), a, h, n, num_threads, i);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return sum;
}



#endif