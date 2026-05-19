#include "graph_length_mpi.hpp"

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int ip, np;
    MPI_Comm_rank(MPI_COMM_WORLD, &ip);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    double a = 1.0;
    double b = 6.0;
    int n = 1; // placeholder

    double local_a = a + ip * (b - a) / np;
    double local_b = a + (ip + 1) * (b - a) / np;
    int local_n = n / np;

    double local_sum = reimann_sum(local_a, local_b, local_n);

    double global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (ip == 0) {
        std::cout << "Result: " << a << "to" << b << "=" << global_sum << std::endl;
    }

    MPI_Finalize();
    return 0;
}