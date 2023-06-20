#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//variable declaration
	char PLP_CARRAY_1[MAX_STRING_LENGTH], PLP_CARRAY_2[MAX_STRING_LENGTH];

	//code

	//**** String input *****
	printf("\n\n");
	printf("Enter First String : \n\n");
	gets_s(PLP_CARRAY_1, MAX_STRING_LENGTH);

	printf("\n\n");
	printf("Enter Second String : \n\n");
	gets_s(PLP_CARRAY_2, MAX_STRING_LENGTH);

	// **** string concat ****
	printf("\n\n");
	printf("********* Before Concatenation ********");
	printf("\n\n");
	printf("The original First string entered by you (i.e : 'PLP_CARRAY_1[]' Is : \n\n");
	printf("%s\n", PLP_CARRAY_1);

	printf("\n\n");
	printf("The original Second string entered by you (i.e : 'PLP_CARRAY_2[]' Is : \n\n");
	printf("%s\n", PLP_CARRAY_2);

	strcat(PLP_CARRAY_1, PLP_CARRAY_2);

	printf("\n\n");
	printf("*************** After Concatination *****************");
	printf("\n\n");

	printf("PLP_CARRAY_1[] is : \n\n");
	printf("%s\n", PLP_CARRAY_1);

	printf("\n\n");
	printf("PLP_CARRAY_2[] is : \n\n");
	printf("%s\n", PLP_CARRAY_2);

	return(0);


}