#include<stdio.h>

int main(void)
{
	//variable declaration

	char PLP_CH;
	char *ptr = NULL; // *ptr is a variable of type 'char'

	//code

	PLP_CH = 'P';

	printf("\n\n");

	printf("********** Before ptr = &PLP_NUM *********\n\n");
	printf("Value of 'PLP_CH'            = %c\n\n", PLP_CH);
	printf("Address of 'PLP_CH'          = %p\n\n", &PLP_CH);
	printf("Value at address of 'PLP_CH' = %c\n\n", *(&PLP_CH));

	//Assigning address of variable 'PLP_NUM' to pointer variable 'ptr'
	//'ptr' now contains the address of PLP_NUM

	ptr = &PLP_CH;

	printf("\n\n");
	printf("********** After ptr = &PLP_NUM *********\n\n");
	printf("Value of 'PLP_CH'            = %c\n\n", PLP_CH);
	printf("Address of 'PLP_CH'          = %p\n\n", ptr);
	printf("Value at address of 'PLP_CH' = %c\n\n", *ptr);

	return(0);







}