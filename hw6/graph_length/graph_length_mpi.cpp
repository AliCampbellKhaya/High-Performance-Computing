#include "graph_length_mpi.hpp"

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int ip, np;
    MPI_Comm_rank(MPI_COMM_WORLD, &ip);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    std::ofstream outFileScale;
    std::ofstream outFileError;
    if (ip == 0) {
        outFileScale.open(("hw6_graph_length_mpi_scale.csv"));
        outFileScale << "num_processes,time\n";
        std::cout << "Num Processes \t Time" << std::endl;
        outFileError.open(("hw6_graph_length_mpi_error.csv"));
        outFileError << "partition_points,error\n";
        std::cout << "partition points\t error" << std::endl;
    }

    double a = 1.0;
    double b = 6.0;
    const int n = 1e8;

    double local_a = a + ip * (b - a) / np;
    double local_b = a + (ip + 1) * (b - a) / np;
    int local_n = n / np;

    const double true_length = log(6) + (36.0 / 8.0) - (1 / 8.0);

    MPI_Barrier(MPI_COMM_WORLD);

    double start_time = MPI_Wtime();

    double local_sum = reimann_sum(local_a, local_b, local_n);

    double end_time = MPI_Wtime();
    double elapsed_time = end_time - start_time;
    double max_time = 0.0;

    double global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&elapsed_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (ip == 0) {
        outFileScale << np << "," << max_time << "\n";
        std::cout << "Number of processes: " << np << "time: " << max_time << std::endl;
        outFileScale.close();
    }

    for (int m = 10; m <= 1e6; m*= 10) {

        int local_m = m / np;

        local_sum = reimann_sum(local_a, local_b, local_m);
        
        MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        if (ip == 0) {
            double error = std::abs(global_sum - true_length);
            std::cout << "partition points: " << m << "error: " << error << std::endl;
            outFileError << m << "," << error << "\n";
        }
    }

    if (ip == 0) {
        outFileError.close();
    }

    MPI_Finalize();
    return 0;
}