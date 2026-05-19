#ifndef BROADCAST_HPP
#define BROADCAST_HPP

#include <mpi.h>

// Change all boradcast to my_broadcast for submit
template <typename T>
void broadcast(T* data, int count, int root, MPI_Comm comm)

#endif