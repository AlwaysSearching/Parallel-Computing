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
	double tstart = 0.0;
	double ttime = 0.0;

	// find all primes from 2 to N.
	int N = atoi(argv[1]);
	int mid = floor((N + 1)/2);
	
	// Use t number of threads
	int t = atoi(argv[2]);
	
	// allocate memory for a pointer to the list of primes. 
	char* primes = (char  *) malloc((N-2)*sizeof(char));
	memset(primes, 0, (N-2)*sizeof(char));	
	
	tstart = omp_get_wtime();

	int i;
	int k;

#	pragma omp parallel for shared(primes) private(k, i) num_threads(t) schedule(static, 1)
	for (i = 2; i <= mid; i++) {
		if (primes[i - 2] ==0) {
			for (k = 2*i; k < N; k += i) {
				primes[k-2] = 1;
			}
		}
	}
	
	ttime = omp_get_wtime() - tstart;
	printf("Time take for the main part: %f\n", ttime);
	
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
