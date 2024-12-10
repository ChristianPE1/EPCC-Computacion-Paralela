#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

int main()
{
    long long total_tosses;
    int num_threads;

    cout << "Numero de lanzamientos: ";
    cin >> total_tosses;
    cout << "Numero de hilos: ";
    cin >> num_threads;

    long long total_hits = 0;

    // Establecer el número de hilos
    omp_set_num_threads(num_threads);

    // Semilla para los números aleatorios (individual para cada hilo)
    srand(time(0));

    #pragma omp parallel reduction(+:total_hits)
    {
        unsigned int seed = time(0) + omp_get_thread_num(); // Semilla específica para cada hilo
        long long local_hits = 0;

        #pragma omp for
        for (long long i = 0; i < total_tosses; i++)
        {
            double x = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
            double y = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
            double distance_squared = x * x + y * y;

            if (distance_squared <= 1.0)
            {
                local_hits++;
            }
        }

        total_hits += local_hits;
    }

    double estimate = 4.0 * (static_cast<double>(total_hits) / total_tosses);
    cout << "Estimado de pi: " << estimate << endl;

    return 0;
}
