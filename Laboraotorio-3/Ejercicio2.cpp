#include <iostream>
#include <cstdlib>
#include <ctime>
#include <mpi.h>

using namespace std;

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long long tosses;         // Numero de lanzamientos
    long long local_hits = 0; // Conteo de dardos dentro del circulo

    // Solo el proceso 0 solicita el numero total de lanzamientos
    if (rank == 0)
    {
        cout << "Introduce el numero total de lanzamientos: ";
        cin >> tosses;
    }

    // Transmite el numero lanzamientos a todos los procesos
    MPI_Bcast(&tosses, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    // Diferentes semillas para cada proceso
    srand(time(0) + rank);
    // Cada proceso lanza su parte
    long long local_tosses = tosses / size;
    for (long long i = 0; i < local_tosses; i++)
    {
        // Generar coordenadas aleatorias entre -1 y 1
        double x = (double)rand() / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand() / RAND_MAX * 2.0 - 1.0;
        double distance_squared = x * x + y * y;

        // Comprueba si el dardo esta dentro del circulo
        if (distance_squared <= 1.0)
        {
            local_hits++;
        }
    }

    // Suma los hits locales al proceso 0
    long long total_hits;
    MPI_Reduce(&local_hits, &total_hits, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    // Proceso 0 calcula y muestra la estimación de pi
    if (rank == 0)
    {
        double pi_estimate = 4.0 * total_hits / tosses;
        cout << "Estimacion de pi: " << pi_estimate << endl;
    }

    MPI_Finalize();
    return 0;
}
