#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mpi.h"

/*
 * pingpong1.c -- pingpong program
 *
 * Send an array from 1 process with rank 1 to rank 0 and vice versa.
 *
 * Input: none.
 * Output: contents of messages of Rank 0.
 *
 * Modified by Clayton Glenn.
 */

int main (int argc, char* argv[]) { /* main */

  const int 	first_rank  = 0;       /* Ranks run 0..number_of_processes-1 */
  const int 	master_rank = 0;       /* Rank of master process             */
  const int 	tag         = 0;       /* Tag for messages                   */
  MPI_Status 	recv_status;           /* Return status for receive          */
  int        	number_of_processes;   /* Number of processes in this run    */
  int        	my_rank;               /* Rank of this process               */
  int        	source;                /* Rank of sender                     */
  int        	destination;           /* Rank of receiver                   */
  int        	mpi_error_code;        /* Error code returned by MPI call    */

	/* Declare Array of Size 2^n */
	const int 	array_size = pow(2.0,10);
  const int 	array_length = array_size*sizeof(char)+1;
  int     	array[array_size];

	/* Initiallize MPI Communication */
  mpi_error_code = MPI_Init(&argc, &argv);
  mpi_error_code = MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  mpi_error_code = MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);

	/* If process is not Master process */
  if (my_rank != master_rank) {

		//Grab master rank
	  destination = master_rank;
		source = master_rank;

		// Loop for 100 recieves and sends
		int count = 1;
		while(count <= 100) {

			// Recieve the array
      mpi_error_code = MPI_Recv(array, array_length, MPI_INT, source, tag, MPI_COMM_WORLD, &recv_status);

			// Send the array
      mpi_error_code = MPI_Send(array, array_length, MPI_INT, destination, tag, MPI_COMM_WORLD);

			// Increment Count for loop
			count++;
		}
	/* If process is Master process */
  } else {
	  double time1 = MPI_Wtime();
		// Grab rank 1
	  destination = 1;
		source = 1;

		// Populated the array
		int count = 0;
		while(count < array_size) {
			array[count] = count;
			count++;
		}

		// Loop 100 times for Sends and Recieves
		count = 1;
		while(count <= 100) {

			// Print loop count
	    fprintf(stdout, "Ping-Pong: %d!\n", count);

			// Send the array
			mpi_error_code = MPI_Send(array, array_length, MPI_INT, destination, tag, MPI_COMM_WORLD);

			// Recieve the array
      mpi_error_code = MPI_Recv(array, array_length, MPI_INT, source, tag, MPI_COMM_WORLD, &recv_status);

			// Increment Count for loop
			count++;
		}
	  double time2 = MPI_Wtime();
	  fprintf(stdout, "\nElapsed time of the program is %.10f seconds\n", (time2 - time1));
  }

	// Close MPI Connection
  mpi_error_code = MPI_Finalize();
}
