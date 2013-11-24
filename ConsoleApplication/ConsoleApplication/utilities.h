#define INFINITY LONG_MAX
#define TRUE 1
#define FALSE 0
#define MIN(x,y) ((x)<=(y)?(x):(y))
#define MAX(x, y) ((x)>=(y)?(x):(y))

//Constants for random number generator
#define IA 16807
#define IQ 127773
#define IR 2836
#define IM 2147483647
#define AM (1.0/IM)

extern long int seed;

void swap_items(long int v[], long int v2[], long int i, long int j);
void apply_quick_sort(long int v[], long int v2[], long int left, long int right);
double **generate_double_matrix(long int n, long int m);
double generate_random_between_0_and_1(long *current_seed);