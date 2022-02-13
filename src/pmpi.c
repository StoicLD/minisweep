#include "mpi.h"
#include "stdio.h"

OMPI_DECLSPEC int MPI_Init(int *argc, char ***argv)
{
    printf("use my mpi\n");
    PMPI_Init(argc, argv);
}