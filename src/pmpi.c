#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"

#define ROOT_RANK 0
int* send_vector = NULL;
int curr_rank;
int total_rank_size;

void print_mpi_event_data(int total_size, int *result_matrix);

int MPI_Init(int *argc, char ***argv)
{
    printf("use my mpi init\n");
    int result = PMPI_Init(argc, argv);
    if(send_vector != NULL)
    {
        free(send_vector);
    }

    PMPI_Comm_rank(MPI_COMM_WORLD, &curr_rank);
    PMPI_Comm_size(MPI_COMM_WORLD, &total_rank_size);

    send_vector = (int*) malloc(sizeof(int) * total_rank_size);
    memset(send_vector, 0, sizeof(int) * total_rank_size);
    return result;
}

int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest,
                            int tag, MPI_Comm comm)
{
    send_vector[dest]++;
    return PMPI_Send(buf, count, datatype, dest, tag, comm);
}

int MPI_Isend(const void *buf, int count, MPI_Datatype datatype, int dest,
                             int tag, MPI_Comm comm, MPI_Request *request)
{
    send_vector[dest]++;
    return PMPI_Isend(buf, count, datatype, dest, tag, comm, request);
}

int MPI_Finalize(void)
{
    printf("use my mpi finialize\n");
    MPI_Barrier(MPI_COMM_WORLD);
    int* result_matrix;
    if(curr_rank == ROOT_RANK)
    {
        result_matrix = (int*) malloc(total_rank_size * total_rank_size);
    }

    // gather data
    MPI_Gather(send_vector, total_rank_size, MPI_INT, result_matrix, total_rank_size, MPI_INT, ROOT_RANK, MPI_COMM_WORLD);

    // finalize
    int result = PMPI_Finalize();

    // print result
    if(curr_rank == ROOT_RANK)
    {
        print_mpi_event_data(total_rank_size, result_matrix);
        free(result_matrix);
    }

    // release memory
    free(send_vector);
    return result;
}

//prints out the send event matrix to a file
void print_mpi_event_data(int total_size, int *result_matrix)
{
    int i;

    char filename[1024];
    sprintf(filename, "matrix.data");

    FILE *fp = fopen(filename, "w");

    for(i = 0; i < total_size; i++)
    {
        fprintf(fp, "%d ", i);
        for(int j = 0; j < total_rank_size; j++)
        {
            if(j == 0)
            {
                fprintf(fp, "%d", result_matrix[i * total_rank_size + j]);
            }
            else
            {
                fprintf(fp, " %d", result_matrix[i * total_rank_size + j]);
            }
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}