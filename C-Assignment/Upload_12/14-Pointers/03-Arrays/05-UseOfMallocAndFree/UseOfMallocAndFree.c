#include<stdio.h>
#include<stdlib.h>

int main(void)
{
	//variable declaration
	int *PLP_PTR_IARRAY = NULL;

	//it is a good practice to initialize any pointer with NULL value.It prevents any garbage value getting into it

	unsigned int PLP_INT_ARRAY_LENGTH = 0;
	int i;

	//code

	printf("\n\n");
	printf("Enter the number of elements you want in your Integer array : ");
	scanf("%d", &PLP_INT_ARRAY_LENGTH);

	//allocating memory in real time to a integer array.
	//user will choose the size of an array.
	//To allocate memory in realtime malloc() is used.
	//malloc() will return the base address of allocated memory by it
	//malloc() will return void (generic pointer)

	PLP_PTR_IARRAY = (int *)malloc(sizeof(int) * PLP_INT_ARRAY_LENGTH);
	if (PLP_PTR_IARRAY == NULL)
	{
		printf("Memory allocation for integer array has failed !!! Exitting Now ...\n\n");
		exit(0);
	}

	else  //If PLP_PTR_IARRAY is not null 
	{
		printf("Memory allocation for integer array has Succeeded !!! \n\n");
		printf("Memory Address from %p to %p have been allocated to integer array !!!\n\n", PLP_PTR_IARRAY, (PLP_PTR_IARRAY + (PLP_INT_ARRAY_LENGTH - 1)));

	}

	printf("\n\n");
	printf("Enter %d elements for the integer array : \n\n", PLP_INT_ARRAY_LENGTH);
	for (i = 0; i < PLP_INT_ARRAY_LENGTH; i++)
	{
		scanf("%d", (PLP_PTR_IARRAY + i));
	}

	printf("\n\n");
	printf("The Integer array entered by you, Consisting of %d Elements : \n\n", PLP_INT_ARRAY_LENGTH);

	for (i = 0; i < PLP_INT_ARRAY_LENGTH; i++)
	{
		printf("*(PLP_PTR_IARRAY + %d) = %d \t \t at address (PLP_PTR_IARRAY + %d) : %p \n", i, *(PLP_PTR_IARRAY + i), i, (PLP_PTR_IARRAY + i));

		
		


	}

	// Now as we allocated memory using malloc()
	// we must free the allocated memory by using free()
	// after free() we must initialize the pointer variable to NULL as agood practice


	if (PLP_PTR_IARRAY)
	{
		free(PLP_PTR_IARRAY);
		PLP_PTR_IARRAY = NULL;

		printf("\n\n");
		printf("Memory Allocated for integer array has been successfully freed !!! \n\n");
	}

	return(0);




}