#include<stdio.h>
#include<stdlib.h>

int main(void)
{
	//function declaration
	void MathematicalOperations(int, int, int *, int *, int *, int *, int *);
	
	//variable declaration
	int PLP_A, PLP_B;

	int *PLP_ANSWER_SUM = NULL;
	int *PLP_ANSWER_DIFFERENCE = NULL;
	int *PLP_ANSWER_PRODUCT = NULL;
	int *PLP_ANSWER_QUOTIENT = NULL;
	int *PLP_ANSWER_REMAINDER = NULL;

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
	
	
	PLP_ANSWER_SUM = (int *)malloc(1 * sizeof(int));
	if (PLP_ANSWER_SUM == NULL)
	{
		printf("Could not allocate memory for 'PLP_ANSWER_SUM'. Exiting now ....\n\n");
		exit(0);
	}

	PLP_ANSWER_DIFFERENCE = (int *)malloc(1 * sizeof(int));
	if (PLP_ANSWER_DIFFERENCE == NULL)
	{
		printf("Could not allocate memory for 'PLP_ANSWER_DIFFERENCE'. Exiting now ....\n\n");
		exit(0);
	}

	PLP_ANSWER_PRODUCT = (int *)malloc(1 * sizeof(int));
	if (PLP_ANSWER_PRODUCT == NULL)
	{
		printf("Could not allocate memory for 'PLP_ANSWER_PRODUCT'. Exiting now ....\n\n");
		exit(0);
	}

	PLP_ANSWER_QUOTIENT = (int *)malloc(1 * sizeof(int));
	if (PLP_ANSWER_QUOTIENT == NULL)
	{
		printf("Could not allocate memory for 'PLP_ANSWER_QUOTIENT'. Exiting now ....\n\n");
		exit(0);
	}

	PLP_ANSWER_REMAINDER = (int *)malloc(1 * sizeof(int));
	if (PLP_ANSWER_REMAINDER == NULL)
	{
		printf("Could not allocate memory for 'PLP_ANSWER_REMAINDER'. Exiting now ....\n\n");
		exit(0);
	}

	MathematicalOperations(PLP_A, PLP_B, PLP_ANSWER_SUM, PLP_ANSWER_DIFFERENCE, PLP_ANSWER_PRODUCT, PLP_ANSWER_QUOTIENT, PLP_ANSWER_REMAINDER);


	printf("\n\n");
	printf("************** Results ************\n\n");
	printf("Sum = %d\n\n", *PLP_ANSWER_SUM);
	printf("Difference = %d\n\n", *PLP_ANSWER_DIFFERENCE);
	printf("Product = %d\n\n", *PLP_ANSWER_PRODUCT);
	printf("Quotient = %d\n\n", *PLP_ANSWER_QUOTIENT);
	printf("Remainder = %d\n\n", *PLP_ANSWER_REMAINDER);


	if (PLP_ANSWER_REMAINDER)
	{
		free(PLP_ANSWER_REMAINDER);
		PLP_ANSWER_REMAINDER = NULL;
		printf("Memory allocated for 'PLP_ANSWER_REMAINDER' is freed successfully !! \n\n");
	}

	if (PLP_ANSWER_QUOTIENT)
	{
		free(PLP_ANSWER_QUOTIENT);
		PLP_ANSWER_QUOTIENT = NULL;
		printf("Memory allocated for 'PLP_ANSWER_QUOTIENT' is freed successfully !! \n\n");
	}
	

	if (PLP_ANSWER_PRODUCT)
	{
		free(PLP_ANSWER_PRODUCT);
		PLP_ANSWER_PRODUCT = NULL;
		printf("Memory allocated for 'PLP_ANSWER_PRODUCT' is freed successfully !! \n\n");
	}


	if (PLP_ANSWER_DIFFERENCE)
	{
		free(PLP_ANSWER_DIFFERENCE);
		PLP_ANSWER_DIFFERENCE = NULL;
		printf("Memory allocated for 'PLP_ANSWER_DIFFERENCE' is freed successfully !! \n\n");
	}

	if (PLP_ANSWER_SUM)
	{
		free(PLP_ANSWER_SUM);
		PLP_ANSWER_SUM = NULL;
		printf("Memory allocated for 'PLP_ANSWER_SUM' is freed successfully !! \n\n");
	}

	return(0);
}

void MathematicalOperations(int x, int y, int *sum, int *difference, int *product, int *quotient, int *remainder)
{
	//code

	*sum = x + y;         //value at address of 'sum'
	*difference = x - y;
	*product = x * y;
	*quotient = x / y;
	*remainder = x % y;
}