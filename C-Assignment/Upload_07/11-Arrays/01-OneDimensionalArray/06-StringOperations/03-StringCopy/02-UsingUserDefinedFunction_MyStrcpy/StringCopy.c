#include<stdio.h>

#define MAX_STRING_LENGTH 512 

int main(void)
{
	// Function prototype
	void MyStrCpy(char[], char[]);

	//variable declaration
	char PLP_CARRAY_ORIGINAL[MAX_STRING_LENGTH], PLP_CARRAY_COPY[MAX_STRING_LENGTH];

	//code
	//string input

	printf("\n\n");
	printf("Enter a string : \n\n");
	gets_s(PLP_CARRAY_ORIGINAL, MAX_STRING_LENGTH);

	//String copy

	MyStrCpy(PLP_CARRAY_COPY, PLP_CARRAY_ORIGINAL);

	//String output

	printf("\n\n");
	printf("The original string entered by you (i.e : 'PLP_CARRAY_ORIGINAL[]')\n\n");
	printf("%s\n", PLP_CARRAY_ORIGINAL);
	printf("\n\n");
	printf("The copied string (i.e : 'PLP_CARRAY_COPY[]')\n\n");
	printf("%s\n", PLP_CARRAY_COPY);

	return(0);


}

void MyStrCpy(char str_destination[], char str_source[])
{
	// Function prototype
	int MyStrLen(char[]);

	//Variable declaration

	int PLP_STRING_LENGTH = 0;
	int PLP_J;

	//code
	PLP_STRING_LENGTH = MyStrLen(str_source);
	for (PLP_J = 0; PLP_J < PLP_STRING_LENGTH; PLP_J++)
	{
		str_destination[PLP_J] = str_source[PLP_J];
		
	}
	str_destination[PLP_J] = '\0';



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
