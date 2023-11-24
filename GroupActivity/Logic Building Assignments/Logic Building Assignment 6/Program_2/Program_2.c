#include<stdio.h>

int CountOdd(int iNo)
{
	// Logic
	int iDigit = 0;
	int count = 0;
	if (iNo < 0)
	{
		iNo = -iNo;
	}
	if (iNo == 0)
	{
		return(0);
	}

	while (iNo > 0)
	{
		iDigit = iNo % 10;
		if ((iDigit % 2) != 0)
		{
			count += 1;
		}
		iNo = iNo / 10;
	}
	return count;
	
}

int main()
{
	int iValue = 0;
	int iRet = 0;
	printf("\nEnter number :\t");
	scanf("%d", &iValue);
	iRet = CountOdd(iValue);
	printf("\nCount of odd digits in the given number is %d\n", iRet);
	return 0;
}