#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ants.h"
#include "TSP.h"
#include "utilities.h"
#include "InOut.h"
#include "ls.h"

long int number_of_ants;
long int nearest_neighbours_list_length;

double evaporation_parameter;
double trail_importance;
double heuristic_value_importance;
double best_choice_probability;

long int iterations_to_update_best_ant;
long int max_min_ant_system_flag;
long int number_of_elitist_ants;

double maximum_pheromone_trail;
double minimum_pheromone_trail;

antStruct *ant_colony;
antStruct *best_so_far_ant;
antStruct *restart_best_ant;

double **pheromone_matrix;
double **pheromone_times_heuristic_matrix;

double *probOfSelection;

void allocate_ant_colony_memory(void){
	long int i;
	ant_colony = malloc(sizeof(antStruct)*number_of_ants + 
		sizeof(antStruct *)* number_of_ants);
	if (ant_colony == NULL){
		printf("Out of memory. exit");
		exit(1);
	}

	for (i = 0; i < number_of_ants; i++){
		ant_colony[i].tour = calloc(numberOfCities + 1, sizeof(long int));
		ant_colony[i].visited = calloc(numberOfCities, sizeof(char));
	}
	
	best_so_far_ant = malloc(sizeof(antStruct));
	if (best_so_far_ant == NULL){
		printf("Out of memory. exit");
		exit(1);
	}
	best_so_far_ant->tour = calloc(numberOfCities + 1, sizeof(long int));
	best_so_far_ant->visited = calloc(numberOfCities, sizeof(char));

	restart_best_ant = malloc(sizeof(antStruct));
	if (restart_best_ant == NULL){
		printf("Out of memory. exit");
		exit(1);
	}
	restart_best_ant->tour = calloc(numberOfCities + 1, sizeof(long int));
	restart_best_ant->visited = calloc(numberOfCities, sizeof(char));

	probOfSelection = malloc(sizeof(double)*(number_of_ants + 1));
	if (probOfSelection == NULL){
		printf("Out of memory. exit");
		exit(1);
	}
	probOfSelection[number_of_ants] = HUGE_VAL;
}

long int get_some_nearest_neighbour_tour_length(void){
	long int constructionStepCounter;
	long int help;
	empty_ant_memory(&ant_colony[0]);
	
	constructionStepCounter = 0;
	place_ant(&ant_colony[0], constructionStepCounter);

	while (constructionStepCounter < numberOfCities -1){
		constructionStepCounter ++;
		move_to_closest_city(&ant_colony[0], constructionStepCounter);
	}
	constructionStepCounter = numberOfCities;
	ant_colony[0].tour[numberOfCities] = ant_colony[0].tour[0];
	if(local_search_flag){
		apply_two_opt_first(ant_colony[0].tour);
	}
	constructed_tours_counter += 1;
	ant_colony[0].tour_length = computeTourLength(ant_colony[0].tour);

	help = ant_colony[0].tour_length;
	empty_ant_memory(&ant_colony[0]);
	return help;
}

void move_to_closest_city(antStruct *ant, long int constructionStepPhase){
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

void empty_ant_memory(antStruct *ant){
	long int i;
	for (i = 0; i < numberOfCities; i++){
		ant->visited[i] = FALSE;
	}
}

void place_ant(antStruct *ant, long int numberOfConstructionSteps){
	long int random;
	random = (long int)(generateRandomBetween0and1(&seed) 
		* (double) numberOfCities);

	ant->tour[numberOfConstructionSteps] = random;
	ant->visited[random] = TRUE;
}

void initialize_pheromone_trails(double initialValue){
	long int i;
	long int j;
	for (i = 0; i < numberOfCities; i++){
		for (j = 0; j <= i; j ++){
			pheromone_matrix[i][j] = initialValue;
			pheromone_matrix[j][i] = initialValue;
			pheromone_times_heuristic_matrix[i][j] = initialValue;
			pheromone_times_heuristic_matrix[j][i] = initialValue;
		}
	}
}

void calculate_pheromone_times_heuristic_matrix(void){
	long int i;
	long int j;
	printf("Compute total information \n");
	for (i = 0; i < numberOfCities; i++){
		for (j = 0; j < i; j++){
			pheromone_times_heuristic_matrix[i][j] = pow(pheromone_matrix[i][j], trail_importance) *
				pow(HEURISTIC(i, j), heuristic_value_importance);
			pheromone_times_heuristic_matrix[j][i] = pheromone_times_heuristic_matrix[i][j];
		}
	}
}