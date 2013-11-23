#include <stdio.h>
#include <limits.h>

#include "InOut.h"
#include "TSP.h"
#include "ants.h"
#include "utilities.h"
#include "InOut.h"

int main(int argc, char *argv[]){
	init_program(argc, argv);
	instance.nearestNeighboursList = computeNearestNeighboursList();
	pheromone_matrix = generateDoubleMatrix(numberOfCities, 
		numberOfCities);
	pheromone_times_heuristic_matrix = generateDoubleMatrix(numberOfCities, 
		numberOfCities);
	printf("Initialization finished \n");

	for (try_counter = 0; try_counter < maximum_independent_tries; try_counter++){
		initialize_variables_for_trial(try_counter);

		/*while(!isTerminationConditionMet()){
			}*/
	}
}

void initialize_variables_for_trial(long int try_number){
	printf("Initialize trial \n");
	constructed_tours_counter = 1;
	iteration_counter = 1;
	restart_iteration = 1; 

	branching_factor_parameter = 0.05;
	best_so_far_ant->tour_length = INFINITY;
	best_solution_iteration = 0;

	maximum_pheromone_trail = 1./((evaporation_parameter)*
		get_some_nearest_neighbour_tour_length());
	minimum_pheromone_trail = maximum_pheromone_trail/(2. * numberOfCities);
	initialize_pheromone_trails(maximum_pheromone_trail);
	calculate_pheromone_times_heuristic_matrix();
}

/*long int isTerminationConditionMet(void){
}*/