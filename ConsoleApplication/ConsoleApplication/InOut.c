#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <assert.h>
#include <string.h>

#include "InOut.h"
#include "ls.h"
#include "ants.h"
#include "utilities.h"
#include "TSP.h"

char fileName[LINE_BUF_LEN];

long int tryCounter;
long int constructedToursCounter;
long int iterationCounter;

long int bestSolutionIteration;
long int restartIteration;

long int maximumIndependentTries;
long int maximumToursOneTry;
long int seed;
double maximumTimeForOneTry;
long int optimalSolution;

double branchingFactorParameter;
double branchingFactorLimit;

long int *bestInTry;
long int *bestFoundAt;
double *timeBestFound;
double *timeTotalRun;

void initProgram(long int argc, char *argv[]){
	printf(PROGRAM_IDENTIFIER);
	setDefaultParameters();
	bestInTry = calloc(maximumIndependentTries, sizeof(long int));
	bestFoundAt = calloc(maximumIndependentTries, sizeof(long int));
	timeBestFound = calloc(maximumIndependentTries, sizeof(double));
	timeTotalRun = calloc(maximumIndependentTries, sizeof(double));

	printf("Reading problem data - Starting... \n\n");
	//Hard-coded value. Replace later for command line parameter.
	instance.nodesArray = readInstanceFile("ulysses16.tsp");
	printf("Reading problem data - Done ... \n\n");

	nearestNeighboursMaximalDepth = MIN(numberOfAnts -1, 
		nearestNeighboursMaximalDepth);
	assert(numberOfAnts < MAXIMUM_NUMBER_OF_ANTS);
	assert(nearestNeighboursListLength < MAXIMUM_NEAREST_NEIGHBOURS);
	assert(nearestNeighboursListLength > 0);
	assert(nearestNeighboursMaximalDepth > 0);

	printf("calculating distance matrix ..\n\n");
	instance.distanceMatrix = computeDistanceMatrix();
	printf(" .. done\n");
	showParameters();
	printf("allocate ants' memory ..\n\n");
	allocateAntColonyMemory();
	printf(" .. done\n");
}

void showParameters(void){
	fprintf(stdout, "\nParameter-settings: \n\n");

	fprintf(stdout, "localSearchFlag \t\t %ld\n", localSearchFlag);
	fprintf(stdout, "nearestNeighboursMaximalDepth \t\t %ld\n", nearestNeighboursMaximalDepth);
	fprintf(stdout, "numberOfAnts \t\t %ld\n", numberOfAnts);
	fprintf(stdout, "nearestNeighboursListLength \t\t %ld\n", nearestNeighboursListLength);
	fprintf(stdout, "trailImportance \t\t %ld\n", trailImportance);
	fprintf(stdout, "heuristicValueImportance \t\t %ld\n", heuristicValueImportance);
	fprintf(stdout, "evaporationParameter \t\t %ld\n", evaporationParameter);
	fprintf(stdout, "bestChoiceProbability \t\t %ld\n", bestChoiceProbability);
	fprintf(stdout, "maximumIndependentTries \t\t %ld\n", maximumIndependentTries);
	fprintf(stdout, "maximumToursOneTry \t\t %ld\n", maximumToursOneTry);
	fprintf(stdout, "seed \t\t %ld\n", seed);
	fprintf(stdout, "maximumTimeForOneTry \t\t %ld\n", maximumTimeForOneTry);
	fprintf(stdout, "optimalSolution\t\t %ld\n", optimalSolution);
	fprintf(stdout, "maxMinAntSystemFlag \t\t %ld\n", maxMinAntSystemFlag);
	fprintf(stdout, "numberOfElitistAnts \t\t %ld\n", numberOfElitistAnts);

	fprintf(stdout, "\n");
}


void setDefaultParameters(void){
	dontLookBitsFlag = TRUE;
	localSearchFlag = 3;
	nearestNeighboursMaximalDepth = 20;
	numberOfAnts = 25;
	nearestNeighboursListLength = 20;

	trailImportance = 1.0;
	heuristicValueImportance = 2.0;
	evaporationParameter = 0.5;
	bestChoiceProbability = 0.0;
	maximumIndependentTries = 10;
	maximumToursOneTry = 0;

	seed = (long int) time(NULL);
	maximumTimeForOneTry = 10.0;

	optimalSolution = 1;
	branchingFactorLimit = 1.00001;
	iterationsToUpdateBestAnt = INFINITY;
	maxMinAntSystemFlag = TRUE;
	numberOfElitistAnts = 0;	
}

//TODO: Change variable names
struct point *readInstanceFile(const char *fileName){
	FILE *tsp_file;
	char buf[LINE_BUF_LEN];
	long int i, j;
	struct point *nodeptr;

	tsp_file = fopen(fileName, "r");

	if ( tsp_file == NULL ) {
		fprintf(stderr,"No instance file specified, abort\n");
		exit(1);
	}
	assert(tsp_file != NULL);
	printf("\nreading tsp-file %s ... \n\n", fileName);

