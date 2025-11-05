#include <mpi.h>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Rang du processus
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Nombre total de processus

    const int N = 1000000000; // Taille de la boucle
    int local_sum = 0;

    // Boucle parallèle : chaque processus calcule une partie
    for (int i = rank; i < N; i += size) {
        local_sum += i * i;
    }

    int global_sum = 0;
    // Réduction pour obtenir la somme globale
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "La somme des carrés de 0 à " << N-1 << " est " << global_sum << std::endl;
    }

    MPI_Finalize();
    return 0;
}

