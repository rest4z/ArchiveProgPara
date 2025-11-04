#include <iostream>
#include <unistd.h>
#include <cmath>
#include <cstdlib>
#include <omp.h>
#include <stdio.h>

typedef double real;
typedef real (*ReductionMethod)(real *t, size_t n);

real function(real v) {
	for (int i = 0; i < 100; ++i) {
		v = sin(v + 0.01);
	}
	return v;
}

real reduction_serial(real *t, size_t n) {
	real sum = 0;
	for (size_t i = 0; i < n ; ++i) sum += function(t[i]);
	return sum;
}

real reduction_openmp(real *t, size_t n) {
	real sum = 0;
#pragma omp parallel for
	for (size_t i = 0; i < n ; ++i) sum += function(t[i]);
	return sum;
}

real* init_tab(size_t n){
	real* tab = new real[n];
	for (size_t i = 0; i < n; i++){
		tab[i] = (real) (rand() % 101);
	}
	return tab;
}

int main(int argc, char* argv[]){
	omp_set_num_threads(8);
	int opt;
	int size = 0;
	int methode = 1;

	while ((opt = getopt(argc, argv, "p:s:m:")) != -1 ){
		switch(opt){
		case 'p':
			//set numthreads
			omp_set_num_threads(atoi(optarg));
		break;
		case 's':
			size = atoi(optarg);
			std::cout << "taille tableau : " << size << std::endl;
		break;
		case 'm':
			methode = atoi(optarg);
			std::cout << "methode selectionne : " << methode << std::endl;
		break;
		case '?':
			std::cout<< "Argument inconnu" << std::endl;
		break;
		default:
			std::cout <<"Saisir arguments" << std::endl;
		}
	}
	real* tab = init_tab(size);
	if (methode == 1){
		std::cout << "somme methode 1: "<<reduction_serial(tab, size)<< std::endl;
	} else if (methode == 2){
		std::cout << "somme methode 2 : "<<reduction_openmp(tab, size)<< std::endl;
	}
	return 0;
}


