#include<stdio.h>

int CountRange(int iNo)
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
		if (iDigit >= 3 && iDigit <= 7)
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
	iRet = CountRange(iValue);
	printf("\nCount of digits in between '3' and '7' in the given number is %d\n", iRet);
	return (0);
}