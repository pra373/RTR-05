#include<stdio.h>

void PrintEven(int iNo)
{
	int EvenNo = 0;

	if (iNo <= 0)
	{
		printf("Please enter a positive integer \n");
	}
	else
	{
		while (iNo > 0)
		{
			EvenNo = EvenNo + 2;
			printf("%3d \n", EvenNo);
			iNo--;
		}
	}
	
}

int main()
{
	int iValue = 0;
	printf("Enter number : \t");
	scanf("%d", &iValue);
	PrintEven(iValue);
	return(0);
}