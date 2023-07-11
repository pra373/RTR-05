#include<stdio.h>

int main(void)
{
	//function declaration
	void MathematicalOperations(int, int, int *, int *, int *, int *, int *);

	//variable declaration
	int PLP_A, PLP_B, PLP_ANSWER_SUM, PLP_ANSWER_DIFFERENCE, PLP_ANSWER_PRODUCT, PLP_ANSWER_QUOTIENT, PLP_ANSWER_REMAINDER;

	//code

	printf("\n\n");
	printf("Enter value of 'PLP_A' : ");
	scanf("%d", &PLP_A);

	printf("\n\n");
	printf("Enter value of 'PLP_B' : ");
	scanf("%d", &PLP_B);

	// passing address to function ...
	// function will fill these addresses with valid return values
	// so this addresses are going empty and coming out with a valid value assigned to them
	// this type of parameters are called as "out parameters" or "Parameterised return value".
	// hence although each function only has one return value , using the concept of "Parameterised return value" our function has given 5 return values

	 MathematicalOperations(PLP_A, PLP_B, &PLP_ANSWER_SUM, &PLP_ANSWER_DIFFERENCE, &PLP_ANSWER_PRODUCT, &PLP_ANSWER_QUOTIENT, &PLP_ANSWER_REMAINDER);


	printf("\n\n");
	printf("************** Results ************\n\n");
	printf("Sum = %d\n\n", PLP_ANSWER_SUM);
	printf("Difference = %d\n\n", PLP_ANSWER_DIFFERENCE);
	printf("Product = %d\n\n", PLP_ANSWER_PRODUCT);
	printf("Quotient = %d\n\n", PLP_ANSWER_QUOTIENT);
	printf("Remainder = %d\n\n", PLP_ANSWER_REMAINDER);

	return(0);







}

void MathematicalOperations(int x, int y , int *sum, int *difference, int *product, int *quotient, int *remainder)
{
	//code

	*sum = x + y;         //value at address of 'sum'
	*difference = x - y;
	*product = x * y;
	*quotient = x / y;
	*remainder = x % y;
}


