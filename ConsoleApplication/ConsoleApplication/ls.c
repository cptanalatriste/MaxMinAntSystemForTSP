#include <stdio.h>
#include <stdlib.h>

#include "TSP.h"
#include "utilities.h"
#include "ls.h"

long int local_search_flag;
long int dont_look_bits_flag;
long int nearest_neighbours_maximal_depth;

void apply_two_opt_first(long int *tour){
	//TODO: Improve code formatting
    long int c1, c2;             /* cities considered for an exchange */
    long int s_c1, s_c2;         /* successor cities of c1 and c2     */
    long int p_c1, p_c2;         /* predecessor cities of c1 and c2   */   
    long int pos_c1, pos_c2;     /* positions of cities c1, c2        */
    long int i, j, h, l;
    long int improvement_flag, improve_node, help, n_improves = 0, n_exchanges=0;
    long int h1=0, h2=0, h3=0, h4=0;
    long int radius;             /* radius of nn-search */
    long int gain = 0;
    long int *random_vector;
    long int *pos;               /* positions of cities in tour */ 
    long int *dlb;               /* vector containing don't look bits */ 
  
    pos = malloc(numberOfCities * sizeof(long int));
    dlb = malloc(numberOfCities * sizeof(long int));

    for ( i = 0 ; i < numberOfCities ; i++ ) {
		pos[tour[i]] = i;
		dlb[i] = FALSE;
    }

    improvement_flag = TRUE;
    random_vector = generate_random_permutation( numberOfCities );

    while ( improvement_flag ) {
		improvement_flag = FALSE;

		for (l = 0 ; l < numberOfCities; l++) {
			c1 = random_vector[l]; 
			if ( dont_look_bits_flag  && dlb[c1] )
				continue;
			improve_node = FALSE;

			pos_c1 = pos[c1];
			s_c1 = tour[pos_c1+1];
			radius = instance.distanceMatrix[c1][s_c1];

			/* First search for c1's nearest neighbours, use successor of c1 */
			for ( h = 0 ; h < nearest_neighbours_maximal_depth ; h++ ) {
				c2 = instance.nearestNeighboursList[c1][h]; /* exchange partner, determine its position */
				if ( radius > instance.distanceMatrix[c1][c2] ) {
					s_c2 = tour[pos[c2]+1];
					gain =  - radius + instance.distanceMatrix[c1][c2] + 
						instance.distanceMatrix[s_c1][s_c2] - instance.distanceMatrix[c2][s_c2];
					if ( gain < 0 ) {
						h1 = c1;
						h2 = s_c1;
						h3 = c2;
						h4 = s_c2; 
						improve_node = TRUE;
						goto exchange2opt;
					}
				}
				else 
					break;
			}      
			/* Search one for next c1's h-nearest neighbours, use predecessor c1 */
			if (pos_c1 > 0)
				p_c1 = tour[pos_c1-1];
			else 
				p_c1 = tour[numberOfCities-1];
			radius = instance.distanceMatrix[p_c1][c1];
			for ( h = 0 ; h < nearest_neighbours_maximal_depth ; h++ ) {
				c2 = instance.nearestNeighboursList[c1][h];  /* exchange partner, determine its position */
				if ( radius > instance.distanceMatrix[c1][c2] ) {
					pos_c2 = pos[c2];
					if (pos_c2 > 0)
						p_c2 = tour[pos_c2-1];
					else 
						p_c2 = tour[numberOfCities-1];
					if ( p_c2 == c1 )
						continue;
					if ( p_c1 == c2 )
						continue;
					gain =  - radius + instance.distanceMatrix[c1][c2] + 
						instance.distanceMatrix[p_c1][p_c2] - instance.distanceMatrix[p_c2][c2];
					if ( gain < 0 ) {
						h1 = p_c1; h2 = c1; h3 = p_c2; h4 = c2; 
						improve_node = TRUE;
						goto exchange2opt;
					}
				}
				else 
					break;
			}      
			if (improve_node) {
				exchange2opt:
				n_exchanges++;
				improvement_flag = TRUE;
				dlb[h1] = FALSE; dlb[h2] = FALSE;
				dlb[h3] = FALSE; dlb[h4] = FALSE;


				/* Now perform move */
				if ( pos[h3] < pos[h1] ) {
					help = h1;
					h1 = h3;
					h3 = help;
					help = h2;
					h2 = h4; 
					h4 = help;
				}
				if ( pos[h3] - pos[h2] < numberOfCities / 2 + 1) {
					/* reverse inner part from pos[h2] to pos[h3] */
					i = pos[h2];
					j = pos[h3];
					while (i < j) {
						c1 = tour[i];
						c2 = tour[j];
						tour[i] = c2;
						tour[j] = c1;
						pos[c1] = j;
						pos[c2] = i;
						i++; j--;
					}
				} else {
					/* reverse outer part from pos[h4] to pos[h1] */
					i = pos[h1]; 
					j = pos[h4];
					if ( j > i )
						help = numberOfCities - (j - i) + 1;
					else 
						help = (i - j) + 1;
					help = help / 2;
					for ( h = 0 ; h < help ; h++ ) {
						c1 = tour[i];
						c2 = tour[j];
						tour[i] = c2;
						tour[j] = c1;
						pos[c1] = j;
						pos[c2] = i;
						i--; j++;
						if ( i < 0 )
							i = numberOfCities-1;
						if ( j >= numberOfCities )
							j = 0;
					}
					tour[numberOfCities] = tour[0];
				}
			} else {
				dlb[c1] = TRUE;
			}
		}
		if ( improvement_flag ) {
			n_improves++;
		}
    }
    free( random_vector );
    free( dlb );
    free( pos );
}

long int * generate_random_permutation( long int arrayLenght ){
	long int  i, help, node, tot_assigned = 0;
	double    rnd;
	long int  *r;

	r = malloc(arrayLenght * sizeof(long int));  

	for ( i = 0 ; i < arrayLenght; i++){ 
		r[i] = i;
	}
	for ( i = 0 ; i < arrayLenght ; i++ ) {
		rnd  = generateRandomBetween0and1( &seed );
		node = (long int) (rnd  * (arrayLenght - tot_assigned)); 
		help = r[i];
		r[i] = r[i+node];
		r[i+node] = help;
		tot_assigned++;
	}
   return r;
}