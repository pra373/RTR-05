#include<stdio.h>

#define NUM_ELEMENTS 10

int main(void)
{
	//Variable declarations

	int PLP_IARRAY[NUM_ELEMENTS];
	int PLP_I, PLP_NUM, PLP_SUM = 0;

	//code

	printf("\n\n");

	// ******* Input Array Elements **********

	printf("Enter Integer Elements for array : \n\n");

	for (PLP_I = 0; PLP_I < NUM_ELEMENTS; PLP_I++)
	{
		scanf("%d", &PLP_IARRAY[PLP_I]);
	}

	// *******Seperating Even Numbers From Array Elements ******

	printf("\n\n");
	printf("Even numbers amongst the array elements are : \n\n");

	for (PLP_I = 0; PLP_I < NUM_ELEMENTS; PLP_I++)
	{
		if (PLP_IARRAY[PLP_I] % 2 == 0)
		{
			printf("%d\n", PLP_IARRAY[PLP_I]);
		}

	}

	// *******Seperating odd Numbers From Array Elements ******

	printf("\n\n");
	printf("Odd numbers amongst the array elements are : \n\n");

	for (PLP_I = 0; PLP_I < NUM_ELEMENTS; PLP_I++)
	{
		if (PLP_IARRAY[PLP_I] % 2 != 0)
		{
			printf("%d\n", PLP_IARRAY[PLP_I]);
		}
	}


	return(0);

}