#include<stdio.h>
int Count(int iNo)
{
	// Logic
	int iDigit = 0;
	int count = 0;
	
	if (iNo == 0)
	{
		return(0);
	}
	else
	{
		while (iNo > 0)
		{
			iDigit = iNo % 10;
			if (iDigit < 6)
			{
				count += 1;
			}
			iNo = iNo / 10;
		}
		return count;
	}
}
int main()
{
	int iValue = 0;
	int iRet = 0;
	printf("\nEnter number : \t");
	scanf("%d", &iValue);
	iRet = Count(iValue);
	printf("\nCount of digits less than '6' in the given number is %d\n", iRet);
	return 0;
}