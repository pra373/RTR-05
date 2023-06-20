#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function prototype
	int MyStrlen(char[]);
	

	//variable declaration
	char PLP_CARRAY[MAX_STRING_LENGTH];
	int PLP_STRING_LENGTH, PLP_I, word_count = 0, space_count = 0;

	//code

	//**** String input *****

	printf("\n\n");
	printf("Enter a String : \n\n");
	gets_s(PLP_CARRAY, MAX_STRING_LENGTH);

	PLP_STRING_LENGTH = MyStrlen(PLP_CARRAY);

	for (PLP_I = 0; PLP_I < PLP_STRING_LENGTH; PLP_I++)
	{
		switch (PLP_CARRAY[PLP_I])
		{
		case 32: //ASCII value for space (' ') character
			space_count++;
			break;
		default:
			break;
		}
	}

	word_count = space_count + 1;

	//******** string output ***********

	printf("\n\n");
	printf("The string entered by you Is : \n\n");
	printf("%s\n", PLP_CARRAY);

	printf("\n\n");
	printf("\n\n");
	printf("Number of spaces in the input string = %d\n\n", space_count);
	printf("Number of words in the input string = %d\n\n", word_count);

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