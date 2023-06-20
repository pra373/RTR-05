#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	char PLP_CARRAY[MAX_STRING_LENGTH];
	int PLP_STRING_LENGTH = 0;

	// string input

	printf("\n\n");
	printf("Enter a String : \n\n");
	gets_s(PLP_CARRAY, MAX_STRING_LENGTH);

	//String Output
	printf("\n\n");
	printf("String entered by you is : %s \n\n", PLP_CARRAY);
	
	//string length

	printf("\n\n");
	PLP_STRING_LENGTH = strlen(PLP_CARRAY);
	printf("Length of string is = %d characters !!!\n\n", PLP_STRING_LENGTH);

	return(0);

}

