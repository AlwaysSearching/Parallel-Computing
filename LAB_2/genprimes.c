/*
 * =====================================================================================
 *
 *       Filename:  genprime.c
 *
 *    Description:  Lab 2 for Parallel Computing Spring 2020. OpenMP implimentation of
 *    	   	    the Sieves of Eratosthenes algorithms.
 *
 *        Created:  04/18/2020  
 *
 *         Author:  Sam Fieldman 
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <omp.h>


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  Main function for sieves of Eratosthenes.   
 * =====================================================================================
 */
int
main ( int argc, char *argv[] )
{
	// find all primes from 2 to N.
	int N = atoi(argv[1]);
	int mid = floor((N + 1)/2);
	
	// Use t number of threads
	int t = atoi(argv[2]);
	
	// allocate memory for a pointer to the list of primes. 
	int* primes = (int  *) malloc((N-2)*sizeof(int));
	memset(primes, 0, (N-2)*sizeof(int));	
	int i, k;
	for (i = 2; i <= mid; i++) {
		if(primes[i-2] == 0) {
#			pragma omp parallel for num_threads(t) shared(primes, i, k) private(k)
			for (k = 2*i; k < N; k +=i) {
				primes[k-2] = 1;
			}
		}
	}
		
	// open file and write primes.
	char filename[100] = "";
	strcat(filename, argv[1]);
	strcat(filename, ".txt");
	FILE* file; 
	
	if( !(file = fopen(filename,"w"))){
                printf("Cannot create file %s\n", filename);
                exit(1);
        }	
	
	for(int i = 0; i < N-2; i++){
		if (primes[i] == 0) {
			fprintf(file, "%d\n", i+2);
		}        
	}	
		
	fclose(file);	
	
	free(primes);

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */


