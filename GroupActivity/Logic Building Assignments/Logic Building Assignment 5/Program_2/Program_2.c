#include<stdio.h>
#define TRUE 1
#define FALSE 0
typedef int BOOL;

BOOL ChkZero(int iNo)
{
	// Logic
	int iDigit = 0;
	if (iNo == 0)
	{
		return TRUE;
	}
	else
	{
		while (iNo > 0)
		{
			iDigit = iNo % 10;
			if (iDigit == 0)
			{
				return TRUE;
			}
			iNo = iNo / 10;
		}
		return FALSE;
	}
}

int main()
{
	int iValue = 0;
	BOOL bRet = FALSE;

	printf("\nEnter number :\t");
	scanf("%d", &iValue);
	printf("\n");
	bRet = ChkZero(iValue);
	if (bRet == TRUE)
	{
		printf("It Contains Zero\n");
	}
	else
	{
		printf("There is no Zero\n");
	}
	return 0;
}