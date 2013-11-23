#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ants.h"
#include "TSP.h"
#include "utilities.h"
#include "InOut.h"
#include "ls.h"

long int numberOfAnts;
long int nearestNeighboursListLength;

double evaporationParameter;
double trailImportance;
double heuristicValueImportance;
double bestChoiceProbability;

long int iterationsToUpdateBestAnt;
long int maxMinAntSystemFlag;
long int numberOfElitistAnts;

double maximumPheromoneTrail;
double minimumPheromoneTrail;

antStruct *antColony;
antStruct *bestSoFarAnt;
antStruct *restartBestAnt;

double **pheromoneMatrix;
double **pheromoneTimesHeuristicMatrix;

double *probOfSelection;

void allocateAntColonyMemory(void){
	long int i;
	antColony = malloc(sizeof(antStruct)*numberOfAnts + 
		sizeof(antStruct *)* numberOfAnts);
	if (antColony == NULL){
		printf("Out of memory. exit");
		exit(1);
	}

	for (i = 0; i < numberOfAnts; i++){
		antColony[i].tour = calloc(numberOfCities + 1, sizeof(long int));
		antColony[i].visited = calloc(numberOfCities, sizeof(char));
	}
	
	bestSoFarAnt = malloc(sizeof(antStruct));
	if (bestSoFarAnt == NULL){
		printf("Out of memory. exit");
		exit(1);
	}
	bestSoFarAnt->tour = calloc(numberOfCities + 1, sizeof(long int));
	bestSoFarAnt->visited = calloc(numberOfCities, sizeof(char));

	restartBestAnt = malloc(sizeof(antStruct));
	if (restartBestAnt == NULL){
		printf("Out of memory. exit");
		exit(1);
	}
	restartBestAnt->tour = calloc(numberOfCities + 1, sizeof(long int));
	restartBestAnt->visited = calloc(numberOfCities, sizeof(char));

	probOfSelection = malloc(sizeof(double)*(numberOfAnts + 1));
	if (probOfSelection == NULL){
		printf("Out of memory. exit");
		exit(1);
	}
	probOfSelection[numberOfAnts] = HUGE_VAL;
}

long int getSomeNearestNeighbourTourLength(void){
	long int constructionStepCounter;
	long int help;
	emptyAntMemory(&antColony[0]);
	
	constructionStepCounter = 0;
	placeAnt(&antColony[0], constructionStepCounter);

	while (constructionStepCounter < numberOfCities -1){
		constructionStepCounter ++;
		moveToClosestCity(&antColony[0], constructionStepCounter);
	}
	constructionStepCounter = numberOfCities;
	antColony[0].tour[numberOfCities] = antColony[0].tour[0];
	if(localSearchFlag){
		applyTwoOptFirst(antColony[0].tour);
	}
	constructedToursCounter += 1;
	antColony[0].tourLength = computeTourLength(antColony[0].tour);

	help = antColony[0].tourLength;
	emptyAntMemory(&antColony[0]);
	return help;
}

void moveToClosestCity(antStruct *ant, long int constructionStepPhase){
	long int city;
	long int currentCity;
	long int nextCity;
	long int minimumDistance;

	nextCity = numberOfCities;
	currentCity = ant->tour[constructionStepPhase -1];
	minimumDistance = INFINITY;

	for(city = 0; city < numberOfCities; city++){
		if(!ant->visited[city] &&  
			instance.distanceMatrix[currentCity][city] < minimumDistance){
			nextCity = city;
		}
	}
	ant->tour[constructionStepPhase] = nextCity;
	ant->visited[nextCity] = TRUE;	
}

void emptyAntMemory(antStruct *ant){
	long int i;
	for (i = 0; i < numberOfCities; i++){
		ant->visited[i] = FALSE;
	}
}

void placeAnt(antStruct *ant, long int numberOfConstructionSteps){
	long int random;
	random = (long int)(generateRandomBetween0and1(&seed) 
		* (double) numberOfCities);

	ant->tour[numberOfConstructionSteps] = random;
	ant->visited[random] = TRUE;
}

void initializePheromoneTrails(double initialValue){
	//TODO: Pending implementation  

}