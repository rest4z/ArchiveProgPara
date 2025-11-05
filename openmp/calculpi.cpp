//aucunement sur de la qualite
#include <random>
#include <cstdint>
#include <unistd.h>
#include <iostream>
#include <getopt.h>
#include <omp.h>
using namespace std;

typedef uint64_t u64;

//probleme : acces souvent ressource critic , solution -> critical mais on perd lutilite du parralel
u64 monte_carlo(u64 max_points_inside_square)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    u64 points_inside_circle = 0;

    for (u64 i = 0; i < max_points_inside_square; ++i)
    {
        // génère un point dans le carré
        double x = dis(gen);
        double y = dis(gen);

        // est-il dans le cercle ?
        if (x * x + y * y <= 1.0)
        {
            ++points_inside_circle;
        }
    }

    return points_inside_circle;
}

//solution 1 : faire un tableau d'elements  ? photo
u64 monte_carlo_parallel(u64 max_points_inside_square)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    u64 points_inside_circle = 0;

	#pragma omp parralel for
    for (u64 i = 0; i < max_points_inside_square; ++i)
    {
        // génère un point dans le carré
        double x = dis(gen);
        double y = dis(gen);

        // est-il dans le cercle ?
        if (x * x + y * y <= 1.0)
        {
            ++points_inside_circle;
        }
    }

    return points_inside_circle;
}

//solution 3 : faire de la reduction
u64 monte_carlo_reduction(u64 max_points_inside_square)
{

	u64 points_inside_circle = 0;

#pragma omp parallel
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(-1.0, 1.0);

#pragma omp for reduction(+:points_inside_circle)
		for (u64 i = 0; i < max_points_inside_square; ++i)
		{
			// génère un point dans le carré
			double x = dis(gen);
			double y = dis(gen);

			// est-il dans le cercle ?
			if (x * x + y * y <= 1.0)
			{
				++points_inside_circle;
			}
		}
	}
	return points_inside_circle;
}

int main(int argc, char *argv[]){
	int opt;
	int method = 1;
	u64 nombre_points = 0;
	u64 resultat_points = 0;

	while ((opt = getopt(argc, argv, "m:n:")) != -1 ){
		switch(opt){
			case 'm':
				method = std::atoi(optarg);
				break;
			case 'n':
				nombre_points = std::atoi(optarg);
				break;
			case '?':
				std::cerr << "Unknown option: " << (char)optopt << "\n";
				break;
		}
	}

	std::cout << "methode : "<< method <<std::endl;
	std::cout << "nombre de points : "<< nombre_points <<std::endl;

	if (method == 1){
		resultat_points = monte_carlo(nombre_points);
	}
	if (method == 2){
		resultat_points = monte_carlo_parallel(nombre_points);
	}
	if (method == 3){
		resultat_points = monte_carlo_reduction(nombre_points);
	}
	double result = (float)resultat_points/(float)nombre_points;

	std::cout<< "resultat" << resultat_points << "/" << nombre_points << " : " << result*4 << std::endl;
	return 0;
}
