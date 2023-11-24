#include<stdio.h>

int MultDigits(int iNo)
{
	// Logic
	int iDigit = 0;
	int multiplication = 1;
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
		if (iDigit != 0)
		{
			multiplication *= iDigit;
		}
		iNo = iNo / 10;
	}
	return multiplication;
}

int main()
{
	int iValue = 0;
	int iRet = 0;
	printf("\nEnter number :\t");
	scanf("%d", &iValue);
	iRet = MultDigits(iValue);
	printf("\nMultiplication of all digits (ignoring zero) is %d\n", iRet);
	return 0;
}