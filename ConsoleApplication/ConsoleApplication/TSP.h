#include "InOut.h"

struct point{
	double x;
	double y;
};

struct problem{
	char instance_name[LINE_BUF_LEN];
	char edge_weight_type[LINE_BUF_LEN];
	long int optimal_tour_lenght;
	long int number_of_cities;
	long int number_nearest_neighbours;
	struct point *nodes_array;
	long int **distance_matrix;
	long int **nearest_neighbours_list;
};

extern struct problem instance;

long int number_of_cities;

long int (*distance_function) (long int, long int);

long int get_round_distance(long int i, long int j);

long int get_ceiling_distance(long int i, long int j);

long int get_geometric_distance(long int i, long int j);

long int get_att_distance(long int i, long int j);

long int **compute_distance_matrix(void);

long int **compute_nearest_neighbours_list(void);

long int compute_tour_length(long int *tour);
