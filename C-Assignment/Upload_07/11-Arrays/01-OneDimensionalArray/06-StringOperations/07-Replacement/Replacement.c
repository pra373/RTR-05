// **** This program replaces all vowels in the input with the * (asterish) symbol

#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function prototype
	int MyStrlen(char[]);
	void MyStrCpy(char[], char[]);

	//variable declaration

	char PLP_CARRAY_ORIGINAL[MAX_STRING_LENGTH];
	char PLP_CARRAY_VOWELREPLACED[MAX_STRING_LENGTH];
	int PLP_STRING_LENGTH, PLP_I;
	
	//code

	//**** String input *****

	printf("\n\n");
	printf("Enter a String : \n\n");
	gets_s(PLP_CARRAY_ORIGINAL, MAX_STRING_LENGTH);

	//******** string output ***********

	MyStrCpy(PLP_CARRAY_VOWELREPLACED, PLP_CARRAY_ORIGINAL);

	PLP_STRING_LENGTH = MyStrlen(PLP_CARRAY_VOWELREPLACED);

	for (PLP_I = 0; PLP_I < PLP_STRING_LENGTH; PLP_I++)
	{
		switch (PLP_CARRAY_VOWELREPLACED[PLP_I])
		{
		case 'A':
		case 'a':
		case 'E':
		case 'e':
		case 'I':
		case 'i':
		case 'o':
		case 'O':
		case 'U':
		case 'u':
			PLP_CARRAY_VOWELREPLACED[PLP_I] = '*';
			break;
		default:
			break;
		

			
		}
	}

	printf("\n\n");
	printf("The string entered by you Is : \n\n");
	printf("%s\n", PLP_CARRAY_ORIGINAL);

	printf("\n\n");
	printf("The string after Replacement of vowels by * Is : \n\n");
	printf("%s\n", PLP_CARRAY_VOWELREPLACED);
	return(0);



}

MyStrlen(char str[])
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

void MyStrCpy(char str_destination[], char str_source[])
{
	// Function prototype
	int MyStrlen(char[]);

	//Variable declaration

	int PLP_STRING_LENGTH = 0;
	int PLP_J;

	//code
	PLP_STRING_LENGTH = MyStrlen(str_source);
	for (PLP_J = 0; PLP_J < PLP_STRING_LENGTH; PLP_J++)
	{
		str_destination[PLP_J] = str_source[PLP_J];

	}
	str_destination[PLP_J] = '\0';



}

