#define MAXIMUM_NUMBER_OF_ANTS 1024
#define MAXIMUM_NEAREST_NEIGHBOURS 512

#define HEURISTIC(m, n) (1.0/((double)instance.distanceMatrix[m][n] + 0.1))

typedef struct{
	long int *tour;
	char *visited;
	long int tour_length;
} antStruct;

extern antStruct *ant_colony;
extern antStruct *best_so_far_ant;
extern antStruct *restart_best_ant;

extern double **pheromone_matrix;
extern double **pheromone_times_heuristic_matrix;

extern double *prob_of_selection;

extern long int number_of_ants;
extern long int nearest_neighbours_list_length;

extern double evaporation_parameter;
extern double trail_importance;
extern double heuristic_value_importance;
extern double best_choice_probability;

extern long int iterations_to_update_best_ant;
extern long int max_min_ant_system_flag;
extern long int number_of_elitist_ants;

extern double maximum_pheromone_trail;
extern double minimum_pheromone_trail;

void allocate_ant_colony_memory(void);
void initialize_pheromone_trails(double initial_value);
long int get_some_nearest_neighbour_tour_length(void);
void empty_ant_memory(antStruct *ant);
void place_ant(antStruct *ant, long int number_of_construction_steps);
void move_to_closest_city(antStruct *ant, long int construction_step_phase);
void calculate_pheromone_times_heuristic_matrix(void);