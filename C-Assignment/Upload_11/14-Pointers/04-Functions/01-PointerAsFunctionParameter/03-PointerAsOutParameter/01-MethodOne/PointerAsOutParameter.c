#include<stdio.h>

int main(void)
{
	//function declaration

	void MathematicalOperations(int, int, int *, int *, int *, int *, int *);

	//variable declaration
	int PLP_A;
	int PLP_B;
	int PLP_ANSWER_SUM;
	int PLP_ANSWER_DIFFERENCE;
	int PLP_ANSWER_QUOTIENT;
	int PLP_ANSWER_REMAINDER;
	int PLP_ANSWER_PRODUCT;

	//code
	printf("\n\n");
	printf("Enter Value For 'PLP_A' : ");
	scanf("%d", &PLP_A);

	printf("\n\n");
	printf("Enter Value For 'PLP_B' : ");
	scanf("%d", &PLP_B);

	// we are passing addresses of variable to the function. This addresses have nothing in them (garbage) currently.
	// But through function definition function will fill thos addresses with values
	//so,This addressess are going as empty parametersin the function but after the function call the function will assign some value at thos address locations
	// so as this parameters go inside as mere addresses with no value assigned but they come out with assigned values so this parameters are called as 'out parameters'
	//or parametarised return

	MathematicalOperations(PLP_A, PLP_B, &PLP_ANSWER_SUM, &PLP_ANSWER_DIFFERENCE, &PLP_ANSWER_PRODUCT, &PLP_ANSWER_QUOTIENT,&PLP_ANSWER_REMAINDER);

	printf("\n\n");
	printf("*************** RESULT ***********\n\n");
	printf("Sum        = %d\n\n", PLP_ANSWER_SUM);
	printf("Difference = %d\n\n", PLP_ANSWER_DIFFERENCE);
	printf("Product    = %d\n\n", PLP_ANSWER_PRODUCT);
	printf("Quotient   = %d \n\n", PLP_ANSWER_QUOTIENT);
	printf("Remainder  = %d\n\n", PLP_ANSWER_REMAINDER);

	return(0);


}

void MathematicalOperations(int x, int y, int *sum, int *difference, int *product, int *quotient, int *remainder)
{
	//code
	*sum = x + y;
	*difference = x - y;
	*product = (x * y);
	*quotient = x / y;
	*remainder = x % y;
}


