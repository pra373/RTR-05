#include<stdio.h>

int main(void)
{
	//variable declaration

	int PLP_NUM;
	int* ptr = NULL; // 'ptr' is a variable of type 'int*'

	//code

	PLP_NUM = 76;

	printf("\n\n");

	printf("********** Before ptr = &PLP_NUM *********\n\n");
	printf("Value of 'PLP_NUM'            = %d\n\n", PLP_NUM);
	printf("Address of 'PLP_NUM'          = %p\n\n", &PLP_NUM);
	printf("Value at address of 'PLP_NUM' = %d\n\n", *(&PLP_NUM));

	//Assigning address of variable 'PLP_NUM' to pointer variable 'ptr'
	//'ptr' now contains the address of PLP_NUM

	ptr = &PLP_NUM;

	printf("\n\n");
	printf("********** After ptr = &PLP_NUM *********\n\n");
	printf("Value of 'PLP_NUM'            = %d\n\n", PLP_NUM);
	printf("Address of 'PLP_NUM'          = %p\n\n", ptr);
	printf("Value at address of 'PLP_NUM' = %d\n\n", *ptr);

	return(0);







}