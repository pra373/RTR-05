#include<stdio.h>

#define NUM_ELEMENTS 10

int main(void)
{
	//variable declaration
	int PLP_IARRAY[NUM_ELEMENTS];
	int PLP_I, PLP_NUM, PLP_J, PLP_COUNT = 0;

	//code

	printf("\n\n");

	//********* Input Array Elements *********

	printf("Enter Integer Elements of array : \n\n");

	for (PLP_I = 0; PLP_I < NUM_ELEMENTS; PLP_I++)
	{
		scanf("%d", &PLP_NUM);

		if (PLP_NUM < 0)
		{
			PLP_NUM = -1 * PLP_NUM; // If PLP_NUM is negative convert it to a positive number.
		}

		PLP_IARRAY[PLP_I] = PLP_NUM;

		
	}

	// ********* Printing Entire Array *********

	printf("\n\n");
	printf("Array Elements are : \n\n");
	for (PLP_I = 0; PLP_I < NUM_ELEMENTS; PLP_I++)
	{
		printf("%d\n",PLP_IARRAY[PLP_I]);
	}

	// ********* finding Prime Numbers **********

	printf("\n\n");
	printf("Prime numbers amongst the array elements are : \n\n");

	for (PLP_I = 0; PLP_I < NUM_ELEMENTS; PLP_I++)
	{
		for (PLP_J = 1; PLP_J <= PLP_IARRAY[PLP_I]; PLP_J++)
		{
			if ((PLP_IARRAY[PLP_I] % PLP_J) == 0)
			{
				PLP_COUNT++;
			}

		}

		if (PLP_COUNT == 2)
		{
			printf("%d\n", PLP_IARRAY[PLP_I]);
		}

		PLP_COUNT = 0;  // reset value of count

		// Number 1 is neither a prime number nor a consonant
		// If a number is prime it is only divisible by 1 and itself;
		// Hence If a number is prime the value of count will be exactly 2
		//If value of count is greater than 2, The number is not a prime number 
	}

	
	
	return 0;



}