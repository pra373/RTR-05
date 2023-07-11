#include<stdio.h>

int main(void)
{
	//variable declaration

	int PLP_NUM;
	int *ptr = NULL;
	int **pptr = NULL;  //*pptr is a double pointer variable pointing to a pointer variable.

	//code

	PLP_NUM = 10;

	printf("\n\n");

	printf("************ Before ptr = &num *********\n\n");
	printf("Value of 'PLP_NUM'                 = %d\n\n", PLP_NUM);
	printf("Address of 'PLP_NUM'               = %p\n\n", &PLP_NUM);
	printf("Value at address of 'PLP_NUM'      = %d\n\n",  *(& PLP_NUM));

	ptr = &PLP_NUM;

	//ptr now contains address of variable 'PLP_NUM'
	
	printf("\n\n");

	printf("************ After ptr = &num *********\n\n");
	printf("Value of 'PLP_NUM'                 = %d\n\n", PLP_NUM);
	printf("Address of 'PLP_NUM'               = %p\n\n", ptr);
	printf("Value at address of 'PLP_NUM'      = %d\n\n", *ptr);

	//assigning address of variable 'ptr' to double pointer 'pptr'
	//pptr now contains the address of 'ptr' which in turn contains the address of 'num'

	pptr = &ptr;

	printf("\n\n");

	printf(" ****************** after pptr = &ptr **********\n\n");
	printf("Value of 'PLP_NUM'                            = %d\n\n", PLP_NUM);
	printf("Address of 'PLP_NUM' (ptr)                    = %p\n\n", ptr);
	printf("Address of 'ptr' (pptr)                       = %p\n\n", pptr);
	printf("Value at address of 'ptr' (*pptr)             = %p\n\n", *pptr);
	printf("value at address of 'PLP_NUM' (*ptr) (**pptr) = %d\n\n", **pptr);
	return(0);
}