#include <iostream>
#include <chrono>
#include <vector>

using namespace std;

void block_multiply(vector<vector<double>> &A, vector<vector<double>> &B, vector<vector<double>> &C, int blockSize)
{
   int n = A.size();

   for (int ii = 0; ii < n; ii += blockSize)
      for (int jj = 0; jj < n; jj += blockSize)
         for (int kk = 0; kk < n; kk += blockSize)
            for (int i = ii; i < min(ii + blockSize, n); i++)
               for (int j = jj; j < min(jj + blockSize, n); j++)
                  for (int k = kk; k < min(kk + blockSize, n); k++)
                     C[i][j] += A[i][k] * B[k][j];
}

int main()
{
   int fA = 1000, cA = 1000;
   int fB = 1000, cB = 1000;
   int blockSize = 50;

   vector<vector<double>> A(fA, vector<double>(cA, 1.0));
   vector<vector<double>> B(fB, vector<double>(cB, 1.0));
   vector<vector<double>> C(fA, vector<double>(cB, 0.0));

   // Llenar A y B con números aleatorios entre 0 y 1
   for (int i = 0; i < fA; i++)
   {
      for (int j = 0; j < cA; j++)
         A[i][j] = (double)rand() / RAND_MAX;
   }
   for (int i = 0; i < fB; i++)
   {
      for (int j = 0; j < cB; j++)
         B[i][j] = (double)rand() / RAND_MAX;
   }

   auto inicio = chrono::high_resolution_clock::now();
   block_multiply(A, B, C, blockSize);
   auto terminar = chrono::high_resolution_clock::now();
   chrono::duration<float, milli> duration = terminar - inicio;
   cout << "Dimensions: " << fA << "x" << cA << " * " << fB << "x" << cB << endl;
   cout << "Duracion (multiplicacion por bloques): " << duration.count() << " ms" << endl;

   return 0;
}
