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

void find_primes(int from, int to, int* primes, int* N, int* integers, int t);


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  Main function for sieves of Eratosthenes.   
 * =====================================================================================
 */
int
main ( int argc, char *argv[] )
{
	printf("argv: %s", *argv);	
	
	// find all primes from 2 to N.
	int N = atoi(argv[1]);

	// Use t number of threads
	int t = atoi(argv[2]);
	printf("Program start");	
	
	// allocate memory for a pointer to the list of primes. 
	int* primes = (int  *) malloc(0);
	int prime_count = 0;
	
	
	int slice_size = 2 << 20; // ~1000000
	int from, to;
	
	// call find primes until we reach N
	for (from  = 2; from < N; from += slice_size) {
		to = from + slice_size;
		if (N < to) {
			to = N;
		}
		
		int* integers = (int *) malloc((from-to)*sizeof(int));	
		memset(integers, 1, (from-to)*sizeof(int));

		find_primes(from, to, primes, &prime_count, integers, t);
	}
	
	printf("Found Primes");
	// open file and write primes.
	char filename[100] = "";
	strcat(filename, argv[1]);
	strcat(filename, ".txt");
	FILE* file; 
	
	if( !(file = fopen(filename,"w"))){
                printf("Cannot create file %s\n", filename);
                exit(1);
        }	
		
	for(int i = 0; i <= prime_count; i++){
		fprintf(file, "%d\n", primes[i]);
        }
		
	fclose(file);	
	
	free(primes);

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */



/*
 * find_primes:
 *
 * 	This is the chuncked version of the sieves of Eratosthenes. 
 * 	Pass in the bounds for the current sieve. 
 * 	primes in the list of already found prime numbers
 * 	N is the length of the list primes.
 */
void 
find_primes(int from, int to, int* primes, int* N, int* integers, int t)
{
	// current range of numbers. and mid point
	int mem_size = (from - to); 
	int mid = floor((mem_size + 1) / 2);
	
	printf("Made it to func");
	
	// Mark all numbers in the current range that are multiples of the already
	// found prime numbers.
	int i, k; 
#	pragma omp parallel for num_threads(t) shared(integers, from, to, N) private(i, k)
	for (i = 0; i <= *N; i++) {
		for (k = from; k <= to; k++) {
			if (k % integers[i] == 0) {
				integers[k - from] = 0;
			}
		}
	}


	// We can now extend the sieves to the current range. We want to count number of new primes.
	int new_primes = 0;
# 	pragma omp parallel for num_threads(t) shared(mem_size, mid, integers) reduction(+: new_primes) schedule(dynamic)
	for (i = 0; i <= mid; i++) {
		if (integers[i]) {
			// the current prime is at an offset of i from the start of our current range.
			for (k = 2 * (from + i); k <= to; k += (from + i)) {
				integers[k - (2*from) - 2] = 0;
				new_primes++;
			}
		}
	}

	// We now want to extend the list of primes and incriment N.
	int cnt = 0;
	primes = realloc(primes, (*N + new_primes)*sizeof(int));
	for (i = 0; i <= mem_size; i++) {
		if (integers[i]) {
			// add the new primes to the prime counts.
			primes[*N + cnt] = from + i; 
		}
	}

	*N =+ new_primes;
	free(integers);
}

