#include<stdio.h>

enum
{
	NEGATIVE = -1,
	ZERO,
	POSITIVE
};

int main(void)
{
	//function declaration
	int difference(int, int, int *);

	//variable declaration
	int PLP_A;
	int PLP_B;
	int PLP_ANSWER, PLP_RET;

	//code

	
	printf("\n\n");
	printf("Enter Value For 'PLP_A' : ");
	scanf("%d", &PLP_A);

	printf("\n\n");
	printf("Enter Value For 'PLP_B' : ");
	scanf("%d", &PLP_B);

	PLP_RET = difference(PLP_A, PLP_B, &PLP_ANSWER);
	printf("\n\n");
	printf("Difference of %d and %d = %d\n\n ", PLP_A, PLP_B, PLP_ANSWER);

	if (PLP_RET == POSITIVE)
	{
		printf("The difference of %d and %d is positive !!!\n\n ", PLP_A, PLP_B);

	}

	else if (PLP_RET == NEGATIVE)
	{
		printf("The difference of %d and %d is negative !!!\n\n ", PLP_A, PLP_B);

	}

	else
	{
		printf("The difference of %d and %d is zero !!!\n\n ", PLP_A, PLP_B);
	}

	return(0);

	
}

// Every function has only 1 valid retuen type
// but we want to return 2 things from the function 1) actual substraction 2) if that substraction is positive,negative or zero
//to return both things we can use out parameter
//out parameter will actually contain the substraction and return value will return the status of substraction

int difference(int x, int y, int *diff)
{
	//code

	*diff = x - y;
	if (*diff > 0)
	{
		return(POSITIVE);
	}

	else if (*diff < 0)
	{
		return(NEGATIVE);
	}

	else
		return(ZERO);
}