#define MAXIMUM_NUMBER_OF_ANTS 1024
#define MAXIMUM_NEAREST_NEIGHBOURS 512

#define HEURISTIC(m, n) (1.0/((double)instance.distance_matrix[m][n] + 0.1))

typedef struct{
	long int *tour;
	char *visited;
	long int tour_length;
} ant_struct;

extern ant_struct *ant_colony;
extern ant_struct *best_so_far_ant;
extern ant_struct *restart_best_ant;

extern double **pheromone_matrix;
extern double **pheromone_times_heuristic_matrix;

extern double *selection_probabilities;

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
void empty_ant_memory(ant_struct *ant);
void place_ant(ant_struct *ant, long int number_of_construction_steps);
void move_to_closest_city(ant_struct *ant, long int construction_step_phase);
void calculate_pheromone_times_heuristic_matrix(void);
void choose_next_city_and_move(ant_struct *ant, long int construction_step);
void choose_best_next_city(ant_struct* ant, long int construction_step);
void choose_global_best_next_city(ant_struct* ant, long int construction_step);
long int get_best_ant_from_iteration(void);
void transfer_solution(ant_struct *from, ant_struct *to);
void evaporate_pheromone_for_mmas(void);
void reinforce_ant_solution_edges(ant_struct *ant);
void calculate_pheromone_times_heuristic_for_neighbours(void);