#include "graph_length_threads.hpp"

int main(int argc, char* argv[]) {

    std::vector<std::thread> threads;

    const double a = 1.0;
    const double b = 6.0;
    const int n = 1e8;

    for (int num_threads = 1; num_threads <= 16; num_threads *= 2) {
        
        for (int i = 0; i < num_threads; i++) {
            threads.emplace_back(reimann_sum, a, b, n);
        }

        for (auto& thread : threads) {
            thread.join();
        }
    }

    return 0;
}