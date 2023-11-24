#include<stdio.h>

void Display(int iNo)
{
	if (iNo < 10)
	{
		printf("Hello\n");
	}
	else
	{
		printf("Demo");
		printf("\n");
	}
}
int main()
{
	int iValue = 0;
	printf("\n");
	printf("Enter number:\t");
	scanf("%d", &iValue);
	printf("\n");
	Display(iValue);
	return 0;
}