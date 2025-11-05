// exo 5.1
#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Vérification : 3 processus nécessaires
    if (size != 3) {
        if (rank == 0) std::cerr << "Erreur : ce programme nécessite exactement 3 processus.\n";
        MPI_Finalize();
        return 1;
    }

    const int MASTER = 0;
    const int EVEN_SLAVE = 1;
    const int ODD_SLAVE = 2;
    int x; // valeur courante
    int even_count = 0, odd_count = 0; // compte des appels aux esclaves

    if (rank == MASTER) {
        // 🔹 Génération d'un nombre aléatoire strictement positif
        srand(time(NULL));
        x = rand() % 100 + 1; // exemple : 1 à 100
        std::cout << "Nombre initial : " << x << std::endl;

        while (x != 1) {
            int dest, tag = 0;
            if (x % 2 == 0) {
                dest = EVEN_SLAVE;
                even_count++;
            } else {
                dest = ODD_SLAVE;
                odd_count++;
            }

            // 🔹 Envoi de x à l'esclave approprié
            MPI_Send(&x, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);

            // 🔹 Réception de la nouvelle valeur de x
            MPI_Recv(&x, 1, MPI_INT, dest, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Nouvelle valeur : " << x << std::endl;
        }

        // 🔹 Envoi du signal d'arrêt aux esclaves (valeur négative)
        x = -1;
        MPI_Send(&x, 1, MPI_INT, EVEN_SLAVE, 0, MPI_COMM_WORLD);
        MPI_Send(&x, 1, MPI_INT, ODD_SLAVE, 0, MPI_COMM_WORLD);

        // 🔹 Affichage du nombre d'appels
        std::cout << "Nombre d'appels à l'esclave pair : " << even_count << std::endl;
        std::cout << "Nombre d'appels à l'esclave impair : " << odd_count << std::endl;

    } else {
        int received;
        while (true) {
            MPI_Recv(&received, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // 🔹 Condition d'arrêt
            if (received < 0) break;

            int result;
            if (rank == EVEN_SLAVE) {
                result = received / 2;
            } else { // ODD_SLAVE
                result = 3 * received + 1;
            }

            MPI_Send(&result, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
