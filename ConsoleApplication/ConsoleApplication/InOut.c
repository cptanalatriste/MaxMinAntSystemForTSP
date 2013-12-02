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

long int try_counter;
long int constructed_tours_counter;
long int iteration_counter;

long int best_solution_iteration;
long int restart_best_solution_iteration;

long int restart_iteration;

long int maximum_independent_tries;
long int maximum_tours_one_try;
long int seed;
double maximumTimeForOneTry;
long int optimal_solution;

double branching_factor_parameter;
double average_branching_factor;
double branching_factor_on_best_solution;

double branchingFactorLimit;

long int *bestInTry;
long int *bestFoundAt;
double *timeBestFound;
double *timeTotalRun;

void init_program(long int argc, char *argv[]){
	printf(PROGRAM_IDENTIFIER);
	set_default_parameters();
	bestInTry = calloc(maximum_independent_tries, sizeof(long int));
	bestFoundAt = calloc(maximum_independent_tries, sizeof(long int));
	timeBestFound = calloc(maximum_independent_tries, sizeof(double));
	timeTotalRun = calloc(maximum_independent_tries, sizeof(double));

	printf("Reading problem data - Starting... \n\n");
	//Hard-coded value. Replace later for command line parameter.
	instance.nodes_array = read_instance_file("ulysses16.tsp");
	//instance.nodes_array = read_instance_file("eil51.tsp");

	printf("Reading problem data - Done ... \n\n");

	nearest_neighbours_maximal_depth = MIN(number_of_cities -1, 
		nearest_neighbours_maximal_depth);
	assert(number_of_ants < MAXIMUM_NUMBER_OF_ANTS);
	assert(nearest_neighbours_list_length < MAXIMUM_NEAREST_NEIGHBOURS);
	assert(nearest_neighbours_list_length > 0);
	assert(nearest_neighbours_maximal_depth > 0);

	printf("calculating distance matrix ..\n\n");
	instance.distance_matrix = compute_distance_matrix();
	printf(" .. done\n");
	show_parameters();
	printf("allocate ants' memory ..\n\n");
	allocate_ant_colony_memory();
	printf(" .. done\n");
}

void show_parameters(void){
	fprintf(stdout, "\nParameter-settings: \n\n");

	fprintf(stdout, "localSearchFlag \t\t %ld\n", local_search_flag);
	fprintf(stdout, "nearestNeighboursMaximalDepth \t\t %ld\n", nearest_neighbours_maximal_depth);
	fprintf(stdout, "numberOfAnts \t\t %ld\n", number_of_ants);
	fprintf(stdout, "nearestNeighboursListLength \t\t %ld\n", nearest_neighbours_list_length);
	fprintf(stdout, "trailImportance \t\t %ld\n", trail_importance);
	fprintf(stdout, "heuristicValueImportance \t\t %ld\n", heuristic_value_importance);
	fprintf(stdout, "evaporationParameter \t\t %ld\n", evaporation_parameter);
	fprintf(stdout, "bestChoiceProbability \t\t %ld\n", best_choice_probability);
	fprintf(stdout, "maximumIndependentTries \t\t %ld\n", maximum_independent_tries);
	fprintf(stdout, "maximumToursOneTry \t\t %ld\n", maximum_tours_one_try);
	fprintf(stdout, "seed \t\t %ld\n", seed);
	fprintf(stdout, "maximumTimeForOneTry \t\t %ld\n", maximumTimeForOneTry);
	fprintf(stdout, "optimalSolution\t\t %ld\n", optimal_solution);
	fprintf(stdout, "maxMinAntSystemFlag \t\t %ld\n", max_min_ant_system_flag);
	fprintf(stdout, "numberOfElitistAnts \t\t %ld\n", number_of_elitist_ants);

	fprintf(stdout, "\n");
}


void set_default_parameters(void){
	dont_look_bits_flag = TRUE;
	local_search_flag = 3;
	nearest_neighbours_maximal_depth = 20;
	number_of_ants = 25;
	nearest_neighbours_list_length = 20;

	trail_importance = 1.0;
	heuristic_value_importance = 2.0;
	evaporation_parameter = 0.5;
	best_choice_probability = 0.0;
	maximum_independent_tries = 10;

	//cgavidia: This parameter defines execution time
	maximum_tours_one_try = 500;

	seed = (long int) time(NULL);
	maximumTimeForOneTry = 10.0;

	optimal_solution = 1;
	branchingFactorLimit = 1.00001;
	iterations_to_update_best_ant = INFINITY;
	max_min_ant_system_flag = TRUE;
	number_of_elitist_ants = 0;	
}

