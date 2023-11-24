#include<stdio.h>

void Pattern(int iNo)
{
	// Logic
	int i = 0;
	int j = 65;
	for (i = 1; i <= iNo; i++)
	{
		printf("%c ", j);
		j += 1;
	}
	printf("\n");
}
int main()
{
	int iValue = 0;
	printf("\nEnter number of elements( >0  and <=26 ):\t");
	scanf("%d", &iValue);
	printf("\n");
	if (iValue <= 0 || iValue > 26)
	{
		printf("Invalid input entered.\n");
	}

	else
	{
		Pattern(iValue);
	}
	
	return 0;
}