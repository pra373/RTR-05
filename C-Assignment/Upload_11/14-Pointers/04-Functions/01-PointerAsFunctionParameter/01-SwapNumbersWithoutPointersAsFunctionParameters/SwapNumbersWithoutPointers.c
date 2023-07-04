#include<stdio.h>

int main(void)
{
	// function declaration

	void SwapNumbers(int, int);

	//variable declaration
	int PLP_A;
	int PLP_B;

	//code
	printf("\n\n");
	printf("Enter Value For 'PLP_A' : ");
	scanf("%d", &PLP_A);

	printf("\n\n");
	printf("Enter Value For 'PLP_B' : ");
	scanf("%d", &PLP_B);

	printf("\n\n");
	printf("********* Before Swapping *********\n\n");
	printf("Value of 'PLP_A' : %d\n\n",PLP_A);
	printf("Value of 'PLP_B' : %d\n\n", PLP_B);

	SwapNumbers(PLP_A, PLP_B); // Argument passed by value

	printf("\n\n");
	printf("********* After Swapping *********\n\n");
	printf("Value of 'PLP_A' : %d\n\n", PLP_A);
	printf("Value of 'PLP_B' : %d\n\n", PLP_B);

	return(0);

}

void SwapNumbers(int a, int b) // value of PLP_A is copied into 'a' and value of PLP_B is copied into 'b'
{								// swapping takes place only for a anf b not for PLP_A and PLP_B
	//variable declarations
	int temp;

	//code
	printf("\n\n");
	printf("********* Before Swapping *********\n\n");
	printf("Value of 'a' : %d\n\n", a);
	printf("Value of 'b' : %d\n\n", b);

	temp = a;
	a = b;
	b = temp;

	printf("\n\n");	
	printf("********* After Swapping *********\n\n");
	printf("Value of 'a' : %d\n\n", a);
	printf("Value of 'b' : %d\n\n", b);

}

