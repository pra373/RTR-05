#include<stdio.h>

int main(void)
{
	//variable declaration

	double PLP_NUM;
	double *ptr = NULL; // *ptr is a variable of type 'double'

	//code

	PLP_NUM = 657.78651278965;

	printf("\n\n");

	printf("********** Before ptr = &PLP_NUM *********\n\n");
	printf("Value of 'PLP_NUM'            = %lf\n\n", PLP_NUM);
	printf("Address of 'PLP_NUM'          = %p\n\n", &PLP_NUM);
	printf("Value at address of 'PLP_NUM' = %lf\n\n", *(&PLP_NUM));

	//Assigning address of variable 'PLP_NUM' to pointer variable 'ptr'
	//'ptr' now contains the address of PLP_NUM

	ptr = &PLP_NUM;

	printf("\n\n");
	printf("********** After ptr = &PLP_NUM *********\n\n");
	printf("Value of 'PLP_NUM'            = %lf\n\n", PLP_NUM);
	printf("Address of 'PLP_NUM'          = %p\n\n", ptr);
	printf("Value at address of 'PLP_NUM' = %lf\n\n", *ptr);

	return(0);







}