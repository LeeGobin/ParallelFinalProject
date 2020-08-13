#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


int *countSort(int *a, int n);

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int numranks, rank;
    double startTime, endTime, totalTime;
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    startTime = MPI_Wtime();


    int n, i;
    int max;
    int *a, *b;

    if (rank == 0)
    {
        n = 10000;
        //printf("Size of Array: %d\n", n);
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if(rank == 0) 
    {
        a = (int*)malloc(n * sizeof(int));
        b = (int*)malloc(n * sizeof(int));
        for (int i = 0; i < n; i++)
        {
            a[i] = rand() % 10;
            //printf("%d ", a[i]);
        }
    }
    printf("\n");

    int myN = n/numranks;
    int send = myN;
    
    int *sortA = (int*)malloc(myN * sizeof(int));
    int *myA = (int*)malloc(myN * sizeof(int));
    int *temp = (int*)malloc(n * n * sizeof(int));

    MPI_Scatter(a, send, MPI_INT, myA, send, MPI_INT, 0, MPI_COMM_WORLD);
    //MPI_Scatter(a, send, MPI_INT, sortA, send, MPI_INT, 10, MPI_COMM_WORLD);

    sortA = countSort(myA, myN);

    MPI_Gather(sortA, send, MPI_INT, temp, send, MPI_INT, 0, MPI_COMM_WORLD);

    printf("\n");
    if (rank == 0)
    {
        b = countSort(temp, n);

        //printf("Sorted Array: \n");
        // for (int i = 0; i < n; i++)
        // {
        //     printf("%d ", b[i]);
        // }
        
    }
    printf("\n");

    MPI_Finalize();
    endTime = MPI_Wtime();

    printf("\nRank: %d,Time: %f\n", rank, endTime-startTime);

    
}

int *countSort(int *a, int n)
{
    static int finalA[500000] = {0};
    int count[500000] = {0};

    int i, j;
    int max = 0;

    for(i=0;i<n;++i)
    {
        if(a[i]>max)
        {
            max = a[i];
        }
    }
    for(i=0;i<n;++i)
    {
        count[a[i]]=count[a[i]]+1;
    }

    int q = 0;

    for(i=0;i<=max;++i)
    {
        for(j=1;j<=count[i];++j)
        {
            finalA[q] = i;
            q++;
        }
    }
    return finalA;
}
