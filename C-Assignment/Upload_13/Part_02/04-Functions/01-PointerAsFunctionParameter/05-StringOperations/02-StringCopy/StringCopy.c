#include<stdio.h>
#include<stdlib.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function declaration
	int MyStrLen(char *);
	void MyStrCopy(char *, char *);

	//variable declaration
	char *PLP_ORIGINAL_ARRAY = NULL;
	char *PLP_ARRAY_COPY = NULL;

	int original_string_length;

	//code

	//string input

	printf("\n\n");

	PLP_ORIGINAL_ARRAY = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));

	if (PLP_ORIGINAL_ARRAY == NULL)
	{
		printf("Memory allocation to original string failed !!! Exiting now.\n\n");
		exit(0);
	}

	printf("Enter a string : \n\n");
	gets_s(PLP_ORIGINAL_ARRAY, MAX_STRING_LENGTH);

	original_string_length = MyStrLen(PLP_ORIGINAL_ARRAY);

	PLP_ARRAY_COPY = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));

	if (PLP_ARRAY_COPY == NULL)
	{
		printf("Memory allocation to copy string failed !!! Exiting now.\n\n");
		exit(0);

	}

	

	MyStrCopy(PLP_ARRAY_COPY, PLP_ORIGINAL_ARRAY);

	//string output

	printf("\n\n");
	printf("The original String entered by you (PLP_ORIGINAL_ARRAY) is : \n\n");
	printf("%s\n", PLP_ORIGINAL_ARRAY);

	printf("\n\n");
	printf("The copied String (PLP_ARRAY_COPY) is : \n\n");
	printf("%s\n", PLP_ARRAY_COPY);

	if (PLP_ARRAY_COPY)
	{
		free(PLP_ARRAY_COPY);
		PLP_ARRAY_COPY = NULL;
		printf("\n\n");
		printf("Memory allocated for copied string has been freed successfully !!! \n\n");
	}

	if (PLP_ORIGINAL_ARRAY)
	{
		free(PLP_ORIGINAL_ARRAY);
		PLP_ORIGINAL_ARRAY = NULL;
		printf("\n\n");
		printf("Memory allocated for original string has been freed successfully !!! \n\n");
	}

	return(0);
}

void MyStrCopy(char *str_destination, char *str_source)
{
	//function prototype
	int MyStrLen(char *);

	//variable declaration

	int istringlength = 0;
	int j;

	//code

	istringlength = MyStrLen(str_source);
	for (j = 0; j < istringlength; j++)
	{
		*(str_destination + j) = *(str_source + j);
	}

	*(str_destination + j) = '\0';




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
