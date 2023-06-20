#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function prototype
	void Mystrrev(char[], char[]);

	char PLP_CARRAY_ORIGINAL[MAX_STRING_LENGTH];
	char PLP_CARRAY_REVERSED[MAX_STRING_LENGTH];

	//code
	//string Input

	printf("\n\n");
	printf("Enter a String : \n\n");
	gets_s(PLP_CARRAY_ORIGINAL, MAX_STRING_LENGTH);

	//String reverse

	Mystrrev(PLP_CARRAY_REVERSED, PLP_CARRAY_ORIGINAL);

	//string output

	printf("\n\n");
	printf("The original string entered by you (i.e : 'PLP_CARRAY_ORIGINAL[]') is \n\n");
	printf("%s\n", PLP_CARRAY_ORIGINAL);

	printf("\n\n");
	printf("The reversed string (i.e : 'PLP_CARRAY_REVERSED[]') is \n\n");
	printf("%s\n", PLP_CARRAY_REVERSED);


	return(0);


}

void Mystrrev(char str_destination[], char str_source[])
{
	//function prototype
	int Mystrlen(char[]);

	//variable declaration

	int PLP_STR_LENGTH = 0;
	int PLP_I, PLP_J, PLP_LEN;
	
	//code

	PLP_STR_LENGTH = Mystrlen(str_source);

	//Array indices begin from 0 hence last index will be (PLP_STR_LENGTH-1)

	PLP_LEN = PLP_STR_LENGTH - 1;

	for (PLP_I = 0, PLP_J = PLP_LEN; PLP_I < PLP_STR_LENGTH, PLP_J >= 0; PLP_I++, PLP_J--)
	{
		str_destination[PLP_I] = str_source[PLP_J];

	}

	str_destination[PLP_I] = '\0';




}

int Mystrlen(char str[])
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

