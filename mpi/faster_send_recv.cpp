#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Rang du processus
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Nombre total de processus

    const long N = 100000000; // Taille totale du problème
    long local_sum = 0;

    // Chaque processus prend une partie des indices
    std::vector<long> indices;
    for (long i = rank; i < N; i += size) {
        indices.push_back(i);
    }

    // 🔹 Calcul parallèle local avec OpenMP
    #pragma omp parallel for reduction(+:local_sum)
    for (size_t i = 0; i < indices.size(); i++) {
        local_sum += indices[i] * indices[i];
    }

    // 🔹 Chaque processus envoie sa somme locale au processus 0
    if (rank != 0) {
        // MPI_Send(buffer, count, datatype, destination, tag, communicator)
        MPI_Send(&local_sum, 1, MPI_LONG, 0, 0, MPI_COMM_WORLD);
    } else {
        // Processus 0 : reçoit les sommes des autres processus et les additionne
        long global_sum = local_sum; // Commence avec sa propre somme
        for (int source = 1; source < size; source++) {
            long recv_sum = 0;
            MPI_Recv(&recv_sum, 1, MPI_LONG, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            global_sum += recv_sum;
        }
        std::cout << "Somme des carrés de 0 à " << N-1 << " : " << global_sum << std::endl;
    }

    MPI_Finalize();
    return 0;
}
