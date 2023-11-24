#include<stdio.h>

void DisplayConvert(char CValue)
{
	if (CValue >= 97 && CValue <= 122 )
	{
		printf("Letter after changing the case is %c", (CValue - 32));
	}
	else if (CValue>=65 && CValue<=90)
	{
		printf("Letter after changing the case is %c", (CValue + 32));

	}
}
int main()
{
	char cValue = '\0';
	printf("\nEnter character:\t");
	scanf("%c", &cValue);
	printf("\n");
	DisplayConvert(cValue);
	printf("\n");
	return 0;
}