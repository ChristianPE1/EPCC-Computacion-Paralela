#include <iostream>
#include <chrono>
#include <vector>

using namespace std;

void multiply(vector<vector<double>> &A, vector<vector<double>> &B, vector<vector<double>> &C)
{
   for (int i = 0; i < A.size(); i++)
      for (int j = 0; j < B[0].size(); j++)
         for (int k = 0; k < A[0].size(); k++)
            C[i][j] += A[i][k] * B[k][j];
}

int main()
{

   int fA = 10, cA = 10;
   int fB = 10, cB = 10;
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
   multiply(A, B, C);
   auto terminar = chrono::high_resolution_clock::now();
   chrono::duration<float, milli> duration = terminar - inicio;
   cout << "Dimensions: " << fA << "x" << cA << " * " << fB << "x" << cB << endl;
   cout << "Duracion: " << duration.count() << " ms" << endl;

   return 0;
}