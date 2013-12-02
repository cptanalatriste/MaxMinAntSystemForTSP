#include <stdio.h>
#include <limits.h>

#include "InOut.h"
#include "TSP.h"
#include "ants.h"
#include "utilities.h"
#include "InOut.h"
#include "ls.h"

long int is_termination_condition_met(void){
	int response = (constructed_tours_counter >= maximum_tours_one_try) ||
		(best_so_far_ant->tour_length <= optimal_solution);
	printf("constructed_tours_counter: %d \n", constructed_tours_counter);
	printf("maximum_tours_one_try: %d \n", maximum_tours_one_try);
	printf("best_so_far_ant->tour_length: %d \n", best_so_far_ant->tour_length);
	printf("optimal_solution: %d \n", optimal_solution);
	printf("response: %d \n", response);
	return response;
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
		for (k = 0; k < number_of_ants; k++){
			choose_next_city_and_move(&ant_colony[k], construction_step_counter);
		}
	}
	construction_step_counter = number_of_cities;
	for (k = 0; k < number_of_ants; k++){
		ant_colony[k].tour[number_of_cities] = ant_colony[k].tour[0];
		ant_colony[k].tour_length = compute_tour_length(ant_colony[k].tour);
	}
	constructed_tours_counter += number_of_ants;
}

void apply_local_search(void){
	//TODO (cgavidia): Currently, we're only soporting 3-opt local search 
	long int k;
	printf("Apply local search to all ants \n");
	for (k = 0; k < number_of_ants; k++){
		apply_three_opt_search(ant_colony[k].tour);
		ant_colony[k].tour_length = compute_tour_length(ant_colony[k].tour);
		if(is_termination_condition_met()){
			return;
		}
	}
}

void update_statiscal_information(void){

	long int iteration_best_ant;

	iteration_best_ant = get_best_ant_from_iteration();
	printf("ant_colony[iteration_best_ant].tour_length : %d \n",ant_colony[iteration_best_ant].tour_length );
	if(ant_colony[iteration_best_ant].tour_length < best_so_far_ant->tour_length){
		transfer_solution(&ant_colony[iteration_best_ant], best_so_far_ant);
		transfer_solution(&ant_colony[iteration_best_ant], restart_best_ant);

		best_solution_iteration = iteration_counter;
		restart_best_solution_iteration = iteration_counter;
		branching_factor_on_best_solution = compute_lambda_branching_factor(branching_factor_parameter);
		average_branching_factor = branching_factor_on_best_solution;
		//There's specif behaviour for MMAS with no local search that is omitted.
	}

	if(ant_colony[iteration_best_ant].tour_length < restart_best_ant->tour_length){
		transfer_solution(&ant_colony[iteration_best_ant], restart_best_ant);
		restart_best_solution_iteration = iteration_counter;
		printf("restart best: %ld, restart_found_best: %ld \n", restart_best_ant->tour_length, 
			restart_best_solution_iteration);
	}
}

void deposite_pheromone_for_mmas(void){
	long int iteration_best_ant;
	printf("MAX-MIN Ant System pheromone deposit\n");
	if(iteration_counter & iterations_to_update_best_ant){
		iteration_best_ant = get_best_ant_from_iteration();
		reinforce_ant_solution_edges(&ant_colony[iteration_best_ant]);
	} else {
		if (iterations_to_update_best_ant == 1 
			&& (iteration_counter -restart_best_solution_iteration >50)){
				reinforce_ant_solution_edges(best_so_far_ant);
		} else {
			reinforce_ant_solution_edges(restart_best_ant);
		}
	}

	if (local_search_flag){
		if(iteration_counter -restart_iteration < 25){
			iterations_to_update_best_ant = 25;
		} else if(iteration_counter -restart_iteration < 75){
			iterations_to_update_best_ant = 5;
		} else if (iteration_counter -restart_iteration < 125){
			iterations_to_update_best_ant = 3;
		} else if(iteration_counter -restart_iteration < 250){
			iterations_to_update_best_ant = 2;
		} else {
			iterations_to_update_best_ant = 1;
		}
	} else {
		iterations_to_update_best_ant = 25;
	}
}

void update_pheromone_trails(void){
	//The pheromone evaporation logic is only implemented for MMAS with local search
	if(local_search_flag){
		evaporate_pheromone_for_mmas();
	}
	deposite_pheromone_for_mmas();
	//There's behaviour for MMAS with no local search that is omitted.
	if(local_search_flag){
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
			if (local_search_flag > 0){
				apply_local_search();
			}
			update_statiscal_information();
			update_pheromone_trails();
			iteration_counter++;
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

