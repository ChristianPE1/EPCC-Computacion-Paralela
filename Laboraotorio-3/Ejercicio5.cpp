#include <iostream>
#include <mpi.h>
#include <ctime>

using namespace std;

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int my_rank, comm_sz;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    const int NUM_PINGS = 1000;
    double start_time, end_time;

    // Asegúrate de que solo dos procesos estén participando
    if (comm_sz != 2)
    {
        if (my_rank == 0)
        {
            cout << "Este programa debe ejecutarse con exactamente 2 procesos." << endl;
        }
        MPI_Finalize();
        return 1;
    }

    // Medición usando clock()
    clock_t start_clock = clock(); // Inicio del tiempo de CPU
    for (int i = 0; i < NUM_PINGS; i++)
    {
        if (my_rank == 0)
        {
            MPI_Send(nullptr, 0, MPI_BYTE, 1, 0, MPI_COMM_WORLD);                    // Ping
            MPI_Recv(nullptr, 0, MPI_BYTE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Pong
        }
        else
        {
            MPI_Recv(nullptr, 0, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Pong
            MPI_Send(nullptr, 0, MPI_BYTE, 0, 0, MPI_COMM_WORLD);                    // Ping
        }
    }
    clock_t end_clock = clock(); // Fin del tiempo de CPU

    // Tiempo total con clock()
    double cpu_time_used = double(end_clock - start_clock) / CLOCKS_PER_SEC;

    // Medición usando MPI_Wtime
    start_time = MPI_Wtime(); // Inicio del tiempo MPI
    for (int i = 0; i < NUM_PINGS; i++)
    {
        if (my_rank == 0)
        {
            MPI_Send(nullptr, 0, MPI_BYTE, 1, 0, MPI_COMM_WORLD);                    // Ping
            MPI_Recv(nullptr, 0, MPI_BYTE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Pong
        }
        else
        {
            MPI_Recv(nullptr, 0, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Pong
            MPI_Send(nullptr, 0, MPI_BYTE, 0, 0, MPI_COMM_WORLD);                    // Ping
        }
    }
    end_time = MPI_Wtime(); // Fin del tiempo MPI

    // Tiempo total con MPI_Wtime()
    double mpi_time_used = end_time - start_time;

    // Resultados
    if (my_rank == 0)
    {
        cout << "Tiempo de CPU con clock(): " << cpu_time_used << " segundos." << endl;
        cout << "Tiempo total con MPI_Wtime(): " << mpi_time_used << " segundos." << endl;
    }

    MPI_Finalize();
    return 0;
}
