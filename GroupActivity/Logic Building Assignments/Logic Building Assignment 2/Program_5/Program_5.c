#include<stdio.h>
#define TRUE 1
#define FALSE 0
#define ERROR -1
typedef int BOOL;

BOOL ChkEven(int iNo)
{
	if (iNo == 0)
	{
		return(ERROR);
	}
	else if (iNo % 2 == 0)
	{
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
}
int main()
{
	int iValue = 0;
	BOOL bRet = FALSE;
	printf("\n");
	printf("Enter number:\t");
	scanf("%d", &iValue);
	printf("\n");
	bRet = ChkEven(iValue);
	if (bRet == ERROR)
	{
		printf("Given number is neither even nor odd\n");
	}

	else if (bRet == TRUE)
	{
		printf("Given number is an Even number\n");
	}

	else
	{
		printf("Given number is an Odd number\n");
	}
	return 0;
}