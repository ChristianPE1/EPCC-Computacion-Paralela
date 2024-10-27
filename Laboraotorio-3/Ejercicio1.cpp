#include <iostream>
#include <vector>
#include <mpi.h>

using namespace std;

void initialize_bins(float &min_meas, float &max_meas, const int &bin_count, vector<float> &bin_maxes, vector<int> &bin_counts)
{
    float bin_width = (max_meas - min_meas) / bin_count;

    // Inicializar bin_maxes y bin_counts
    for (int b = 0; b < bin_count; b++)
    {
        bin_maxes[b] = min_meas + bin_width * (b + 1);
        bin_counts[b] = 0;
    }
}

int find_bin(float value, const vector<float> &bin_maxes, int bin_count, float min_meas)
{
    if (value < bin_maxes[0])
        return 0;

    for (int b = 1; b < bin_count; b++)
    {
        if (value < bin_maxes[b])
            return b;
    }
    return bin_count - 1;
}

void create_histogram(const vector<float> &data, int data_count, float min_meas, float max_meas, int bin_count, vector<int> &global_bin_counts)
{
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    vector<float> bin_maxes(bin_count);
    vector<int> local_bin_counts(bin_count, 0);

    initialize_bins(min_meas, max_meas, bin_count, bin_maxes, global_bin_counts);

    // Dividir el trabajo: cada proceso trabaja en una parte de los datos
    int local_data_count = data_count / size;
    int start = rank * local_data_count;
    int end = (rank == size - 1) ? data_count : start + local_data_count;

    for (int i = start; i < end; i++)
    {
        if (data[i] >= min_meas && data[i] < max_meas)
        {
            int bin_index = find_bin(data[i], bin_maxes, bin_count, min_meas);
            local_bin_counts[bin_index]++;
        }
    }

    // Reducir los conteos locales al proceso 0
    MPI_Reduce(local_bin_counts.data(), global_bin_counts.data(), bin_count, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        for (int b = 0; b < bin_count; b++)
        {
            cout << "Bin " << b << ": Range [" << (b == 0 ? min_meas : bin_maxes[b - 1]) << ", " << bin_maxes[b] << "), Count: " << global_bin_counts[b] << endl;
        }
    }
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    vector<float> data{1.3, 2.9, 0.4, 0.3, 1.3, 4.4, 1.7, 0.4, 3.2, 0.3,
                       4.9, 2.4, 3.1, 4.4, 3.9, 0.4, 4.2, 4.5, 4.9, 0.9};
    int data_count = data.size();
    float min_meas = 0.0;
    float max_meas = 5.0;
    const int bin_count = 5;

    vector<int> global_bin_counts(bin_count);

    create_histogram(data, data_count, min_meas, max_meas, bin_count, global_bin_counts);

    MPI_Finalize();
    return 0;
}
