#include <stdio.h>
#include <stdlib.h>

#include "utilities.h"

void swapItems(long int v[], long int v2[], long int i, long int j){
	long int tmp;
	tmp = v[i];
	v[i] = v[j];
	v[j] = tmp;
	tmp = v2[i];
	v2[i] = v2[j];
	v2[j] = tmp;
}

void applyQuickSort(long int v[], long int v2[], long int left, long int right){
  long int k, last;

  if (left >= right){ 
    return;
  }
  swapItems(v, v2, left, (left + right)/2);
  last = left;
  for (k=left+1; k <= right; k++){
    if (v[k] < v[left]){
		swapItems(v, v2, ++last, k);
	}
  }
  swapItems(v, v2, left, last);
  applyQuickSort(v, v2, left, last);
  applyQuickSort(v, v2, last+1, right);
}

double **generateDoubleMatrix(long int n, long int m){
	long int i;
	double **matrix;

	if((matrix = malloc(sizeof(double) * n * m +
			sizeof(double *) * n)) == NULL){
		printf("Out of memory, exit.");
		exit(EXIT_FAILURE);
	}
	for ( i = 0 ; i < n ; i++ ) {
		matrix[i] = (double *)(matrix + n) + i*m;
	}
	return matrix;
}

double generateRandomBetween0and1(long *currentSeed){
	long k;
	double answer;

	k =(*currentSeed)/IQ;
	*currentSeed = IA * (*currentSeed - k * IQ) - IR * k;
	if (*currentSeed < 0 ) {
		*currentSeed += IM;
	}
	answer = AM * (*currentSeed);
	return answer;
}