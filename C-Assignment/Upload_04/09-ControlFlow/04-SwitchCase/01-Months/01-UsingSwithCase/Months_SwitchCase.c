#include<stdio.h>

int main(void)
{
	// variable declarations
	int PLP_MONTH;

	//code

	printf("\n\n");

	printf("Enter Number of Month (1 to 12) : ");
	scanf("%d", &PLP_MONTH);

	printf("\n\n");

	switch (PLP_MONTH)
	{
	case 1:
		printf("Month Number %d is January !!!\n\n", PLP_MONTH);
		break;

	case 2:
		printf("Month Number %d is February !!!\n\n", PLP_MONTH);
		break;

	case 3:
		printf("Month Number %d is March !!!\n\n", PLP_MONTH);
		break;

	case 4:
		printf("Month Number %d is April !!!\n\n", PLP_MONTH);
		break;

	case 5:
		printf("Month Number %d is May !!!\n\n", PLP_MONTH);
		break;

	case 6:
		printf("Month Number %d is June !!!\n\n", PLP_MONTH);
		break;

	case 7:
		printf("Month Number %d is July !!!\n\n", PLP_MONTH);
		break;

	case 8:
		printf("Month Number %d is August !!!\n\n", PLP_MONTH);
		break;

	case 9:
		printf("Month Number %d is September !!!\n\n", PLP_MONTH);
		break;

	case 10:
		printf("Month Number %d is October !!!\n\n", PLP_MONTH);
		break;

	case 11:
		printf("Month Number %d is November !!!\n\n", PLP_MONTH);
		break;

	case 12:
		printf("Month Number %d is December !!!\n\n", PLP_MONTH);
		break;

	default:
		printf("Invalid Month number %d entered !!\n\n", PLP_MONTH);
		break;













	}

	printf("Switch case block complete !\n");

	return(0);
	
}