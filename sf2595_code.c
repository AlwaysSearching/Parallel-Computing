#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/*
 * int main(int argc, char* argv[])
 *
 * program to ...
 *
 *
 *
 */
int main(int argc, char *argv[]) {
	// Initialize MPI, and get rank & number processes
	int rank, size;
	MPI_Init(NULL, NULL);
	MPI_Comm MCW = MPI_COMM_WORLD;

	MPI_Comm_rank(&rank, MCW);
	MPI_Comm_size(&size, MCW);

        unsigned int a, b, c, n;
        unsigned int i; //loop index
        FILE * fp; //for creating the output file
        char filename[100]=""; // the file name
        char * x; //the numbers in the range [2, N]

        clock_t start_p1, start_p3, end_p1,  end_p3;


        // start of part 1

        start_p1 = clock();


        // Check that the input from the user is correct.
        if(argc != 5){

                printf("usage:  ./checkdiv N a b c\n");
                printf("N: the upper bound of the range [2,N]\n");
                printf("a: first divisor\n");
                printf("b: second divisor\n");
                printf("c: third divisor\n");
                exit(1);
        }
	
	// process 0 should read in the args
	if (rank == 0) {
        	n = (unsigned int)atoi(argv[1]);
        	a = (unsigned int)atoi(argv[2]);
        	b = (unsigned int)atoi(argv[3]);
        	c = (unsigned int)atoi(argv[4]);

		// initialize the array x;
		memset(x, 0, n - 1);
	}

	// Process 0 must now send the a, b, c, and n to each process
	// since MIPI_Bcast is blocking, process 0 will time how long 
	// the broadcast to all processes will take.
	MPI_Bcast(&a, 1, MPI_INT, 0, MCW);
	MPI_Bcast(&b, 1, MPI_INT, 0, MCW);
	MPI_Bcast(&c, 1, MPI_INT, 0, MCW);
	MPI_Bcast(&n, 1, MPI_INT, 0, MCW);
	
	end_p1 = clock();
	// end part 1

	// start part 2. Note that the time for this section is the min start time and max end time.
	// Other processes must, after receiving the variables, calculate their own range.
	
	double start_p2 = MPI_Wtime();

	// calculate the range for each process.
	int quotient, remainder, my_first; my_last, my_count;
	quotient =  (n - 1) / size;
	remainder = (n -1) % size;
	
	if (rank < remainder){
		my_count = quotient + 1;
		my_first = rank * my_count;
	}
	else {
		my_count = quotient;
		my_first = (my_rank * my_count) + remainder;
	}

	my_last = my_first + my_count;
	
	// each array calculates its own list
	if (my_rank == 0) {
		x = malloc((n-1)* sizeof(char));
		
		// compute divisable elements.
		for (int i = my_first; i < my_last; i++) {
			if ( (i+2) % a == 0 || (i+2) % b == 0 || (i+2) % c == 0) {
				x[i] = 1;
			}
			else {
				x[i] = 0;
			}
		}	

		MPI_Gather(&x, my_count, MPI_CHAR, &x, my_count, MPI_CHAR, 0, MCW);
	}
	else {	
		x = malloc(my_count * sizeof(char));

                // compute divisable elements.
		for (int i = my_first; i < my_last; i++) {
                	if ( (i+2) % a == 0 || (i+2) % b == 0 || (i+2) % c == 0) {
                        	x[i] = 1;
                        }
                        else {
                                x[i] = 0;
                        }
                }

		// send to process 0
		MPI_Gather(&x, my_count, MPI_CHAR, &x, my_count, MPI_CHAR, 0, MCW);
	}

	// end of part 2
	double end_p2 = MPI_Wtime();

	// reduce using MPI_Reduce - MPI_MAX & MPI_MIN

        //start of part 3
        //forming the filename, we only need process 0 to do work here.
	
	
	if (rank == 0) {
        	// start timer
		start_p3 = clock();
		
        	strcpy(filename, argv[1]);
        	strcpy(filename, ".txt");

        	if( !(fp = fopen(filename,"w+t")))
        	{
                	printf("Cannot create file %s\n", filename);
                	exit(1);
        	}	

        	for(i = 2; i <= n; i++)
                	if(x[i]){
                        	fprintf(fp, "%d ", i);
                	}
        	}	

        	end_p3 = clock();
        	//end of part 3. Close timer.

        	/* Print the times of the three parts*/
		if (my_rank == 0) {
			printf("Time 1 = \t Time 2 = \t Time 3 = \n", time_1, time_2, time_3);
		}
	}

	// finalize and return
	MPI_Finalize();
        return 0;
}

