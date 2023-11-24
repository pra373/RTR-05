#include<stdio.h>

void Accept(int iNo)
{
	int iCnt = 0;
	printf("\n");
	for (iCnt=0; iCnt < iNo; iCnt++)
	{
		printf("*\t");
	}

	printf("\n");
	
}
int main()
{
	int iValue = 0;
	printf("\n");
	printf("Enter a number:\t");
	scanf("%d", &iValue);
	Accept(iValue);
	return 0;
}