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

ant_struct *ant_colony;
ant_struct *best_so_far_ant;
ant_struct *restart_best_ant;

double **pheromone_matrix;
double **pheromone_times_heuristic_matrix;

double *probOfSelection;

void allocate_ant_colony_memory(void){
	long int i;
	ant_colony = malloc(sizeof(ant_struct)*number_of_ants + 
		sizeof(ant_struct *)* number_of_ants);
	if (ant_colony == NULL){
		printf("Out of memory. exit");
		exit(1);
	}

	for (i = 0; i < number_of_ants; i++){
		ant_colony[i].tour = calloc(number_of_cities + 1, sizeof(long int));
		ant_colony[i].visited = calloc(number_of_cities, sizeof(char));
	}
	
	best_so_far_ant = malloc(sizeof(ant_struct));
	if (best_so_far_ant == NULL){
		printf("Out of memory. exit");
		exit(1);
	}
	best_so_far_ant->tour = calloc(number_of_cities + 1, sizeof(long int));
	best_so_far_ant->visited = calloc(number_of_cities, sizeof(char));

	restart_best_ant = malloc(sizeof(ant_struct));
	if (restart_best_ant == NULL){
		printf("Out of memory. exit");
		exit(1);
	}
	restart_best_ant->tour = calloc(number_of_cities + 1, sizeof(long int));
	restart_best_ant->visited = calloc(number_of_cities, sizeof(char));

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

	while (constructionStepCounter < number_of_cities -1){
		constructionStepCounter ++;
		move_to_closest_city(&ant_colony[0], constructionStepCounter);
	}
	constructionStepCounter = number_of_cities;
	ant_colony[0].tour[number_of_cities] = ant_colony[0].tour[0];
	if(local_search_flag){
		apply_two_opt_first(ant_colony[0].tour);
	}
	constructed_tours_counter += 1;
	ant_colony[0].tour_length = compute_tour_length(ant_colony[0].tour);

	help = ant_colony[0].tour_length;
	empty_ant_memory(&ant_colony[0]);
	return help;
}

void move_to_closest_city(ant_struct *ant, long int constructionStepPhase){
	long int city;
	long int currentCity;
	long int nextCity;
	long int minimumDistance;

	nextCity = number_of_cities;
	currentCity = ant->tour[constructionStepPhase -1];
	minimumDistance = INFINITY;

	for(city = 0; city < number_of_cities; city++){
		if(!ant->visited[city] &&  
			instance.distance_matrix[currentCity][city] < minimumDistance){
			nextCity = city;
		}
	}
	ant->tour[constructionStepPhase] = nextCity;
	ant->visited[nextCity] = TRUE;	
}

void empty_ant_memory(ant_struct *ant){
	long int i;
	for (i = 0; i < number_of_cities; i++){
		ant->visited[i] = FALSE;
	}
}

void place_ant(ant_struct *ant, long int numberOfConstructionSteps){
	long int random;
	random = (long int)(generate_random_between_0_and_1(&seed) 
		* (double) number_of_cities);

	ant->tour[numberOfConstructionSteps] = random;
	ant->visited[random] = TRUE;
}

void initialize_pheromone_trails(double initialValue){
	long int i;
	long int j;
	for (i = 0; i < number_of_cities; i++){
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
	for (i = 0; i < number_of_cities; i++){
		for (j = 0; j < i; j++){
			pheromone_times_heuristic_matrix[i][j] = pow(pheromone_matrix[i][j], trail_importance) *
				pow(HEURISTIC(i, j), heuristic_value_importance);
			pheromone_times_heuristic_matrix[j][i] = pheromone_times_heuristic_matrix[i][j];
		}
	}
}

void choose_next_city_and_move(ant_struct *ant, long int constructionStep){
	long int i;
	long int help;
	long int current_city;
	double random;
	double partial_sum = 0.0;
	double sum_prob = 0.0;

	double *selection_probabilities;

	if(best_choice_probability > 0.0 && 
		generate_random_between_0_and_1(&seed) < best_choice_probability){

	}
}