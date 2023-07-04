#include<stdio.h>

int main(void)
{
	//variable declaration

	float PLP_NUM;
	float *ptr = NULL; // *ptr is a variable of type 'float'

	//code

	PLP_NUM = 6.9f;

	printf("\n\n");

	printf("********** Before ptr = &PLP_NUM *********\n\n");
	printf("Value of 'PLP_NUM'            = %f\n\n", PLP_NUM);
	printf("Address of 'PLP_NUM'          = %p\n\n", &PLP_NUM);
	printf("Value at address of 'PLP_NUM' = %f\n\n", *(&PLP_NUM));

	//Assigning address of variable 'PLP_NUM' to pointer variable 'ptr'
	//'ptr' now contains the address of PLP_NUM

	ptr = &PLP_NUM;

	printf("\n\n");
	printf("********** After ptr = &PLP_NUM *********\n\n");
	printf("Value of 'PLP_NUM'            = %f\n\n", PLP_NUM);
	printf("Address of 'PLP_NUM'          = %p\n\n", ptr);
	printf("Value at address of 'PLP_NUM' = %f\n\n", *ptr);

	return(0);







}