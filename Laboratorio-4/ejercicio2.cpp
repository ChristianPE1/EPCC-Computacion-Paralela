#include <iostream>
#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

void *test(void *arg)
{
    int *data = (int *)arg;

    return nullptr;
}

int main()
{
    int num_threads;

    cout << "Numero de hilos: ";
    cin >> num_threads;

    pthread_t threads[num_threads];
    int val = 10;
    long long estimate_time = 0;

    for (int i = 0; i < num_threads; i++)
    {
        auto start = high_resolution_clock::now();
        pthread_create(&threads[i], nullptr, test, &val);
        auto end = high_resolution_clock::now();
        long long duration = duration_cast<microseconds>(end - start).count();
        estimate_time += duration;
    }
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], nullptr);
    }
    cout << "Tiempo aproximado: " << estimate_time / num_threads << endl;

    return 0;
}

// g++ -o ejercicio2 ejercicio2.cpp -pthread
