#include<stdio.h>

typedef int fruit;

#define TRUE 1
#define FALSE 0

int Check(fruit iNo)
{
	if ((iNo % 5) == 0)
		return TRUE;
	else
		return FALSE;



}







int main()
{
	fruit iValue = 0;
	fruit bRet = FALSE;
	printf("Enter number\n");
	scanf("%d", &iValue);
	bRet = Check(iValue);
	if (bRet == TRUE)
	{
		printf("\nDivisible by 5\n");
	}
	else
	{
		printf("\nNot divisible by 5\n");
	}
	return 0;
}