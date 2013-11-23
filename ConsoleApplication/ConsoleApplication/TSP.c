#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "TSP.h"
#include "ants.h"
#include "InOut.h"
#include "utilities.h"
#include "ls.h"

#define M_PI 3.14159265358979323846264

long int numberOfCities;
struct problem instance;

long int (*distanceFunction) (long int, long int);

static double dtrunc(double x){
	int k;
	k = (int) x;
	x = (double) k;
	return x;
}

long int getRoundDistance(long int i, long int j){
	double xd = instance.nodesArray[i].x - instance.nodesArray[j].x;
	double yd = instance.nodesArray[i].y - instance.nodesArray[j].y;
	double r  = sqrt(xd*xd + yd*yd) + 0.5;
	return (long int) r;
}

long int getCeilingDistance(long int i, long int j){
	double xd = instance.nodesArray[i].x - instance.nodesArray[j].x;
	double yd = instance.nodesArray[i].y - instance.nodesArray[j].y;
	double r  = sqrt(xd*xd + yd*yd) + 0.000000001;
	return (long int)r;
}

long int getGeometricDistance(long int i, long int j){
	double deg, min;
	double lati, latj, longi, longj;
	double q1, q2, q3;
	long int dd;
	double x1 = instance.nodesArray[i].x, x2 = instance.nodesArray[j].x, 
	y1 = instance.nodesArray[i].y, y2 = instance.nodesArray[j].y;

	deg = dtrunc (x1);
	min = x1 - deg;
	lati = M_PI * (deg + 5.0 * min / 3.0) / 180.0;
	deg = dtrunc (x2);
	min = x2 - deg;
	latj = M_PI * (deg + 5.0 * min / 3.0) / 180.0;

	deg = dtrunc (y1);
	min = y1 - deg;
	longi = M_PI * (deg + 5.0 * min / 3.0) / 180.0;
	deg = dtrunc (y2);
	min = y2 - deg;
	longj = M_PI * (deg + 5.0 * min / 3.0) / 180.0;

	q1 = cos (longi - longj);
	q2 = cos (lati - latj);
	q3 = cos (lati + latj);
	dd = (int) (6378.388 * acos (0.5 * ((1.0 + q1) * q2 - (1.0 - q1) * q3)) + 1.0);
	return dd;
}

long int getAttDistance(long int i, long int j){
	double xd = instance.nodesArray[i].x - instance.nodesArray[j].x;
	double yd = instance.nodesArray[i].y - instance.nodesArray[j].y;
	double rij = sqrt ((xd * xd + yd * yd) / 10.0);
	double tij = dtrunc (rij);
	long int dij;

	if (tij < rij){
		dij = (int) tij + 1;
	} else {
		dij = (int) tij;
	}
	return dij;
}

long int **computeDistanceMatrix(void){
	long int i;
	long int j;
	long int ** matrix;
	matrix = malloc(sizeof(long int) * numberOfCities * numberOfCities +
		sizeof(long int *) * numberOfCities);

	if(matrix == NULL){
		fprintf(stderr, "Out of memory, exit.");
		exit(1);
	}

	for (i = 0; i < numberOfCities; i++){
		matrix[i] = (long int*) (matrix + numberOfCities) + i * numberOfCities;
		for (j = 0; j < numberOfCities; j++){
			matrix[i][j] = distanceFunction(i, j);
		}
	}
	return matrix;
}

long int **computeNearestNeighboursList(void){
	long int listDepth;
	long int node;
	long int i;
	long int **nearestNeighbourList;
	long int *distanceVector;
	long int *helpVector;

	printf("Computing nearest neighbours list \n");
	listDepth = MAX(nearest_neighbours_maximal_depth, number_of_ants);
	if (listDepth >= numberOfCities){
		listDepth = numberOfCities -1;
	}
	printf("listDepth = %ld ... \n",listDepth);

	nearestNeighbourList = malloc(sizeof(long int) * numberOfCities * listDepth +
		numberOfCities * sizeof(long int *)); 
	if (nearestNeighbourList == NULL){
		exit(EXIT_FAILURE);
	}
	
	distanceVector = calloc(numberOfCities, sizeof(long int));
	helpVector = calloc(numberOfCities, sizeof(long int));

	for (node = 0; node < numberOfCities; node++){
		nearestNeighbourList[node] = (long int *)(nearestNeighbourList + numberOfCities) + 
			node * listDepth;
		for (i = 0; i < numberOfCities; i++){
			distanceVector[i] = instance.distanceMatrix[node][i];
			helpVector[i] = i;
		}
		distanceVector[node] = LONG_MAX;

		applyQuickSort(distanceVector, helpVector, 0, numberOfCities - 1);

		for (i = 0; i < listDepth; i++){
			nearestNeighbourList[node][i] = helpVector[i];
		}
	}
	free(distanceVector);
	free(helpVector);
	printf("\n    .. done\n");
	return nearestNeighbourList;
}

long int computeTourLength(long int *tour){
	int i = 0;
	long int tour_length = 0;

	for (i = 0; i < numberOfCities; i++){
		tour_length += instance.distanceMatrix[tour[i]][tour[i + 1]];
	}
	return tour_length;
}
