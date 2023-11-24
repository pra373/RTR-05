#include<stdio.h>
void DisplayDigit(int iNo)
{
	int iDigit = 0;
	if (iNo < 0)
	{

		iNo = -iNo;
	}
	else if(iNo == 0)
	{
		printf("0\n");
	}
	else
	{
		while (iNo > 0)
		{
			iDigit = iNo % 10;
			printf("%d\n", iDigit);
			iNo = iNo / 10;
		}
	}
}

int main()
{
	int iValue = 0;
	printf("\nEnter number :\t");
	scanf("%d", &iValue);
	printf("\n");
	printf("Printing Digits In Reverse Order\n\n");
	DisplayDigit(iValue);
	return 0;
}
