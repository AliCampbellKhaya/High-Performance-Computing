#include "my_broadcast.hpp"

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int start_bytes = 8;
    const int max_bytes = 256 * 1024 * 1024;
    const int root = 0;

    std::ofstream outFile;
    if (rank == root) {
        outFile.open("hw6_broadcast.csv");
        outFile << "message_size_bytes,my_broadcast_time,mpi_broadcast_time";
        std::cout << "Message Size Bytes \t My Broadcast Time \t MPI Broadcast Time";
    }

    std::vector<char> buffer(max_bytes, 'A');

    for (int bytes = start_bytes; bytes <= max_bytes; bytes *= 2) {
        MPI_Barrier(MPI_COMM_WORLD);

        double start_my = MPI_Wtime();

        my_broadcast(buffer.data(), bytes, root, MPI_COMM_WORLD);

        double end_my = MPI_Wtime();
        double elapsed_time_my = end_my - start_my;
        double max_time_my = 0.0;

        MPI_Reduce(&elapsed_time_my, &max_time_my, 1, MPI_DOUBLE, MPI_MAX, root, MPI_COMM_WORLD);

        MPI_Barrier(MPI_COMM_WORLD);

        double start_mpi = MPI_Wtime();

        MPI_Bcast(buffer.data(), bytes, MPI_BYTE, root, MPI_COMM_WORLD);

        double end_mpi = MPI_Wtime();
        double elapsed_time_mpi = end_mpi - start_mpi;
        double max_time_mpi = 0.0;

        MPI_Reduce(&elapsed_time_mpi, &max_time_mpi, 1, MPI_DOUBLE, MPI_MAX, root, MPI_COMM_WORLD);

        if (rank == root) {
            std::cout << "Bytes: " << bytes << "My time: " << max_time_my << "MPI time: " << max_time_mpi << std::endl;
            outFile << bytes << "," << max_time_my << "," << max_time_mpi << "\n";
        }

    }

    if (rank == root) {
        outFile.close();
    }

    MPI_Finalize();
    return 0;
}