#include<stdio.h>

#define MAX_STRING_LENGTH 512 

int main(void)
{
	char PLP_CARRAY_ORIGINAL[MAX_STRING_LENGTH], PLP_CARRAY_COPY[MAX_STRING_LENGTH];

	//code
	//String input 
	printf("\n\n");
	printf("Enter a string : \n\n");
	gets_s(PLP_CARRAY_ORIGINAL, MAX_STRING_LENGTH);

	//string copy

	strcpy(PLP_CARRAY_COPY, PLP_CARRAY_ORIGINAL);

	//String output

	printf("\n\n");
	printf("The original string entered by you (i.e : 'PLP_CARRAY_ORIGINAL[]')\n\n");
	printf("%s\n", PLP_CARRAY_ORIGINAL);
	printf("\n\n");
	printf("The copied string (i.e : 'PLP_CARRAY_COPY[]')\n\n");
	printf("%s\n", PLP_CARRAY_COPY);

	return(0);



}