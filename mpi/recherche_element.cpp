// exo5.2

#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>  // pour rand() et srand()
#include <ctime>    // pour time()

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // On veut exactement 5 processus(non)
	/*
    if (size != 5) {
        if (rank == 0) std::cerr << "Erreur : ce programme nécessite exactement 5 processus.\n";
        MPI_Finalize();
        return 1;
    }
	*/

    const long N = 1000000; // taille du tableau total
    const long chunk_size = N / size; // taille de chaque sous-tableau

    std::vector<int> full_array; // seulement utilisé par le maître

    // Initialisation aléatoire
    srand(time(NULL) + rank); // seed différente par processus

    // 🔹 Processus maître crée le tableau complet
    if (rank == 0) {
        full_array.resize(N);
        for (long i = 0; i < N; i++) {
            full_array[i] = rand() % 1000; // valeurs aléatoires entre 0 et 999
        }
    }

    // 🔹 Chaque processus prépare son sous-tableau
    std::vector<int> local_array(chunk_size);

    // 🔹 Répartition du tableau avec MPI_Scatter
    // Les N éléments de full_array sont découpés en `size` morceaux
    MPI_Scatter(
        full_array.data(),       // buffer source (sur le maître)
        chunk_size,              // nombre d'éléments envoyés à chaque processus
        MPI_INT,                 // type des éléments
        local_array.data(),      // buffer de réception local
        chunk_size,              // nombre d'éléments reçus
        MPI_INT,
        0,                       // rang du processus maître
        MPI_COMM_WORLD
    );

    // 🔹 Le maître choisit le nombre à chercher
    int x;
    if (rank == 0) {
        x = rand() % 1000; // nombre aléatoire entre 0 et 999
        std::cout << "Nombre à rechercher x = " << x << std::endl;
    }

    // 🔹 Diffuser x à tous les processus avec MPI_Bcast
    MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // 🔹 Chaque processus compte les occurrences locales de x
    int local_count = 0;
    for (long i = 0; i < chunk_size; i++) {
        if (local_array[i] == x) local_count++;
    }

    // 🔹 Le maître collecte les résultats de tous les processus
    if (rank == 0) {
        std::vector<int> counts(size);
        counts[0] = local_count; // ajout de son propre résultat

        for (int src = 1; src < size; src++) {
            MPI_Recv(&counts[src], 1, MPI_INT, src, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // Affichage du nombre d'occurrences par processus
        for (int i = 0; i < size; i++) {
            std::cout << "Processus " << i << " a trouvé " << counts[i] << " occurrences de " << x << std::endl;
        }

    } else {
        // Les esclaves envoient leur résultat au maître
        MPI_Send(&local_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
