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
	pheromoneMatrix = generateDoubleMatrix(numberOfCities, 
		numberOfCities);
	pheromoneTimesHeuristicMatrix = generateDoubleMatrix(numberOfCities, 
		numberOfCities);
	printf("Initialization finished \n");

	for (tryCounter = 0; tryCounter < maximumIndependentTries; tryCounter++){
		initializeVariablesForTrial(tryCounter);
	}
}

void initializeVariablesForTrial(long int tryNumber){
	printf("Initialize trial \n");
	constructedToursCounter = 1;
	iterationCounter = 1;
	restartIteration = 1; 

	branchingFactorParameter = 0.05;
	bestSoFarAnt->tourLength = INFINITY;
	bestSolutionIteration = 0;

	maximumPheromoneTrail = 1./((evaporationParameter)*
		getSomeNearestNeighbourTourLength());
	minimumPheromoneTrail = maximumPheromoneTrail/(2. * numberOfCities);
	initializePheromoneTrails(maximumPheromoneTrail);
}