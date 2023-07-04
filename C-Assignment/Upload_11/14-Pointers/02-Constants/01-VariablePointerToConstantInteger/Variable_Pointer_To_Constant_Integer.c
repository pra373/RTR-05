#include<stdio.h>

int main(void)
{
	//variable declaration

	int PLP_NUM = 5;
	const int *ptr = NULL;

	//code
	ptr = &PLP_NUM;
	printf("\n");
	printf("Current Value of 'PLP_NUM' = %d\n", PLP_NUM);
	printf("Current 'ptr' (Address of 'num') = %p\n", ptr);

	// It is not compulsory that the integer variable to which the pointer is pointing should be a constant
	// it just mean that you cannot change that integers value using 'ptr' pointer

	
	PLP_NUM++;

	printf("\n\n");
	printf("After PLP_NUM++,Value of 'PLP_NUM' = %d\n", PLP_NUM);

	// ***** you cannot change the value of  PLP_NUM by using pointer ptr **********
	// but integer PLP_NUM is not a constant variable you can change the the value of PLP_NUM seperatly
	// integer variable PLP_NUM is constant with respect to pointer 'ptr' only

	// (*ptr)++; // this line will error because you are trying to change the integers value by using pointer 'ptr'

	// but pointer 'ptr' is not constant we can change the address which is stored previously

	ptr++;

	printf("\n\n");
	printf("After ptr++, value of 'ptr' = %p\n", ptr);
	printf("Value at this new 'ptr' = %d\n", *ptr);
	printf("\n\n");
	return(0);










}