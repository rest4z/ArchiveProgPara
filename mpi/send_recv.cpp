#include <mpi.h>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    // 1️⃣ Initialisation de l'environnement MPI
    MPI_Init(&argc, &argv);

    // 2️⃣ Récupération du rang du processus et du nombre total de processus
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Rang du processus courant
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Nombre total de processus

    // On s'assure d'avoir au moins 2 processus pour l'exemple
    if (size < 2) {
        if (rank == 0) {
            std::cerr << "Il faut au moins 2 processus pour cet exemple.\n";
        }
        MPI_Finalize();
        return 1;
    }

    // 3️⃣ Définition des données à envoyer
    int tag = 0; // Identifiant du message
    if (rank == 0) {
        // Le processus 0 va envoyer un entier au processus 1
        int data_to_send = 12345;
        std::cout << "[Rank 0] Envoi de la donnée " << data_to_send << " au processus 1\n";

        // MPI_Send(buffer, count, datatype, destination, tag, communicator)
        MPI_Send(&data_to_send, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);

        // Remarque : MPI_Send est bloquant, donc ce processus attend que MPI commence à transférer
        // le message avant de continuer. Si le message est petit, le transfert peut être immédiat.
    }
    else if (rank == 1) {
        int received_data = 0;

        // MPI_Recv(buffer, count, datatype, source, tag, communicator, status)
        MPI_Recv(&received_data, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Remarque : MPI_Recv est bloquant. Ce processus attend que le message du processus 0 arrive.
        std::cout << "[Rank 1] Reçu la donnée : " << received_data << " du processus 0\n";
    }

    // 4️⃣ Exemple avec un tableau de données
    if (rank == 0) {
        std::vector<int> array_to_send = {1, 2, 3, 4, 5};
        std::cout << "[Rank 0] Envoi d'un tableau de taille " << array_to_send.size()
                  << " au processus 1\n";

        MPI_Send(array_to_send.data(), array_to_send.size(), MPI_INT, 1, 1, MPI_COMM_WORLD);
    }
    else if (rank == 1) {
        int array_received[5];
        MPI_Recv(array_received, 5, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        std::cout << "[Rank 1] Tableau reçu : ";
        for (int i = 0; i < 5; i++) std::cout << array_received[i] << " ";
        std::cout << "\n";
    }

    // 5️⃣ Finalisation de MPI
    MPI_Finalize();

    return 0;
}
