#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, comm_sz;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    // Cada proceso tiene un valor que sumaremos
    int local_value = rank + 1; // Por ejemplo, el proceso 0 tiene el valor 1, el proceso 1 el valor 2, etc.
    cout << "Proceso " << rank << " tiene el valor local: " << local_value << endl;

    // Suma global en estructura de árbol
    int sum = local_value;
    for (int step = 1; step < comm_sz; step *= 2)
    {
        if (rank % (2 * step) == 0)
        {
            int received_value;
            MPI_Recv(&received_value, 1, MPI_INT, rank + step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += received_value; // Suma el valor recibido
        }
        else if (rank % (2 * step) == step)
        {
            MPI_Send(&sum, 1, MPI_INT, rank - step, 0, MPI_COMM_WORLD); // Envía el valor
        }
    }

    // Solo el proceso 0 imprime el resultado
    if (rank == 0)
    {
        cout << "Suma global final: " << sum << endl;
    }

    MPI_Finalize();
    return 0;
}
