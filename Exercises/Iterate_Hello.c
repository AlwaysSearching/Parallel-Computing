/*
 * =====================================================================================
 *
 *       Filename:  Iterate_Hello.c
 *
 *    Description:  Have N processors print hello to the screen sequentially. Iterate this cyclically T times.
 *
 *        Created:  3/23/2020  
 *
 *         Author:  Sam Fieldman
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main ( int argc, char *argv[] )
{
        MPI_Init(&argc, &argv);
        MPI_Comm mcw_0 = MPI_COMM_WORLD;

        int rank;
        int size;

        MPI_Comm_rank(mcw_0, &rank);
        MPI_Comm_size(mcw_0, &size);
	
	int data = 0;
	int iter = 0;
	
	while (iter < 3) {
		if (rank == 0) {
			if (iter != 0) {
				MPI_Recv(&data, 1, MPI_INT, size - 1, 0, mcw_0, MPI_STATUS_IGNORE);	
			}	
			printf("Hello from Rank %d. I recieved data from rank %d. Iteration %d.\n", rank, data, iter);
			MPI_Send(&rank, 1, MPI_INT, rank + 1, 0, mcw_0);
		}
		else if (rank < size -1) {
			MPI_Recv(&data, 1, MPI_INT, rank - 1, 0, mcw_0, MPI_STATUS_IGNORE);
			printf("Hello from Rank %d. I recieved data from rank %d. Iteration %d.\n", rank, data, iter);
                        MPI_Send(&rank, 1, MPI_INT, rank + 1 , 0, mcw_0);
		}
		else {
			MPI_Recv(&data, 1, MPI_INT, rank - 1, 0, mcw_0, MPI_STATUS_IGNORE);
                        printf("Hello from Rank %d. I recieved data from rank %d. Iteration %d.\n", rank, data, iter);
                        MPI_Send(&rank, 1, MPI_INT, 0, 0, mcw_0);
		}
		iter++;
	}

        MPI_Finalize();

        return 0;
}