	fscanf(tsp_file,"%s", buf);
	while ( strcmp("NODE_COORD_SECTION", buf) != 0 ) {
		if ( strcmp("NAME", buf) == 0 ) {
			fscanf(tsp_file, "%s", buf);
			printf("%s ", buf);
			fscanf(tsp_file, "%s", buf);
			strcpy(instance.instanceName, buf);
			printf("%s \n", instance.instanceName); 
			buf[0]=0;
		} else if ( strcmp("NAME:", buf) == 0 ) {
			fscanf(tsp_file, "%s", buf);
			strcpy(instance.instanceName, buf);
			printf("%s \n", instance.instanceName);
			buf[0]=0;
		} else if ( strcmp("COMMENT", buf) == 0 ){
			fgets(buf, LINE_BUF_LEN, tsp_file);
			printf("%s", buf);
			buf[0]=0;
		} else if ( strcmp("COMMENT:", buf) == 0 ){
			fgets(buf, LINE_BUF_LEN, tsp_file);
			printf("%s", buf);
			buf[0]=0;
		} else if ( strcmp("TYPE", buf) == 0 ) {
			fscanf(tsp_file, "%s", buf);
			printf("%s ", buf);
			fscanf(tsp_file, "%s", buf);
			printf("%s\n", buf);
			if( strcmp("TSP", buf) != 0 ) {
			fprintf(stderr,"\n Not a TSP instance in TSPLIB format !!\n");
			exit(1);
			}
			buf[0]=0;
		} else if ( strcmp("TYPE:", buf) == 0 ) {
			fscanf(tsp_file, "%s", buf);
			printf("%s\n", buf);
			if( strcmp("TSP", buf) != 0 ) {
			fprintf(stderr,"\n Not a TSP instance in TSPLIB format !!\n");
			exit(1);
			}
			buf[0]=0;
		} else if( strcmp("DIMENSION", buf) == 0 ){
			fscanf(tsp_file, "%s", buf);
			printf("%s ", buf);
			fscanf(tsp_file, "%ld", &numberOfCities);
			instance.numberOfCities = numberOfCities;
			printf("%ld\n", numberOfCities);
			assert ( numberOfCities > 2 && numberOfCities < 6000);
			buf[0]=0;
		} else if ( strcmp("DIMENSION:", buf) == 0 ) {
			fscanf(tsp_file, "%ld", &numberOfCities);
			instance.numberOfCities = numberOfCities;
			printf("%ld\n", numberOfCities);
			assert ( numberOfCities > 2 && numberOfCities < 6000);
			buf[0]=0;
		} else if( strcmp("DISPLAY_DATA_TYPE", buf) == 0 ){
			fgets(buf, LINE_BUF_LEN, tsp_file);
			printf("%s", buf);
			buf[0]=0;
		} else if ( strcmp("DISPLAY_DATA_TYPE:", buf) == 0 ) {
			fgets(buf, LINE_BUF_LEN, tsp_file);
			printf("%s", buf);	
			buf[0]=0;
		} else if( strcmp("EDGE_WEIGHT_TYPE", buf) == 0 ){
			buf[0]=0;
			fscanf(tsp_file, "%s", buf);
			printf("%s ", buf);
			buf[0]=0;
			fscanf(tsp_file, "%s", buf);
			printf("%s\n", buf);
			if ( strcmp("EUC_2D", buf) == 0 ) {
				distanceFunction = getRoundDistance;
			} else if ( strcmp("CEIL_2D", buf) == 0 ) {
				distanceFunction = getCeilingDistance;
			} else if ( strcmp("GEO", buf) == 0 ) {
				distanceFunction = getGeometricDistance;
			} else if ( strcmp("ATT", buf) == 0 ) {
				distanceFunction = getAttDistance;
			} else {
				fprintf(stderr,"EDGE_WEIGHT_TYPE %s not implemented\n",buf);
			}
			strcpy(instance.edgeWeightType, buf);
			buf[0]=0;
		} else if( strcmp("EDGE_WEIGHT_TYPE:", buf) == 0 ){
			/* set pointer to appropriate distance function; has to be one of 
				EUC_2D, CEIL_2D, GEO, or ATT. Everything else fails */
			buf[0]=0;
			fscanf(tsp_file, "%s", buf);
			printf("%s\n", buf);
			printf("%s\n", buf);
			printf("%s\n", buf);
			if ( strcmp("EUC_2D", buf) == 0 ) {
				distanceFunction = getRoundDistance;
			} else if ( strcmp("CEIL_2D", buf) == 0 ) {
				distanceFunction = getCeilingDistance;
			} else if ( strcmp("GEO", buf) == 0 ) {
				distanceFunction = getGeometricDistance;
			} else if ( strcmp("ATT", buf) == 0 ) {
				distanceFunction = getAttDistance;
			} else {
				fprintf(stderr,"EDGE_WEIGHT_TYPE %s not implemented\n",buf);
				exit(1);
			}
			strcpy(instance.edgeWeightType, buf);
			buf[0]=0;
		}
		buf[0]=0;
		fscanf(tsp_file,"%s", buf);
	}

	if( strcmp("NODE_COORD_SECTION", buf) == 0 ){
		printf("found section contaning the node coordinates\n");
	} else {
		fprintf(stderr,"\n\nSome error ocurred finding start of coordinates from tsp file !!\n");
		exit(1);
	}

	if( (nodeptr = malloc(sizeof(struct point) * numberOfCities)) == NULL ){
		exit(EXIT_FAILURE);
	} else {
		for ( i = 0 ; i < numberOfCities ; i++ ) {
			fscanf(tsp_file,"%ld %lf %lf", &j, &nodeptr[i].x, &nodeptr[i].y );
		}
	}
	printf("number of cities is %ld\n",numberOfCities);
	printf("\n... done\n");
	return (nodeptr);
}