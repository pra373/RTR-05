#include<stdio.h>


void Pattern(int iRow, int iCol)
{
	// Logic
	int i = 0;
	int j = 0;
	for (i = 0; i < iRow; i++)
	{
		for (j = 0; j < iCol; j++)
		{
			printf("*  ");
		}
		printf("\n\n");
	}

	printf("\n\n");
}


int main()
{
	int iValue1 = 0;
	int iValue2 = 0;

	printf("\nEnter number of rows : \t");
	scanf("%d", &iValue1);
	printf("\nEnter number of columns:\t");
	scanf("%d", &iValue2);
	if (iValue1 <= 0 || iValue2 <= 0)
	{
		printf("Enter a positive integer");
	}
	printf("\n\n");
	Pattern(iValue1, iValue2);
	return 0;
}