//TODO: Change variable names
struct point *read_instance_file(const char *fileName){
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
			strcpy(instance.instance_name, buf);
			printf("%s \n", instance.instance_name); 
			buf[0]=0;
		} else if ( strcmp("NAME:", buf) == 0 ) {
			fscanf(tsp_file, "%s", buf);
			strcpy(instance.instance_name, buf);
			printf("%s \n", instance.instance_name);
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
			fscanf(tsp_file, "%ld", &number_of_cities);
			instance.number_of_cities = number_of_cities;
			printf("%ld\n", number_of_cities);
			assert ( number_of_cities > 2 && number_of_cities < 6000);
			buf[0]=0;
		} else if ( strcmp("DIMENSION:", buf) == 0 ) {
			fscanf(tsp_file, "%ld", &number_of_cities);
			instance.number_of_cities = number_of_cities;
			printf("%ld\n", number_of_cities);
			assert ( number_of_cities > 2 && number_of_cities < 6000);
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
				distance_function = get_round_distance;
			} else if ( strcmp("CEIL_2D", buf) == 0 ) {
				distance_function = get_ceiling_distance;
			} else if ( strcmp("GEO", buf) == 0 ) {
				distance_function = get_geometric_distance;
			} else if ( strcmp("ATT", buf) == 0 ) {
				distance_function = get_att_distance;
			} else {
				fprintf(stderr,"EDGE_WEIGHT_TYPE %s not implemented\n",buf);
			}
			strcpy(instance.edge_weight_type, buf);
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
				distance_function = get_round_distance;
			} else if ( strcmp("CEIL_2D", buf) == 0 ) {
				distance_function = get_ceiling_distance;
			} else if ( strcmp("GEO", buf) == 0 ) {
				distance_function = get_geometric_distance;
			} else if ( strcmp("ATT", buf) == 0 ) {
				distance_function = get_att_distance;
			} else {
				fprintf(stderr,"EDGE_WEIGHT_TYPE %s not implemented\n",buf);
				exit(1);
			}
			strcpy(instance.edge_weight_type, buf);
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

	if( (nodeptr = malloc(sizeof(struct point) * number_of_cities)) == NULL ){
		exit(EXIT_FAILURE);
	} else {
		for ( i = 0 ; i < number_of_cities ; i++ ) {
			fscanf(tsp_file,"%ld %lf %lf", &j, &nodeptr[i].x, &nodeptr[i].y );
		}
	}
	printf("number of cities is %ld\n",number_of_cities);
	printf("\n... done\n");
	return (nodeptr);
}

double compute_lambda_branching_factor(double lamda){
	long int i;
	long int m;
	double minimum;
	double maximum;
	double cut_off;
	double average;
	double *branches_number;

	branches_number =  calloc(number_of_cities, sizeof(double));
	for(m = 0; m < number_of_cities; m++){
		maximum = pheromone_matrix[m][instance.nearest_neighbours_list[m][1]];
		minimum = pheromone_matrix[m][instance.nearest_neighbours_list[m][1]];
		for (i = 1; i < nearest_neighbours_list_length; i++){
			if(pheromone_matrix[m][instance.nearest_neighbours_list[m][i]] > maximum){
				maximum = pheromone_matrix[m][instance.nearest_neighbours_list[m][i]];
			}
			if(pheromone_matrix[m][instance.nearest_neighbours_list[m][i]] < minimum){
				minimum = pheromone_matrix[m][instance.nearest_neighbours_list[m][i]];
			}
		}
		cut_off = minimum + lamda*(maximum - minimum);
		for (i = 0; i< nearest_neighbours_list_length; i++){
			if(pheromone_matrix[m][instance.nearest_neighbours_list[m][i]] > cut_off){
				branches_number[m] += 1;
			}
		}
	}
	average = 0.0;
	for (m = 0; m < number_of_cities; m++){
		average += branches_number[m];
	}
	free(branches_number);

	return (average/(double)(number_of_cities *2));
}