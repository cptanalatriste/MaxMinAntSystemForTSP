#define MAXIMUM_NUMBER_OF_ANTS 1024
#define MAXIMUM_NEAREST_NEIGHBOURS 512

typedef struct{
	long int *tour;
	char *visited;
	long int tourLength;
} antStruct;

extern antStruct *antColony;
extern antStruct *bestSoFarAnt;
extern antStruct *restartBestAnt;

extern double **pheromoneMatrix;
extern double **pheromoneTimesHeuristicMatrix;

extern double *probOfSelection;

extern long int numberOfAnts;
extern long int nearestNeighboursListLength;

extern double evaporationParameter;
extern double trailImportance;
extern double heuristicValueImportance;
extern double bestChoiceProbability;

extern long int iterationsToUpdateBestAnt;
extern long int maxMinAntSystemFlag;
extern long int numberOfElitistAnts;

extern double maximumPheromoneTrail;
extern double minimumPheromoneTrail;

void allocateAntColonyMemory(void);
void initializePheromoneTrails(double initialValue);
long int getSomeNearestNeighbourTourLength(void);
void emptyAntMemory(antStruct *ant);
void placeAnt(antStruct *ant, long int numberOfConstructionSteps);
void moveToClosestCity(antStruct *ant, long int constructionStepPhase);