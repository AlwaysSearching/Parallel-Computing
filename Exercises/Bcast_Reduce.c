/*
 * =====================================================================================
 *
 *       Filename:  Bcast_Reduce.c
 *
 *    Description:  
 *
 *        Created:  3/28/2020  
 *
 *         Author: Sam Fieldman 
 *	
 *  Purpuse/usage: The program takes in two arguments from the command line/ The first
 *  		   is an integer value that represents the maximum possible value the
 *  		   random number, N, can take. The second is the seed to pass to 
 *  		   srand so that different results can be orbtained. 
 *  		   (rand is always seeded to one.) 
 *		
 * =====================================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>


int main ( int argc, char *argv[] )
{
        MPI_Init(&argc, &argv);
        MPI_Comm mpi_0 = MPI_COMM_WORLD;

        int rank;
        int size;

        MPI_Comm_rank(mpi_0, &rank);
        MPI_Comm_size(mpi_0, &size);

	// simple check to ensure that we have 2 args.
	if (argc != 3) {
		if (rank == 0) {
			printf("Usage: Bcast_Reduce <N> <seed> \n");
			printf("N: Random int in range (0, N) \n");
			printf("seed: Seed for drawing random integers. \n");
		}
		MPI_Finalize();
		return 0;
	}

	// the value of N will be a random value between N and the value passed in argv
	int N;

	if (rank == 0) {
		int limit = atoi(argv[1]);
		int seed = atoi(argv[2]);
		srand(seed);
		N = rand() % limit;
	}
	
	// process 0 broadcasts to the rest of the processes.
	MPI_Bcast(&N, 1, MPI_INT, 0, mpi_0);

	// each process calculate its unique value.
	N = N - rank;

	// Use Reduce to find the sum, min, and max
	int min, max, sum;

	MPI_Reduce(&N, &sum, 1, MPI_INT, MPI_SUM, 0, mpi_0);
	MPI_Reduce(&N, &max, 1, MPI_INT, MPI_MAX, 0, mpi_0);
	MPI_Reduce(&N, &min, 1, MPI_INT, MPI_MIN, 0, mpi_0);

	// process 0 prints the results.
	if (rank == 0) {
		printf("N = %d \n", N);
		printf("The sum is: %d \n", sum);
		printf("The min is: %d \n", min);
		printf("The max is: %d \n", max);
	}

        MPI_Finalize();
        return 0;
}

