#include<stdio.h>
#include<stdlib.h>

#define INT_SIZE sizeof(int)
#define FLOAT_SIZE sizeof(float)
#define DOUBLE_SIZE sizeof(double)
#define CHAR_SIZE sizeof(char)

int main(int argc, char *argv[], char *envp[])
{
	//variable declarations

	int *PLP_PTR_IARRAY = NULL;
	unsigned int iarray_length = 0;

	float *PLP_PTR_FARRAY = NULL;
	unsigned int farray_length = 0;

	double *PLP_PTR_DARRAY = NULL;
	unsigned int darray_length = 0;

	char *PLP_PTR_CARRAY = NULL;
	unsigned int carray_length = 0;

	int i;

	//code

	// ********* Integer array ***********
	printf("\n\n");
	printf("enter how many number of elements you want in integer array : \n\n");
	scanf("%u", &iarray_length);

	PLP_PTR_IARRAY = (int *)malloc(INT_SIZE * iarray_length);

	if (PLP_PTR_IARRAY == NULL)
	{
		printf("\n\n");
		printf("Memory allocation for integer array has failed !!! Exitting Now ...\n\n");
		exit(0);
	}

	else
	{
		printf("Memory allocation for integer array has Succeeded !!! \n\n");
		printf("Memory Address from %p to %p have been allocated to integer array !!!\n\n", PLP_PTR_IARRAY, (PLP_PTR_IARRAY + (iarray_length - 1)));

	}

	printf("\n\n");
	printf("Enter %d Integer Elements to fill up the Integer Array : \n\n", iarray_length);
	for (i = 0; i < iarray_length; i++)
	{
		scanf("%d", (PLP_PTR_IARRAY + i));

	}

	//float array

	printf("\n\n");
	printf("enter how many number of elements you want in float array : \n\n");
	scanf("%u", &farray_length);

	PLP_PTR_FARRAY = (float *)malloc(FLOAT_SIZE * farray_length);

	if (PLP_PTR_FARRAY  == NULL)
	{
		printf("\n\n");
		printf("Memory allocation for float array has failed !!! Exitting Now ...\n\n");
		exit(0);
	}

	else
	{
		printf("Memory allocation for float array has Succeeded !!! \n\n");
		printf("Memory Address from %p to %p have been allocated to integer array !!!\n\n", PLP_PTR_FARRAY, (PLP_PTR_FARRAY + (farray_length - 1)));

	}

	printf("\n\n");
	printf("Enter %d float Elements to fill up the Float Array : \n\n", farray_length);
	for (i = 0; i < farray_length; i++)
	{
		scanf("%f", (PLP_PTR_FARRAY + i));

	}

	// ********* Double array *********

	printf("\n\n");
	printf("enter how many number of elements you want in double array : \n\n");
	scanf("%u", &darray_length);

	PLP_PTR_DARRAY = (double *)malloc(DOUBLE_SIZE * darray_length);

	if (PLP_PTR_DARRAY == NULL)
	{
		printf("\n\n");
		printf("Memory allocation for double array has failed !!! Exitting Now ...\n\n");
		exit(0);
	}

	else
	{
		printf("Memory allocation for double array has Succeeded !!! \n\n");
		printf("Memory Address from %p to %p have been allocated to integer array !!!\n\n", PLP_PTR_DARRAY, (PLP_PTR_DARRAY + (darray_length - 1)));

	}

	printf("\n\n");
	printf("Enter %d double Elements to fill up the double Array : \n\n", darray_length);
	for (i = 0; i < darray_length; i++)
	{
		scanf("%lf", (PLP_PTR_DARRAY + i));

	}

	//********* Char array **********

	printf("\n\n");
	printf("enter how many number of elements you want in character array : \n\n");
	scanf("%u", &carray_length);

	PLP_PTR_CARRAY = (char *)malloc(CHAR_SIZE * carray_length);

	if (PLP_PTR_CARRAY == NULL)
	{
		printf("\n\n");
		printf("Memory allocation for character array has failed !!! Exitting Now ...\n\n");
		exit(0);
	}

	else
	{
		printf("Memory allocation for character array has Succeeded !!! \n\n");
		printf("Memory Address from %p to %p have been allocated to character array !!!\n\n", PLP_PTR_CARRAY, (PLP_PTR_CARRAY + (carray_length - 1)));

	}

	printf("\n\n");
	printf("Enter %d character Elements to fill up the character Array : \n\n", carray_length);

	for (i = 0; i < carray_length; i++)
	{
		*(PLP_PTR_CARRAY + i) = getch();
		
		printf("%c\n", *(PLP_PTR_CARRAY + i));

	}

	// Display of arrays

	// *********** Integer Array ***********

	printf("\n\n");

	printf("The integer array entered by you and consisting of %d Elements is as follows : \n\n", iarray_length);
	for (i = 0;i < iarray_length; i++)
	{
		printf("%d \t \t at address : %p\n", *(PLP_PTR_IARRAY + i), (PLP_PTR_IARRAY + i));

	}

	// ********** Float array ************

	printf("\n\n");

	printf("The float array entered by you and consisting of %d Elements is as follows : \n\n", farray_length);
	for (i = 0;i < farray_length; i++)
	{
		printf("%f \t \t at address : %p\n", *(PLP_PTR_FARRAY + i), (PLP_PTR_FARRAY + i));

	}

	// ********** Double array ************

	printf("\n\n");

	printf("The double array entered by you and consisting of %d Elements is as follows : \n\n", darray_length);
	for (i = 0;i < darray_length; i++)
	{
		printf("%lf \t \t at address : %p\n", *(PLP_PTR_DARRAY + i), (PLP_PTR_DARRAY + i));



	}

	// ********** Character array ************

	printf("\n\n");

	printf("The Character array entered by you and consisting of %d Elements is as follows : \n\n", carray_length);
	for (i = 0; i < carray_length; i++)
	{
		printf("%c \t \t at address : %p\n", *(PLP_PTR_CARRAY + i), (PLP_PTR_CARRAY + i));



	}

	// ************ Freeing memory occupied by arrays (in reverse order of allocation) ******

	if (PLP_PTR_CARRAY)
	{
		free(PLP_PTR_CARRAY);
		PLP_PTR_CARRAY = NULL;

		printf("\n\n");
		printf("Memory Allocated for Character array has been successfully freed !!! \n\n");
	}

	if (PLP_PTR_DARRAY)
	{
		free(PLP_PTR_DARRAY);
		PLP_PTR_DARRAY = NULL;

		printf("\n\n");
		printf("Memory Allocated for double array has been successfully freed !!! \n\n");
	}

	if (PLP_PTR_FARRAY)
	{
		free(PLP_PTR_FARRAY);
		PLP_PTR_FARRAY = NULL;

		printf("\n\n");
		printf("Memory Allocated for Float array has been successfully freed !!! \n\n");
	}

	if (PLP_PTR_IARRAY)
	{
		free(PLP_PTR_IARRAY);
		PLP_PTR_IARRAY = NULL;

		printf("\n\n");
		printf("Memory Allocated for Intrger array has been successfully freed !!! \n\n");
	}

	return(0);





	

	








}