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



#include	<stdlib.h>

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
	
	// alloc array of the first N numbers
	int* integers = calloc(N, sizeof(int));

	// The sieves can be implimented with nested loops. Outer loop iterates from 
	// 2 to mid, inner loop goes from 2 to N at increments on k.
	for (int i = 0; i <= mid; i++) {
		if (integers[i] == 0) {
			for (int k = 0; k <= N; k +=i) {
				integers[k] = 1;
			}
		}
	}

	// open file and write primes.
	char filename[100] = "";
	strcat(filename, argv[1]);
	strcat(filename, ".txt")
	FILE* file; 
	
	if( !(file = fopen(filename,"w"))){
                printf("Cannot create file %s\n", filename);
                exit(1);
        }	
		
	for(i = 0; i <= n-2; i++){
		if(x[i]){				
                       	fprintf(fp, "%d ", i + 2);
                }
        }
		
	fclose(fp);	

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

