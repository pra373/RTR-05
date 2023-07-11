#include<stdio.h>
#include<stdlib.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function declaration
	int MyStrLen(char *);

	//variable declaration
	char *chArray = NULL;  // character array can be representated by a char pointer to mark the base address (char *)

	int iStringLength = 0;

	//code

	printf("\n\n");

	chArray = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));

	if (chArray == NULL)
	{
		printf("Memory allocation to Character array failed !!! Exiting now.\n\n");
		exit(0);
	}

	//string input

	printf("Enter a string : \n\n");
	gets_s(chArray, MAX_STRING_LENGTH);

	//string output

	printf("\n\n");
	printf("String entered by you is : \n\n");
	printf("%s\n", chArray);

	//string length
	printf("\n\n");

	iStringLength = MyStrLen(chArray);
	printf("Length of string is %d Characters !!! \n\n", iStringLength);

	if (chArray)
	{
		free(chArray);
		chArray = NULL;
	}

	return(0);




}

int MyStrLen(char *str)
{
	// variable declarations
	int PLP_J, STRING_LENGTH = 0;

	//code

	for (PLP_J = 0; PLP_J < MAX_STRING_LENGTH; PLP_J++)
	{
		if (*(str + PLP_J) == '\0')
		{
			break;
		}
		else
		{
			STRING_LENGTH++;
		}
	}

	return(STRING_LENGTH);
}

