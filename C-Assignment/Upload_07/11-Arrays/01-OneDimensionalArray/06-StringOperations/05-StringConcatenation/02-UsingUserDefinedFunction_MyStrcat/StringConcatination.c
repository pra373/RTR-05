#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	// Function Prototype
	void MyStrcat(char[], char[]);

	//variable declaration
	char PLP_CARRAY_1[MAX_STRING_LENGTH];
	char PLP_CARRAY_2[MAX_STRING_LENGTH];

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

	MyStrcat(PLP_CARRAY_1, PLP_CARRAY_2);

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

void MyStrcat(char str_destination[], char str_source[])
{
	//function prototype
	int MyStrlen(char[]);

	//variable declaration
	int string_length_source = 0;
	int string_length_destination = 0;
	int i, j;

	//code

	string_length_source = MyStrlen(str_source);
	string_length_destination= MyStrlen(str_destination);

	//Array indices begin from 0,Hence last valid index of array will always be (length-1)
	
	// so concatination must begin from index number equal to length of the array 
	//'str_destination'

	//we need to put the character which is at first index of 'str_source to the (last index+1) of 'str_destination'

	for (i = string_length_destination, j = 0; j < string_length_source; i++, j++)
	{
		str_destination[i] = str_source[j];
	}

	str_destination[i] = '\0';




}

int MyStrlen(char str[])
{
	// variable declaration

	int PLP_J;
	int PLP_STRING_LENGTH = 0;

	//code

	for (PLP_J = 0; PLP_J < MAX_STRING_LENGTH; PLP_J++)
	{
		if (str[PLP_J] == '\0')
		{
			break;
		}

		else
		{
			PLP_STRING_LENGTH++;
		}

	}
	return(PLP_STRING_LENGTH);
}


