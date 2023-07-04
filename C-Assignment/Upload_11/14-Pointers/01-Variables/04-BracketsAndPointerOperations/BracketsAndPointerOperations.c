#include<stdio.h>

int main(void)
{
	// variable declarations
	int PLP_NUM;
	int *ptr = NULL;
	int PLP_ANS;

	//code
	PLP_NUM = 5;
	ptr = &PLP_NUM;


	printf("\n\n");
	printf(" PLP_NUM        = %d\n", PLP_NUM);
	printf(" &PLP_NUM       = %p\n", &PLP_NUM);
	printf(" *(&PLP_NUM)    = %d\n", *(&PLP_NUM));
	printf(" PTR            = %p\n", ptr);
	printf(" *PTR           = %d\n", *ptr);

	printf("\n\n");

	// add 10 to ptr which is the address of 'PLP_NUM'
	// Hence 10 will be added to the resultant address of PLP_NUM

	printf("Answer of (ptr+10)     = %p\n", (ptr + 10));

	// add 10 to ptr which is the address of 'PLP_NUM' and print value at the new address

	//hence 10 will be added to the address of PLP_NUM and the value at resultant
	// address will be displayed

	printf("Answer of *(ptr+10)    = %d\n", *(ptr + 10));

	//add 10 to the value of PLP_NUM i.e (*ptr + 10) and give new value without any change in address
	//hence 10 will be added to '*ptr' and resultant value will be given (*ptr+10)=(5+10)=15

	printf("Answer of (*ptr + 10)  = %d\n\n", (*ptr + 10));

	// Associativity of * (Value at address) operator AND ++ AND -- OPERATIONS IS FROM RIGHT to LEFT
	// In below statement first * operator will be considered and then ++ operator will be considered
	// so changes are being made in the value of the variable

	++ *ptr;

	printf("Answer of ++ *ptr      = %d\n", *ptr);

	// Associativity of * (Value at address) operator AND ++ AND -- OPERATIONS IS FROM RIGHT to LEFT
	// so in below statement changes are being made in the address first (ptr) and then value at the new address will be incremented

	*ptr++;

	printf("Answer of *ptr ++      = %d\n", *ptr);

	ptr = &PLP_NUM;

	//correct method to post-incrementing a value using pointer .. 
	// brackets have highest praority

	(*ptr)++; 

	printf("Answer of (*ptr)++     = %d\n\n", *ptr);

	return(0);



}