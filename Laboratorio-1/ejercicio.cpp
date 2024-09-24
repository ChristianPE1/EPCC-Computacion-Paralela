#include <iostream>
#include <chrono>
#include <vector>

using namespace std;

int main()
{
    const int MAX = 10000;
    vector<vector<double>> A(MAX, vector<double>(MAX, 1.0));
    vector<double> x(MAX, 1.0);
    vector<double> y(MAX, 0.0);

    auto inicio = chrono::high_resolution_clock::now();

    // First pair of loops
    for (int i = 0; i < MAX; i++)
        for (int j = 0; j < MAX; j++)
            y[i] += A[i][j] * x[j];

    auto terminar = chrono::high_resolution_clock::now();
    chrono::duration<float, milli> duration = terminar - inicio;
    cout << "Duracion bucle filas: " << duration.count() << " ms" << endl;

    // Reset y
    fill(y.begin(), y.end(), 0.0);

    inicio = chrono::high_resolution_clock::now();

    // Second pair of loops
    for (int j = 0; j < MAX; j++)
        for (int i = 0; i < MAX; i++)
            y[i] += A[i][j] * x[j];

    terminar = chrono::high_resolution_clock::now();
    duration = terminar - inicio;
    cout << "Duracion bucle columnas: " << duration.count() << " ms" << endl;

    return 0;
}
