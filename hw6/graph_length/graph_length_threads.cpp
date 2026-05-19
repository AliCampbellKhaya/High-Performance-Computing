#include "graph_length_threads.hpp"

int main(int argc, char* argv[]) {

    const double a = 1.0;
    const double b = 6.0;
    const int n = 1e8;

    std::ofstream outFileScale("hw6_graph_length_threads_scale.csv");

    std::cout << "num_threads\t time";
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

    return 0;
}