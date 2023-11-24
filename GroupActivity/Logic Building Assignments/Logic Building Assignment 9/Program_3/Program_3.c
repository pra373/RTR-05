#include<stdio.h>

void Pattern(int iNo)
{
	// Logic

	int i = 0;
	for (i = 1; i <=iNo ; i++)
	{
		printf("%d  *  ", i);

	}
	printf("\n\n");
}
int main()
{
	int iValue = 0;
	printf("\n\nEnter number of elements:\t");
	scanf("%d", &iValue);
	printf("\n\n");
	if (iValue <= 0)
	{
		printf("Invalid input.\n");
	}


	else
	{
		Pattern(iValue);
	}

	return 0;
}