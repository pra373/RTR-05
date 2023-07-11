#include<stdio.h>
#include<stdlib.h>

int main(void)
{
	//function declaration
	void MultiplyArrayElementsByNumber(int *, int, int);

	// variable declaration

	int *PLP_IARRAY = NULL;
	int num_elements;
	int i, num;

	//code

	printf("\n\n");
	printf("Enter how many elements you want in the integer array : ");
	scanf("%d", &num_elements);

	PLP_IARRAY = (int *)malloc(num_elements * sizeof(int));
	if (PLP_IARRAY == NULL)
	{
		printf("Memory allocation to PLP_IARRAY has failed !!! Exiting now ...\n\n");
		exit(0);
	}

	printf("\n\n");
	printf("Enter %d elements of the integer array : \n\n",num_elements);
	for (i = 0; i < num_elements; i++)
	{
		scanf("%d", &PLP_IARRAY[i]);
	}

	//one
	printf("\n\n");
	printf("Array before passing to function MultiplyArrayElementsByNumber() : \n\n");
	for (i = 0; i < num_elements; i++)
	{
		printf("PLP_IARRAY[%d] = %d\n", i, PLP_IARRAY[i]);

	}

	printf("\n\n");
	printf("Enter the  number by which you want to multiply each array element : ");
	scanf("%d", &num);

	 MultiplyArrayElementsByNumber(PLP_IARRAY,num_elements, num);

	 printf("\n\n");
	 printf("Array returned by function MultiplyArrayElementsByNumber() : \n\n ");

	 for (i = 0; i < num_elements; i++)
	 {
		 printf("PLP_IARRAY[%d] = %d\n", i, PLP_IARRAY[i]);
	 }

	 if (PLP_IARRAY)
	 {
		 free(PLP_IARRAY);
		 PLP_IARRAY = NULL;
		 printf("\n\n");
		 printf("Memory allocated to 'PLP_IARRAY' has been successfully freed !!! \n\n");
	 }

	 return(0);

}

void  MultiplyArrayElementsByNumber(int *arr, int inumelements, int n)
{
	int i;

	for (i = 0; i < inumelements; i++)
	{
		arr[i] = arr[i] * n;

	}
}