#include <stdio.h>
#include <limits.h>

#include "InOut.h"
#include "TSP.h"
#include "ants.h"
#include "utilities.h"
#include "InOut.h"

int main(int argc, char *argv[]){
	initProgram(argc, argv);
	instance.nearestNeighboursList = computeNearestNeighboursList();
	pheromone_matrix = generateDoubleMatrix(numberOfCities, 
		numberOfCities);
	pheromone_times_heuristic_matrix = generateDoubleMatrix(numberOfCities, 
		numberOfCities);
	printf("Initialization finished \n");

	for (tryCounter = 0; tryCounter < maximumIndependentTries; tryCounter++){
		initializeVariablesForTrial(tryCounter);

		/*while(!isTerminationConditionMet()){
			}*/
	}
}

void initializeVariablesForTrial(long int tryNumber){
	printf("Initialize trial \n");
	constructedToursCounter = 1;
	iterationCounter = 1;
	restartIteration = 1; 

	branchingFactorParameter = 0.05;
	best_so_far_ant->tour_length = INFINITY;
	bestSolutionIteration = 0;

	maximum_pheromone_trail = 1./((evaporation_parameter)*
		get_some_nearest_neighbour_tour_length());
	minimum_pheromone_trail = maximum_pheromone_trail/(2. * numberOfCities);
	initialize_pheromone_trails(maximum_pheromone_trail);
	calculate_pheromone_times_heuristic_matrix();
}

/*long int isTerminationConditionMet(void){
}*/