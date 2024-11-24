#include <iostream>
#include <cstdlib>
#include <ctime>
#include <pthread.h>

using namespace std;

struct ThreadData
{
    long long tosses;
    long long hits;
};

pthread_mutex_t mutex_hits;

void *monteCarlo(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    long long local_hits = 0;

    // Semilla para los numeros aleatorios
    srand(time(0) + pthread_self());

    for (long long i = 0; i < data->tosses; i++)
    {
        double x = (double)rand() / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand() / RAND_MAX * 2.0 - 1.0;
        double distance_squared = x * x + y * y;

        if (distance_squared <= 1.0)
        {
            local_hits++;
        }
    }

    // Seccion crítica
    pthread_mutex_lock(&mutex_hits);
    data->hits += local_hits;
    pthread_mutex_unlock(&mutex_hits);

    return nullptr;
}

int main()
{
    int num_threads;
    long long total_tosses;

    cout << "Numero de lanzamientos: ";
    cin >> total_tosses;
    cout << "Numero de hilos: ";
    cin >> num_threads;

    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];
    long long tosses_per_thread = total_tosses / num_threads;

    // Inicializar el mutex
    pthread_mutex_init(&mutex_hits, nullptr);

    for (int i = 0; i < num_threads; i++)
    {
        thread_data[i].tosses = tosses_per_thread;
        thread_data[i].hits = 0;
        pthread_create(&threads[i], nullptr, monteCarlo, &thread_data[i]);
    }

    long long total_hits = 0;
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], nullptr);
        total_hits += thread_data[i].hits;
    }

    double estimate = 4.0 * (static_cast<double>(total_hits) / total_tosses);
    cout << "Estimado de pi: " << estimate << endl;

    pthread_mutex_destroy(&mutex_hits);

    return 0;
}

// g++ -o ejercicio1 ejercicio1.cpp -pthread
