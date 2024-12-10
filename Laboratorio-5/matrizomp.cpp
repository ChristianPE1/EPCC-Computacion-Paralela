#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

int main() {
    const int N = 4; // Tamaño de la matriz y el vector
    vector<vector<int>> A(N, vector<int>(N, 1)); // Matriz NxN inicializada con 1
    vector<int> x(N, 1); // Vector inicializado con 1
    vector<int> y(N, 0); // Resultado

    #pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            #pragma omp atomic
            y[i] += A[i][j] * x[j];
        }
    }

    // Mostrar el resultado
    cout << "Resultado y: ";
    for (int i = 0; i < N; ++i) {
        cout << y[i] << " ";
    }
    cout << endl;

    return 0;
}
