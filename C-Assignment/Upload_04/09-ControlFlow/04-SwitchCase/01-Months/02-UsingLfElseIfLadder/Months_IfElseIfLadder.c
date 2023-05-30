#include<stdio.h>

int main(void)
{
	// variable declarations
	int PLP_MONTH;

	//code

	printf("\n\n");

	printf("Enter Number of Month (1 to 12) : ");
	scanf("%d", &PLP_MONTH);

	printf("\n\n");

	// If-else if ladder begins from here..

	if (PLP_MONTH == 1)
	{
		printf("Month Number %d is January !!!\n\n", PLP_MONTH);

	}

	else if (PLP_MONTH == 2)
	{
		printf("Month Number %d is February !!!\n\n", PLP_MONTH);

	}

	else if (PLP_MONTH == 3)
	{
		printf("Month Number %d is March !!!\n\n", PLP_MONTH);

	}

	else if (PLP_MONTH == 4)
	{
		printf("Month Number %d is April !!!\n\n", PLP_MONTH);

	}

	else if (PLP_MONTH == 5)
	{
		printf("Month Number %d is May !!!\n\n", PLP_MONTH);

	}

	else if (PLP_MONTH == 6)
	{
		printf("Month Number %d is June !!!\n\n", PLP_MONTH);

	}

	else if (PLP_MONTH == 7)
	{
		printf("Month Number %d is July !!!\n\n", PLP_MONTH);

	}

	else if (PLP_MONTH == 8)
	{
		printf("Month Number %d is August !!!\n\n", PLP_MONTH);

	}

	else if (PLP_MONTH == 9)
	{
		printf("Month Number %d is September !!!\n\n", PLP_MONTH);

	}

	else if (PLP_MONTH == 10)
	{
		printf("Month Number %d is octomber !!!\n\n", PLP_MONTH);

	}
	else if (PLP_MONTH == 11)
	{
		printf("Month Number %d is November !!!\n\n", PLP_MONTH);

	}

	else if (PLP_MONTH == 12)
	{
		printf("Month Number %d is December !!!\n\n", PLP_MONTH);

	}

	else
	{
		printf("Invalid Month Number %d entered !!! Please Try Again...\n\n", PLP_MONTH);

	}
	printf("IF - ELSE-IF Ladder Complete !!!\n");

	return(0);


}


