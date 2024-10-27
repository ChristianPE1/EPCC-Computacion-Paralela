#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int comm_sz, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int local_sum = my_rank + 1; // Suma local de cada proceso
    int global_sum = 0;

    // Enfoque de suma global usando mariposa para cualquier número de procesos
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
                local_sum += partner_sum;
            }
            else
            {
                // Enviar la suma local al companero
                MPI_Send(&local_sum, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);
            }
        }
        else if (my_rank > partner)
        {
            // Si no hay compañero, sumar la suma local
            local_sum += local_sum;
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
