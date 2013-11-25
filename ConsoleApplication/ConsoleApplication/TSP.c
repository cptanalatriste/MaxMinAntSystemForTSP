#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "TSP.h"
#include "ants.h"
#include "InOut.h"
#include "utilities.h"
#include "ls.h"

#define M_PI 3.14159265358979323846264

long int number_of_cities;
struct problem instance;

long int (*distance_function) (long int, long int);

static double dtrunc(double x){
	int k;
	k = (int) x;
	x = (double) k;
	return x;
}

long int get_round_distance(long int i, long int j){
	double xd = instance.nodes_array[i].x - instance.nodes_array[j].x;
	double yd = instance.nodes_array[i].y - instance.nodes_array[j].y;
	double r  = sqrt(xd*xd + yd*yd) + 0.5;
	return (long int) r;
}

long int get_ceiling_distance(long int i, long int j){
	double xd = instance.nodes_array[i].x - instance.nodes_array[j].x;
	double yd = instance.nodes_array[i].y - instance.nodes_array[j].y;
	double r  = sqrt(xd*xd + yd*yd) + 0.000000001;
	return (long int)r;
}

long int get_geometric_distance(long int i, long int j){
	double deg, min;
	double lati, latj, longi, longj;
	double q1, q2, q3;
	long int dd;
	double x1 = instance.nodes_array[i].x, x2 = instance.nodes_array[j].x, 
	y1 = instance.nodes_array[i].y, y2 = instance.nodes_array[j].y;

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

long int get_att_distance(long int i, long int j){
	double xd = instance.nodes_array[i].x - instance.nodes_array[j].x;
	double yd = instance.nodes_array[i].y - instance.nodes_array[j].y;
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

long int **compute_distance_matrix(void){
	long int i;
	long int j;
	long int ** matrix;
	matrix = malloc(sizeof(long int) * number_of_cities * number_of_cities +
		sizeof(long int *) * number_of_cities);

	if(matrix == NULL){
		fprintf(stderr, "Out of memory, exit.");
		exit(1);
	}

	for (i = 0; i < number_of_cities; i++){
		matrix[i] = (long int*) (matrix + number_of_cities) + i * number_of_cities;
		for (j = 0; j < number_of_cities; j++){
			matrix[i][j] = distance_function(i, j);
		}
	}
	return matrix;
}

long int **compute_nearest_neighbours_list(void){
	long int listDepth;
	long int node;
	long int i;
	long int **nearestNeighbourList;
	long int *distanceVector;
	long int *helpVector;

	printf("Computing nearest neighbours list \n");
	listDepth = MAX(nearest_neighbours_maximal_depth, nearest_neighbours_list_length);
	if (listDepth >= number_of_cities){
		listDepth = number_of_cities -1;
		//TODO (cgavidia): This a modification of the original algorithm, necesary for small problem sets
		nearest_neighbours_list_length = listDepth;
	}
	printf("listDepth = %ld ... \n",listDepth);

	nearestNeighbourList = malloc(sizeof(long int) * number_of_cities * listDepth +
		number_of_cities * sizeof(long int *)); 
	if (nearestNeighbourList == NULL){
		exit(EXIT_FAILURE);
	}
	
	distanceVector = calloc(number_of_cities, sizeof(long int));
	helpVector = calloc(number_of_cities, sizeof(long int));

	for (node = 0; node < number_of_cities; node++){
		nearestNeighbourList[node] = (long int *)(nearestNeighbourList + number_of_cities) + 
			node * listDepth;
		for (i = 0; i < number_of_cities; i++){
			distanceVector[i] = instance.distance_matrix[node][i];
			helpVector[i] = i;
		}
		distanceVector[node] = LONG_MAX;

		apply_quick_sort(distanceVector, helpVector, 0, number_of_cities - 1);

		for (i = 0; i < listDepth; i++){
			nearestNeighbourList[node][i] = helpVector[i];
		}
	}
	free(distanceVector);
	free(helpVector);
	printf("\n    .. done\n");
	return nearestNeighbourList;
}

long int compute_tour_length(long int *tour){
	int i = 0;
	long int tour_length = 0;

	for (i = 0; i < number_of_cities; i++){
		tour_length += instance.distance_matrix[tour[i]][tour[i + 1]];
	}
	return tour_length;
}
