#define PROGRAM_IDENTIFIER "\nACO algorithms for the TSP\n"
#define LINE_BUF_LEN 100

extern long int tryCounter;
extern long int maximumIndependentTries;
extern long int constructedToursCounter;
extern long int iterationCounter;

extern long int bestSolutionIteration;
extern long int restartIteration;
extern double branchingFactorParameter;

struct point *readInstanceFile(const char *fileName);
void initProgram(long int argc, char *argv[]);
void setDefaultParameters();
void showParameters(void);
void initializeVariablesForTrial(long int tryNumber);
