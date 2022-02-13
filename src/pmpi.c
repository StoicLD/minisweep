#include "mpi.h"
#include "stdio.h"

int MPI_Init(int *argc, char ***argv)
{
    printf("use my mpi init\n");
    return PMPI_Init(argc, argv);
}

int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest,
                            int tag, MPI_Comm comm)
{
    return PMPI_Send(buf, count, datatype, dest, tag, comm);
}

int MPI_Isend(const void *buf, int count, MPI_Datatype datatype, int dest,
                             int tag, MPI_Comm comm, MPI_Request *request)
{
    return PMPI_Isend(buf, count, datatype, dest, tag, comm, request);
}

int MPI_Finalize(void)
{
    printf("use my mpi finialize\n");
    return PMPI_Finalize();
}