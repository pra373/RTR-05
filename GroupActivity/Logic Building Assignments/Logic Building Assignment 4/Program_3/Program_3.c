#include<stdio.h>


void NonFact(int iNo)
{
	int i = 0;
	if (iNo == 0)
	{
		printf("0 has no non factors\n");

	}

	if (iNo == 1)
	{
		printf("all numbers except 0 are non factors of 1\n");
	}

	if (iNo < 0)
	{
		iNo = -iNo;
	}

	printf("\n\nAs there are infinite number of non factors of every number, only considering till the number chosen by you.\n\n");

	for (i = 1; i < iNo; i++)
	{
		if (iNo % i != 0)
		{
			printf("%d\n", i);
		}
	}
}

int main()
{
	int iValue = 0;
	printf("\nEnter Number:\t");
	scanf("%d", &iValue);
	NonFact(iValue);
	printf("\n");
	return 0;
}