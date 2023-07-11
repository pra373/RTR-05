#include<stdio.h>
#include<stdlib.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function declaration
	int MyStrLen(char *);
	void MyStrCat(char *, char *);

	//variable declaration
	char *PLP_CARRAY_ONE = NULL;
	char *PLP_CARRAY_TWO = NULL;

	//code

	//string input

	printf("\n\n");
	PLP_CARRAY_ONE = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));

	if (PLP_CARRAY_ONE == NULL)
	{
		printf("Memory allocation to first string failed !!! Exiting now.\n\n");
		exit(0);
	}

	printf("Enter First string : \n\n");
	gets_s(PLP_CARRAY_ONE, MAX_STRING_LENGTH);

	printf("\n\n");

	PLP_CARRAY_TWO = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));

	if (PLP_CARRAY_TWO == NULL)
	{
		printf("Memory allocation to second string failed !!! Exiting now.\n\n");
		exit(0);
	}

	printf("Enter second string : \n\n");
	gets_s(PLP_CARRAY_TWO, MAX_STRING_LENGTH);

	// string concat

	printf("\n\n");
	printf("************** Before Concatination *********\n\n");
	printf("First string Entered by you (i.e : PLP_CARRAY_ONE[] ) is : \n\n");
	printf("%s\n", PLP_CARRAY_ONE);

	printf("\n\n");
	printf("second string Entered by you (i.e : PLP_CARRAY_TWO[] ) is : \n\n");
	printf("%s\n", PLP_CARRAY_TWO);

	MyStrCat(PLP_CARRAY_ONE, PLP_CARRAY_TWO);

	printf("\n\n");
	printf("************** After Concatination *********\n\n");

	printf("'PLP_CARRAY_ONE[]' is : \n\n");
	printf("%s\n", PLP_CARRAY_ONE);

	printf("\n\n");
	printf("'PLP_CARRAY_TWO[]' is : \n\n");
	printf("%s\n", PLP_CARRAY_TWO);

	if (PLP_CARRAY_TWO)
	{
		free(PLP_CARRAY_TWO);
		PLP_CARRAY_TWO = NULL;
		printf("\n\n");
		printf("Memory allocated for second string has been freed successfully !!! \n\n");
	}

	if (PLP_CARRAY_ONE)
	{
		free(PLP_CARRAY_ONE);
		PLP_CARRAY_ONE = NULL;
		printf("\n\n");
		printf("Memory allocated for first string has been freed successfully !!! \n\n");
	}

	return(0);

}

void MyStrCat(char *str_destination, char *str_source)
{
	int MyStrLen(char *);

	//variable declaration
	int istringlength_source = 0, istringlength_destination = 0, i, j;

	//code

	istringlength_source = MyStrLen(str_source);
	istringlength_destination = MyStrLen(str_destination);

	// array indices begin from 0 hence last index will be (Length - 1)

	// concatination begins from index number equal to length of 'str_destination'

	for (i = istringlength_destination, j = 0; j < istringlength_source; i++, j++)
	{
		*(str_destination + i) = *(str_source + j);
	}

	*(str_destination + i) = '\0';



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



