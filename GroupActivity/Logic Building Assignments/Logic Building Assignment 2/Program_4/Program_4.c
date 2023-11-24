#include<stdio.h>

void Display(int iNo, int iFrequency)
{
	int i = 0;
	for (i=0; i< iFrequency; i++)
	{
		printf("%d\t", iNo);
	}
	printf("\n");
	
}
int main()
{
	int iValue = 0;
	int iCount = 0;
	printf("\n");
	printf("Enter number: \t");
	
	scanf("%d", &iValue);
	printf("\n");
	printf("Enter frequency: \t");
	
	scanf("%d", &iCount);
	printf("\n");
	Display(iValue, iCount);
	return 0;
}