#include<stdio.h>

void Display(int iNo)
{
	printf("\n");
	while (iNo > 0)
	{
		printf("*\t");
		iNo--;
	}
	printf("\n");
}

int main()
{
	int iValue = 0;
	printf("\n");
	printf("Enter Value:\t");
	scanf("%d", &iValue);
	Display(iValue);
	return 0;
}