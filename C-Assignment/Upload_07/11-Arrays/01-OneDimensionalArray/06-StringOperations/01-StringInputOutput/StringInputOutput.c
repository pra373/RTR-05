#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	// variable declaration
	char PLP_CHARRAY[MAX_STRING_LENGTH];

	//code

	printf("\n\n");
	printf("Enter a string : \n\n");
	gets_s(PLP_CHARRAY, MAX_STRING_LENGTH);

	printf("\n\n");
	printf("String Entered by you Is : \n\n");
	printf("%s\n", PLP_CHARRAY);
	return(0);

		
}