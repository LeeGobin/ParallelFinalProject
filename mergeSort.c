#include <stdio.h> 
#include <stdlib.h>
#include <mpi.h>

//Function Declaration
void merge(int *nums, int left, int m, int right);
void mergeSort(int *nums, int left, int right);

//Main Function
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

    mergeSort(myA, myN*rank, myN-1);

    MPI_Gather(myA, send, MPI_INT, temp, send, MPI_INT, 0, MPI_COMM_WORLD);

    //printf("\n");
    if (rank == 0)
    {
        mergeSort(temp, 0, n-1);
        //printf("Sorted Array: \n");
        for (int i = 0; i < n; i++)
        {
            //printf("%d ", temp[i]);
        }
        
    }

    //printf("\n");

    MPI_Finalize();
    endTime = MPI_Wtime();

    printf("\nRank: %d,Time: %f\n", rank, endTime-startTime);
}

void merge(int *nums, int left, int m, int right)
{
    int i;
    int j;
    int k;
    int n1 = m - left + 1;
    int n2 = right-m;

    int tempL[n1];
    int tempR[n2];

    //Populate temp arrays 
    for(int i=0; i<n1;i++)
    {
        tempL[i]= nums[left+i];
    }

    for(int j=0;j<n2;j++)
    {
        tempR[j] = nums[m+1+j];
    }


    //rejoin temp arrays back into main array
    i=0;
    j=0;
    k=left;

    while(i<n1 && j<n2)
    {
        if(tempL[i] <= tempR[j])
        {
            nums[k]=tempL[i];
            i++;
        }
        else
        {
            nums[k]=tempR[j];
            j++;
        }
        k++;
    }

    //Copy remaining elements of tempL[]
    while(i<n1)
    {
        nums[k]=tempL[i];
        i++;
        k++;
    }

    //Copy remaining elements of tempR[]
    while(j<n2)
    {
        nums[k]=tempR[j];
        j++;
        k++;
    }
}

void mergeSort(int *nums, int left, int right)
{
    if(left<right)
    {
        int m = left+(right-left)/2;

        //Sort the halves
        mergeSort(nums, left, m);
        mergeSort(nums,m+1,right);

        //Merge
        merge(nums, left, m, right);
    }
}
