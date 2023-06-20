#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function prototype
	int MyStrlen(char[]);

	//variable declaration
	char PLP_CARRAY_ORIGINAL[MAX_STRING_LENGTH], PLP_CARRAY_SPACES_REMOVED[MAX_STRING_LENGTH];

	int PLP_STRING_LENGTH, PLP_I=0, PLP_J=0;

	//code

	//**** String input *****

	printf("\n\n");
	printf("Enter a String : \n\n");
	gets_s(PLP_CARRAY_ORIGINAL, MAX_STRING_LENGTH);

	PLP_STRING_LENGTH = MyStrlen(PLP_CARRAY_ORIGINAL);

	for (PLP_I = 0; PLP_I < PLP_STRING_LENGTH; PLP_I++)
	{
		if (PLP_CARRAY_ORIGINAL[PLP_I] == ' ')
		{
			continue;
		}
		else
		{
			PLP_CARRAY_SPACES_REMOVED[PLP_J] = PLP_CARRAY_ORIGINAL[PLP_I];
			PLP_J++;
		}
	}

	PLP_CARRAY_SPACES_REMOVED[PLP_J] = '\0';

	//******** string output ***********

	printf("\n\n");
	printf("The string entered by you Is : \n\n");
	printf("%s\n", PLP_CARRAY_ORIGINAL);

	printf("\n\n");
	printf("The string after removal of spaces Is : \n\n");
	printf("%s\n", PLP_CARRAY_SPACES_REMOVED);

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