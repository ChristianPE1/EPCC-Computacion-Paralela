#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Cada proceso tiene un valor único
    int local_value = rank + 1; // Por ejemplo: proceso 0 -> 1, proceso 1 -> 2, ...
    int global_sum = 0;

    if (rank == 0) {
        // Proceso raíz inicia la acumulación
        global_sum = local_value;
        for (int i = 1; i < size; ++i) {
            int received_value;
            MPI_Recv(&received_value, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            global_sum += received_value;
        }
        std::cout << "La suma total es: " << global_sum << std::endl;
    } else {
        // Otros procesos envían su valor al proceso raíz
        MPI_Send(&local_value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
