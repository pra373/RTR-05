#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	// FUnction prototype
	int MyStrLen(char[]);

	// variable declaration
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
	PLP_STRING_LENGTH = MyStrLen(PLP_CARRAY);
	printf("Length of string is = %d characters !!!\n\n", PLP_STRING_LENGTH);

	return(0);
}

int MyStrLen(char str[])
{
	int PLP_J, PLP_STRING_LENGTH = 0;
	// Determining exact length of the string, by detecting the first occurence of Null-Terminating character(\0)

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
