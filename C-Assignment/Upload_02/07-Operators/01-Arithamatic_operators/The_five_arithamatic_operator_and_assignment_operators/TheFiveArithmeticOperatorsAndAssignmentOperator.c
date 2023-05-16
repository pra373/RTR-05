#include<stdio.h>
#include<conio.h>


int main(void)
{
	// variable declarations
	int PLP_A;
	int PLP_B;
	int PLP_RESULT;

	//code

	printf("\n\n");
	printf("Enter a Number: ");
	scanf("%d",&PLP_A);

	printf("\n\n");
	printf("Enter another number : ");
	scanf("%d",&PLP_B);

	printf("\n\n");

	PLP_RESULT = PLP_A + PLP_B;
	printf("Addition of PLP_A = %d and PLP_B = %d gives %d.\n", PLP_A, PLP_B, PLP_RESULT);
	
	PLP_RESULT = PLP_A - PLP_B;
	printf("Substraction of PLP_A = %d and PLP_B = %d gives %d.\n", PLP_A, PLP_B, PLP_RESULT);
	
	PLP_RESULT = PLP_A * PLP_B;
	printf("Multiplication of of PLP_A = %d and PLP_B = %d gives %d.\n", PLP_A, PLP_B, PLP_RESULT);

	PLP_RESULT = PLP_A / PLP_B;
	printf("Division of PLP_A = %d and PLP_B = %d gives quotient %d.\n", PLP_A, PLP_B, PLP_RESULT);

	PLP_RESULT = PLP_A % PLP_B;
	printf("Division of PLP_A = %d and PLP_B = %d gives remainder %d.\n", PLP_A, PLP_B, PLP_RESULT);

	printf("\n\n");
	getch();

	return(0);

}
