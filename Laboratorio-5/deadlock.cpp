#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int send_value = rank + 1;   // Valor a enviar
    int recv_value;

    // Cada proceso intenta enviar a otro antes de recibir
    int dest = (rank + 1) % size; // Enviar al siguiente proceso
    int src = (rank - 1 + size) % size; // Recibir del proceso anterior

    // Deadlock potencial: todos intentan enviar antes de recibir
    MPI_Send(&send_value, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
    MPI_Recv(&recv_value, 1, MPI_INT, src, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    std::cout << "Proceso " << rank << " recibió " << recv_value << " desde el proceso " << src << std::endl;

    MPI_Finalize();
    return 0;
}
