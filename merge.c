#include <stdio.h> 
#include <stdlib.h> 

//Function Declaration
void merge(int nums[], int left, int m, int right);
void mergeSort(int nums[], int left, int right);
void printArray(int nums[], int size);

//Main Function
int main(int argc, char **argv)
{
    //Variables
    int numbers[]={34,65,74,1,67,26};
    int numbers_size=sizeof(numbers)/sizeof(numbers[0]);

    //Print Starting Array
   /* printf("Original Array: \n");
    for(int i=0;i<numbers_size;i++)
    {
        printf("%d, ", numbers[i]);
    }
    */
    printf("Original Array: \n");
    printArray(numbers, numbers_size);

    mergeSort(numbers, 0, numbers_size-1);

    printf("Sorted Array: \n");
    printArray(numbers, numbers_size);
    
}

void merge(int nums[], int left, int m, int right)
{
    int i;
    int j;
    int k;
    int n1 = m - left +1;
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

void mergeSort(int nums[], int left, int right)
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

void printArray(int nums[], int size)
{
    for(int i=0;i<size;i++)
    {
        printf("%d ", nums[i]);
        printf("\n");
    }
}