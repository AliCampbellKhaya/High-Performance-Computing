#ifndef MY_BROADCAST_HPP
#define MY_BROADCAST_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <mpi.h>

template <typename T>
void my_broadcast(T* data, int count, int root, MPI_Comm comm) {
    int rank;
    int size;

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    int bytes = count * sizeof(T);
    const int tag = 0;

    if (rank == root) {
        for (int i = 0; i < size; i++) {
            if (i != root) {
                MPI_Send(data, bytes, MPI_BYTE, i, tag, comm);
                std::cout << "Rank " << root << " sent data to Rank " << i << std::endl;
            }
        }
    }
    else {
        MPI_Recv(data, bytes, MPI_BYTE, root, tag, comm, MPI_STATUS_IGNORE);
        std::cout << "Rank " << rank << " received data: ";
    }
}

#endif