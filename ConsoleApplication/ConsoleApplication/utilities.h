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

void swapItems(long int v[], long int v2[], long int i, long int j);
void applyQuickSort(long int v[], long int v2[], long int left, long int right);
double **generateDoubleMatrix(long int n, long int m);
double generateRandomBetween0and1(long *currentSeed);