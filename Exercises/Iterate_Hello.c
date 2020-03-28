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
	
	// Get rank in group and number of processes.
        MPI_Comm_rank(mcw_0, &rank);
        MPI_Comm_size(mcw_0, &size);
	
	int data = 0;
	int iter = 0;
	
	while (iter < 3) {
		if (rank == 0) {
			// Print status. Send message to following process. Wait on message from process size - 1.
			printf("Hello from Rank %d. I recieved data from rank %d. Iteration %d.\n", rank, data, iter);
			MPI_Send(&rank, 1, MPI_INT, rank + 1, 0, mcw_0);
			MPI_Recv(&data, 1, MPI_INT, size - 1, 0, mcw_0, MPI_STATUS_IGNORE);
		}
		else if (rank < size -1) {
			// Wait to recieve message from prior process. Print Status. Send to following process.
			MPI_Recv(&data, 1, MPI_INT, rank - 1, 0, mcw_0, MPI_STATUS_IGNORE);
			printf("Hello from Rank %d. I recieved data from rank %d. Iteration %d.\n", rank, data, iter);
                        MPI_Send(&rank, 1, MPI_INT, rank + 1 , 0, mcw_0);
		}
		else {
			// Wait for message. Print status. Send message to process 0.
			MPI_Recv(&data, 1, MPI_INT, rank - 1, 0, mcw_0, MPI_STATUS_IGNORE);
                        printf("Hello from Rank %d. I recieved data from rank %d. Iteration %d.\n", rank, data, iter);
                        MPI_Send(&rank, 1, MPI_INT, 0, 0, mcw_0);
		}
		iter++;
	}

        MPI_Finalize();

        return 0;
}

