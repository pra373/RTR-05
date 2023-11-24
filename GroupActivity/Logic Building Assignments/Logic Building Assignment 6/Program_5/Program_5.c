#include<stdio.h>

int CountDiff(int iNo)
{
	// Logic
	int iDigit = 0;
	int SumEven = 0;
	int SumOdd = 0;

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
		if ((iDigit % 2) == 0)
		{
			SumEven += iDigit;
		}
		if ((iDigit % 2) != 0)
		{
			SumOdd += iDigit;
		}
		iNo = iNo / 10;
	}

	return (SumEven - SumOdd);
}

int main()
{
	int iValue = 0;
	int iRet = 0;
	printf("\nEnter number :\t");
	scanf("%d", &iValue);
	iRet = CountDiff(iValue);
	printf("\n(Sum of even digits) - (Sum of odd digits) = %d\n", iRet);
	return 0;
}