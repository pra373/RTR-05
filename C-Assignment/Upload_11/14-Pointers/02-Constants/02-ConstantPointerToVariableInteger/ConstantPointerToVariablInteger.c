#include<stdio.h>

int main(void)
{
	//variable declaration

	int PLP_NUM = 5;
	int *const ptr = &PLP_NUM; //ptr is constant pointer to integer variable

	//code

	printf("\n\n");
	printf("Current Value of 'PLP_NUM' = %d\n", PLP_NUM);
	printf("Current 'ptr' (Address of 'num') = %p\n", ptr);

	PLP_NUM++;
	printf("After PLP_NUM++,Value of 'PLP_NUM' = %d\n", PLP_NUM);

	// The following line give error and hence commented out
	// pointer 'ptr' is a constant pointer to integer
	// hence we cannot change the address stored in pointer 'ptr'

	//ptr++;

	// following line does not give error because we are changing the value which is stored in the address
	// so we are not changing the value of constant 'ptr' pointer 
	(*ptr)++;

	printf("\n\n");
	printf("After (*ptr++), value of 'ptr' = %p\n", ptr);
	printf("Value at this 'ptr' = %d\n", *ptr);
	return(0);





}