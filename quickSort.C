#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
 
void quickSort(int *mat, int left, int right);
void swap(int* a, int* b);

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    //int *mat = NULL;
    int n;
    int numranks, rank;
    double startTime, endTime, totalTime;
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    startTime = MPI_Wtime();

    if (rank == 0)
    {
        n = 500000;
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int *a;

    if(rank == 0)
    {
        a = (int*)malloc(n * sizeof(int));
        for (int i = 0; i < n; i++)
        {
            a[i] = rand() % 10;
            //printf("%d ", a[i]);
        }
    }
    int myN = n/numranks;
    int send = myN;

    int *myA = (int*)malloc(myN * sizeof(int));
    int *temp = (int*)malloc(n * n * sizeof(int));

    MPI_Scatter(a, send, MPI_INT, myA, send, MPI_INT, 0, MPI_COMM_WORLD);

    quickSort(myA, myN*rank, myN-1);

    MPI_Gather(myA, send, MPI_INT, temp, send, MPI_INT, 0, MPI_COMM_WORLD);

    printf("\n");
    if (rank == 0)
    {
        quickSort(temp, 0, n-1);
        // printf("Sorted Array: \n");
        // for (int i = 0; i < n; i++)
        // {
        //     printf("%d ", temp[i]);
        // }
        
    }

    printf("\n");

    MPI_Finalize();
    endTime = MPI_Wtime();

    printf("\nRank: %d,Time: %f\n", rank, endTime-startTime);
}

int partition(int *mat, int left, int right)
{
    int pivot = mat[right];
    int i = (left - 1);

    for (int j = left; j <= right - 1; j++)
    {
        if (mat[j] < pivot)
        {
            i++;
            swap(&mat[i], &mat[j]);
        }
    }
    swap(&mat[i+1], &mat[right]);
    return (i + 1);
}

void quickSort(int *mat, int left, int right)
{
    if (left < right)
    {
        int p = partition(mat, left, right);

        quickSort(mat, left, p - 1);
        quickSort(mat, p + 1, right);
    }
}

void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}
