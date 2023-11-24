#include<stdio.h>
int CountTwo(int iNo)
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
			if (iDigit == 2)
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
	int bRet = 0;
	printf("\nEnter number :\t");
	scanf("%d", &iValue);
	bRet = CountTwo(iValue);
	printf("\nCount of number '2' in the given number is %d\n", bRet);
	return (0);
}
