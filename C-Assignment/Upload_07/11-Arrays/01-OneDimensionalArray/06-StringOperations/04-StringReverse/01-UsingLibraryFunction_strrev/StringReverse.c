#include<stdio.h>
#include<string.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	// variable declaration
	char PLP_CARRAY[MAX_STRING_LENGTH];

	//code

	//String Input

	printf("\n\n");
	printf("Enter a String : \n\n");
	gets_s(PLP_CARRAY, MAX_STRING_LENGTH);

	//String output

	printf("\n\n");
	printf("The original string entered by you (i.e : 'PLP_CARRAY[]') is \n\n");
	printf("%s\n", PLP_CARRAY);

	printf("\n\n");
	printf("The reversed string is : \n\n");
	printf("%s\n", strrev(PLP_CARRAY));

	return(0);




}