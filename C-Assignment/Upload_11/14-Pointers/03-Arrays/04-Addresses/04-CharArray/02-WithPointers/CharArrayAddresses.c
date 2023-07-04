#include<stdio.h>

int main(void)
{
	//variable declaration
	char PLP_CARRAY[10];
	int i;
	char *ptr_carray = NULL;

	//code

	for (i = 0; i < 10; i++)
	{
		PLP_CARRAY[i] = (char)(i + 65);
	}

	// name of an array is its own base address
	// assigning base address of double array 'PLP_CARRAY' to double pointer 'ptr_carray'

	ptr_carray = PLP_CARRAY;

	printf("\n\n");
	printf("Elements of the character array : \n\n");
	for (i = 0; i < 10; i++)
	{
		printf("PLP_CARRAY[%d] = %c\n", i, *(ptr_carray + i));

	}

	printf("\n\n");

	for (i = 0; i < 10; i++)
	{
		printf("PLP_CARRAY[%d] = %c \t \t Address = %p\n", i, *(ptr_carray + i), ptr_carray + i);
	}

	printf("\n\n");

	return(0);
}



