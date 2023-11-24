#include<stdio.h>

int SumNonFact(int iNo)
{
	int sum = 0;
	int i = 0;
	if (iNo == 0)
	{
		return -1;

	}

	if (iNo == 1)
	{
		return (0);
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
			sum = sum + i;
		}
	}

	return(sum);
}


int main()
{
	int iValue = 0;
	int iRet = 0;
	printf("\nEnter number:\t");
	scanf("%d", &iValue);
	iRet = SumNonFact(iValue);
	printf("The sum of non factors is %d\n", iRet);
	return 0;
}