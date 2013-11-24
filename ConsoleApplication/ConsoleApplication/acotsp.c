#include <stdio.h>
#include <limits.h>

#include "InOut.h"
#include "TSP.h"
#include "ants.h"
#include "utilities.h"
#include "InOut.h"

long int is_termination_condition_met(void){
	return (constructed_tours_counter >= maximum_tours_one_try) &&
		(best_so_far_ant->tour_length <= optimal_solution);
}

void construct_solutions(void){
	long int k;
	long int construction_step_counter;

	for (k=0; k < number_of_ants; k++){
		empty_ant_memory(&ant_colony[k]);
	}

	construction_step_counter = 0;
	for (k = 0; k < number_of_ants; k++){
		place_ant(&ant_colony[k], construction_step_counter);
	}

	while(construction_step_counter < number_of_cities - 1){
		construction_step_counter++;
		choose_next_city_and_move(&ant_colony[k], construction_step_counter);
	}

}

int main(int argc, char *argv[]){
	init_program(argc, argv);
	instance.nearest_neighbours_list = compute_nearest_neighbours_list();
	pheromone_matrix = generate_double_matrix(number_of_cities, 
		number_of_cities);
	pheromone_times_heuristic_matrix = generate_double_matrix(number_of_cities, 
		number_of_cities);
	printf("Initialization finished \n");

	for (try_counter = 0; try_counter < maximum_independent_tries; try_counter++){
		initialize_variables_for_trial(try_counter);

		while(!is_termination_condition_met()){
			construct_solutions();
		}
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
	minimum_pheromone_trail = maximum_pheromone_trail/(2. * number_of_cities);
	initialize_pheromone_trails(maximum_pheromone_trail);
	calculate_pheromone_times_heuristic_matrix();
}

