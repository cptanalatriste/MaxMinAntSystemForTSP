#include <stdio.h>
#include <stdlib.h>

#include "utilities.h"

void swap_items(long int v[], long int v2[], long int i, long int j){
	long int tmp;
	tmp = v[i];
	v[i] = v[j];
	v[j] = tmp;
	tmp = v2[i];
	v2[i] = v2[j];
	v2[j] = tmp;
}

void apply_quick_sort(long int v[], long int v2[], long int left, long int right){
  long int k, last;

  if (left >= right){ 
    return;
  }
  swap_items(v, v2, left, (left + right)/2);
  last = left;
  for (k=left+1; k <= right; k++){
    if (v[k] < v[left]){
		swap_items(v, v2, ++last, k);
	}
  }
  swap_items(v, v2, left, last);
  apply_quick_sort(v, v2, left, last);
  apply_quick_sort(v, v2, last+1, right);
}

double **generate_double_matrix(long int n, long int m){
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

double generate_random_between_0_and_1(long *currentSeed){
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