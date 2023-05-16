#include<stdio.h>
#include<conio.h>


int main(void)
{
	// variable declarations
	int PLP_A;
	int PLP_B;
	int PLP_TEMP;

	//code

	printf("\n\n");
	printf("Enter a Number: ");
	scanf("%d", &PLP_A);

	printf("\n\n");
	printf("Enter another number : ");
	scanf("%d", &PLP_B);

	printf("\n\n");

	PLP_TEMP = PLP_A;
	PLP_A += PLP_B;

	printf("Addition of PLP_A = %d and PLP_B = %d gives %d\n\n", PLP_TEMP, PLP_B, PLP_A);

	PLP_TEMP = PLP_A;
	PLP_A -= PLP_B;
	printf("Substration of PLP_A = %d and PLP_B = %d gives %d\n\n", PLP_TEMP, PLP_B, PLP_A);

	PLP_TEMP = PLP_A;
	PLP_A *= PLP_B;
	printf("Multiplication of PLP_A = %d and PLP_B = %d gives %d\n\n", PLP_TEMP, PLP_B, PLP_A);

	PLP_TEMP = PLP_A;
	PLP_A /= PLP_B;
	printf("Division of PLP_A = %d and PLP_B = %d gives quotient %d\n\n", PLP_TEMP, PLP_B, PLP_A);

	PLP_TEMP = PLP_A;
	PLP_A %= PLP_B;
	printf("Division of PLP_A = %d and PLP_B = %d gives remainder %d\n\n", PLP_TEMP, PLP_B, PLP_A);


	printf("\n\n");

	getch();

	return(0);
}
		




