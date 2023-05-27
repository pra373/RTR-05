#include<stdio.h>

int main(void)
{
	//Function prototype
	void PrintBinaryFormOfNumber(unsigned int);

	// Variable Declaration
	unsigned int a;
	unsigned int b;
	unsigned int result;

	//code

	printf("\n\n");
	printf("Enter An Integer = ");
	scanf("%u", &a);

	printf("\n\n");
	printf("Enter Another Integer = ");
	scanf("%u", &b);

	printf("\n\n\n\n");
	result = a ^ b;
	printf("Bitwise XOR-ing of A = %d (Decimal) and B = %d (Decimal) gives result %d (Decimal).\n\n", a, b, result);

	PrintBinaryFormOfNumber(a);
	PrintBinaryFormOfNumber(b);
	PrintBinaryFormOfNumber(result);

	return(0);


}

void PrintBinaryFormOfNumber(unsigned int decimal_number)
{
	//Variable declarations
	unsigned int quotient, remainder, num;
	unsigned int binary_array[8];
	int i;

	//code

	for (i = 0; i < 8; i++)
	{
		binary_array[i] = 0;
	}
	printf("The Binary Form of the Decimal Integer %d is \t=\t", decimal_number);
	num = decimal_number;
	i = 7;

	while (num != 0)
	{
		quotient = num / 2;
		remainder = num % 2;
		binary_array[i] = remainder;
		num = quotient;
		i--;
	}

	for (i = 0; i < 8; i++)
	{
		printf("%u", binary_array[i]);

	}



	printf("\n\n");



}
