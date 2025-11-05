//aucunement sur de la qualite
#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <getopt.h>
using namespace std;

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);

	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);


	int seed;
	if (rank == 0) {
		seed = (rand() % 100) +10;
		int slave_rank;
		while ( seed != 1){
			if (seed % 2 == 0){
				slave_rank = 1;
				MPI_Send(&seed, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
			}
			else{
				slave_rank = 2;
				MPI_Send(&seed, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
			}
			MPI_Recv(&seed, 1, MPI_INT, slave_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			cout << "donnee recu : " << seed << " d'esclave " << slave_rank << endl;
		}
        MPI_Send(&seed, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&seed, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
		cout << "code termine " << endl;
	}
	if (rank ==1) {
		while (true){
			MPI_Recv(&seed, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if ( seed ==1 ) break;
			seed = seed / 2;
			cout << " slave 1 , value : " << seed << endl;
			MPI_Send(&seed, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
	}
	if (rank ==2) {
		while (true){
			MPI_Recv(&seed, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if ( seed ==1 ) break;
			seed = seed*3 +1 ;
			cout << " slave 2 , value : " << seed << endl;
			MPI_Send(&seed, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
	}


	MPI_Finalize();
	return 0;
}
