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

        MPI_Comm_rank(mcw_0, &rank);
        MPI_Comm_size(mcw_0, &size);

	// the value of N will be a random value between N and the value passed in argv
	int N;

	if (rank == 0) {
		int limit = atoi(argv[1]);
		N = rand() % limit;
	}
	
	// process 0 broadcasts to the rest of the processes.
	MPI_Bcast(&N, 1, MPI_INT, 0, mpi_0);

	// each process calculate its unique value.
	N = N - rank;

	// Use Reduce to find the sum, min, and max
	int min, max. sum;

	MPI_Reduce(&N, &sum, 1, MPI_INT, MPI_SUM, mpi_0);
	MPI_Reduce(&N, &max, 1, MPI_INT, MPI_MAX, mpi_0);
	MPI_Reduce(&N, &min, 1, MPI_INT, MPI_MIN, mpi_0);

	// process 0 prints the results.
	if (rank == 0) {
		printf("N = %d \n", N);
		printf("The sum is: %d \n", sum);
		printf("The min is: %d \n", min);
		printf("The max is: %d \n", max);
	}

        MPI_Finalize()
        return 0;
}

