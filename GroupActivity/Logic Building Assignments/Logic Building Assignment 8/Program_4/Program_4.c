#include<stdio.h>

int RangeSumEven(int iStart, int iEnd)
{
	// Logic

	int i = 0;
	int sum = 0;
	if (iStart >= iEnd)
	{
		return(-1);
	}

	else
	{
		for (i = iStart; i <= iEnd; i++)
		{
			if (i % 2 == 0)
			{
				sum = sum + i;
			}

		}

		return(sum);
	}
}
int main()
{
	int iValue1 = 0, iValue2 = 0, iRet = 0;

	printf("\nEnter Starting point:\t");
	scanf("%d", &iValue1);
	printf("\nEnter ending point:\t");
	scanf("%d", &iValue2);
	printf("\n\n");
	iRet = RangeSumEven(iValue1, iValue2);
	if (iRet == -1)
	{
		printf("Invalid Range\n");
	}
	else
	{
		printf("Addition of all even numbers in given range = %d\n\n", iRet);

	}
	
	return 0;
}