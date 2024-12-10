#include <iostream>
#include <pthread.h>

using namespace std;

const int NUM_THREADS = 4;
int counter = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;

void *threadFunction(void *arg) {
    int thread_id = *(int *)arg;

    cout << "Thread " << thread_id << " esperando en la barrera...\n";

    pthread_mutex_lock(&mutex);
    counter++;
    if (counter == NUM_THREADS) {
        pthread_cond_broadcast(&cond); // Despierta a todos los threads
    } else {
        pthread_cond_wait(&cond, &mutex); // Espera a que la condición sea verdadera
    }
    pthread_mutex_unlock(&mutex);

    cout << "Thread " << thread_id << " pasó la barrera.\n";
    return nullptr;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&cond, nullptr);

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], nullptr, threadFunction, &thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], nullptr);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
