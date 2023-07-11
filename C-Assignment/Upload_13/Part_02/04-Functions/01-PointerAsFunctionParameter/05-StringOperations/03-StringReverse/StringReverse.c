#include<stdio.h>
#include<stdlib.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function declaration
	int MyStrLen(char *);
	void MyStrRev(char *, char *);

	//variable declaration
	char *PLP_ORIGINAL_ARRAY = NULL;
	char *PLP_ARRAY_REVERSED = NULL;

	int original_string_length;

	//code
	//string input
	PLP_ORIGINAL_ARRAY = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
	
	if (PLP_ORIGINAL_ARRAY == NULL)
	{
		printf("Memory allocation to original string failed !!! Exiting now.\n\n");
		exit(0);
	}

	printf("Enter a string : \n\n");
	gets_s(PLP_ORIGINAL_ARRAY, MAX_STRING_LENGTH);

	//string reverse

	original_string_length = MyStrLen(PLP_ORIGINAL_ARRAY);

	PLP_ARRAY_REVERSED = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));

	if (PLP_ARRAY_REVERSED == NULL)
	{
		printf("Memory allocation to reversed string failed !!! Exiting now.\n\n");
		exit(0);
	}

	MyStrRev(PLP_ARRAY_REVERSED, PLP_ORIGINAL_ARRAY);

	//string output

	printf("\n\n");
	printf("The original String entered by you (PLP_ORIGINAL_ARRAY) is : \n\n");
	printf("%s\n", PLP_ORIGINAL_ARRAY);

	printf("\n\n");
	printf("The reversed String (PLP_ARRAY_REVERSED) is : \n\n");
	printf("%s\n", PLP_ARRAY_REVERSED);

	if (PLP_ARRAY_REVERSED)
	{
		free(PLP_ARRAY_REVERSED);
		PLP_ARRAY_REVERSED = NULL;
		printf("\n\n");
		printf("Memory allocated for reversed string has been freed successfully !!! \n\n");
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

void MyStrRev(char *str_destination, char *str_source)
{
	int MyStrLen(char *);

	//variable declaration
	int istringlength = 0;
	int j, i, len;

	//code

	istringlength = MyStrLen(str_source);

	// array indices begin from 0 hence last index will be (Length - 1)

	len = istringlength - 1;

	for (i = 0, j = len; i < istringlength, j >= 0; i++, j--)
	{
		*(str_destination + i) = *(str_source + j);
	}

	*(str_destination + i) = '\0';
	
}






