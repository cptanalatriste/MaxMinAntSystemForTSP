#include "InOut.h"

struct point{
	double x;
	double y;
};

struct problem{
	char instanceName[LINE_BUF_LEN];
	char edgeWeightType[LINE_BUF_LEN];
	long int optimalTourLenght;
	long int numberOfCities;
	long int numberNearestNeighbours;
	struct point *nodesArray;
	long int **distanceMatrix;
	long int **nearestNeighboursList;
};

extern struct problem instance;

long int numberOfCities;

long int (*distanceFunction) (long int, long int);

long int getRoundDistance(long int i, long int j);

long int getCeilingDistance(long int i, long int j);

long int getGeometricDistance(long int i, long int j);

long int getAttDistance(long int i, long int j);

long int **computeDistanceMatrix(void);

long int **computeNearestNeighboursList(void);

long int computeTourLength(long int *tour);
