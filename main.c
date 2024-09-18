#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	MPI_Init(&argc, &argv);
	int rank;
	int nProcs;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);
	printf("Rank: %d\n",rank);
	printf("Processes: %d\n",nProcs);
	MPI_Finalize();
}