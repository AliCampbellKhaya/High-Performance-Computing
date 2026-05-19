#include "graph_length_threads.hpp"

int main(int argc, char* argv[]) {

    const double a = 1.0;
    const double b = 6.0;
    const int n = 1e8;

    std::ofstream outFileScale("hw6_graph_length_threads_scale.csv");

    std::cout << "num threads\t time" << std::endl;
    if (outFileScale.is_open()) {
        outFileScale << "num_threads, time\n";
    }

    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    long double elapsed_time = 0.L;

    for (int num_threads = 1; num_threads <= 16; num_threads *= 2) {
        start = std::chrono::high_resolution_clock::now();
        double reimann_sum = parallel_reimann_sum(a, b, n, num_threads);
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        elapsed_time = (duration.count() * 1.e-9);

        std::cout << "num threads: " << num_threads << "time: " << elapsed_time << std::endl;
        if (outFileScale.is_open()) {
            outFileScale << num_threads << "," << elapsed_time << "\n";
        }
    }

    std::ofstream outFileError{"hw6_graph_length_threads_error.csv"};

    std::cout << "partition points\t error" << std::endl;
    if (outFileError.is_open()) {
        outFileError << "partition points, error\n";
    }

    double true_length = 0; //placeholder
    const int m_threads = 1; //placeholder

    for (int m = 10; m <= 1e6; m *= 10) {
        double reimann_sum = parallel_reimann_sum(a, b, m, m_threads);
        double error = abs(reimann_sum - true_length);
        std::cout << "partition points: " << m << "error: " << error << std::endl;
        if (outFileError.is_open()) {
            outFileError << m << "," << error << "\n";
        }
    }

    return 0;
}