#include<stdio.h>
#include<conio.h>

int main(void)
{
	//variable declaration
	int PLP_A;
	int PLP_B;
	int PLP_RESULT;

	//code
	printf("\n\n");
	printf("Enter one integer : ");
	scanf("%d", &PLP_A);

	printf("\n\n");
	printf("Enter another integer : ");
	scanf("%d", &PLP_B);

	printf("\n\n");
	printf("If answer = 0,It is 'FALSE'. \n");
	printf("If answer = 1,It is 'TRUE'. \n\n");

	PLP_RESULT = (PLP_A < PLP_B);
	printf(" (PLP_A < PLP_B) PLP_A = %d Is Less Than PLP_B = %d         \t Answer = %d\n", PLP_A, PLP_B, PLP_RESULT);

	PLP_RESULT = (PLP_A > PLP_B);
	printf(" (PLP_A > PLP_B) PLP_A = %d Is greater Than PLP_B = %d      \t Answer = %d\n", PLP_A, PLP_B, PLP_RESULT);

	PLP_RESULT = (PLP_A <= PLP_B);
	printf(" (a <= b) A = %d Is Less Than or equal to B = %d            \t Answer = %d\n", PLP_A, PLP_B, PLP_RESULT);

	PLP_RESULT = (PLP_A >= PLP_B);
	printf(" (a >= b) A = %d Is greater than or equal to B = %d         \t Answer = %d\n", PLP_A, PLP_B, PLP_RESULT);

	PLP_RESULT = (PLP_A == PLP_B);
	printf(" (a == b) A = %d Is equal to B = %d                         \t Answer = %d\n", PLP_A, PLP_B, PLP_RESULT);

	PLP_RESULT = (PLP_A != PLP_B);
	printf(" (a != b) A = %d Is not equal to B = %d                     \t Answer = %d\n", PLP_A, PLP_B, PLP_RESULT);

	getch();
	return(0);
}