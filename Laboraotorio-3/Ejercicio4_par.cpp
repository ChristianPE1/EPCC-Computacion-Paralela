#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int comm_sz, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int local_sum = my_rank + 1;
    int global_sum = 0;

    // Enfoque de suma global usando mariposa para potencias de dos
    for (int step = 1; step < comm_sz; step *= 2)
    {
        int partner = my_rank ^ step; // Usar XOR para encontrar el compañero

        if (partner < comm_sz)
        {
            if (my_rank < partner)
            {
                // Recibir la suma del compañero
                int partner_sum;
                MPI_Recv(&partner_sum, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                local_sum += partner_sum; // Sumar al total
            }
            else
            {
                // Enviar la suma local al compañero
                MPI_Send(&local_sum, 1, MPI_INT, partner, 0, MPI_COMM_WORLD); // Agregar el comunicador aquí
            }
        }
    }

    // Al finalizar, el proceso 0 tendrá la suma global
    if (my_rank == 0)
    {
        global_sum = local_sum; // El proceso 0 tiene la suma global
        cout << "Suma global: " << global_sum << endl;
    }

    MPI_Finalize();
    return 0;
}
