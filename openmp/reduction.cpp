#include <iostream>
#include <omp.h>
#include <vector>
#include <cmath>
#include <unistd.h> // pour getopt

int main(int argc, char* argv[]) {
    int num_threads = 1; // valeur par défaut

    // Parsing des options avec getopt
    int opt;
    while((opt = getopt(argc, argv, "p:")) != -1) {
        switch(opt) {
            case 'p':
                num_threads = std::atoi(optarg);
                break;
            default:
                std::cerr << "Usage: " << argv[0] << " -p <nbthreads>" << std::endl;
                return 1;
        }
    }

    const int N = 10000000; // 10 millions d'éléments
    std::vector<double> array(N);

    // Remplissage du tableau
    for(int i = 0; i < N; i++) {
        array[i] = i + 1.0;
    }

    double sum = 0.0;

    // Définir le nombre de threads OpenMP
    omp_set_num_threads(num_threads);

    #pragma omp parallel for reduction(+:sum)
    for(int i = 0; i < N; i++) {
        double x = array[i];
        sum += sqrt(x) * sin(x) + cos(x) * log(x + 1);
    }

    std::cout << "Résultat : " << sum << std::endl;
    return 0;
}

