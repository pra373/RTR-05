#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	// Function Prototype
	int MyStrlen(char[]);

	//variable declaration
	char PLP_CARRAY[MAX_STRING_LENGTH];
	int PLP_STRING_LENGTH;
	int count_A = 0, count_E = 0, count_I = 0, count_O = 0, count_U = 0;
	int PLP_I;

	//code

	//**** String input *****

	printf("\n\n");
	printf("Enter a String : \n\n");
	gets_s(PLP_CARRAY, MAX_STRING_LENGTH);

	//******** string output ***********

	printf("\n\n");
	printf("The string entered by you Is : \n\n");
	printf("%s\n", PLP_CARRAY);

	PLP_STRING_LENGTH = MyStrlen(PLP_CARRAY);

	for (PLP_I = 0; PLP_I < PLP_STRING_LENGTH; PLP_I++)
	{
		switch (PLP_CARRAY[PLP_I])
		{
		case 'A':
		case 'a':
			count_A++;
			break;
		case 'E':
		case 'e':
			count_E++;
			break;
		case 'I':
		case 'i':
			count_I++;
			break;
		case 'O':
		case 'o':
			count_O++;
			break;
		case 'U':
		case 'u':
			count_U++;
			break;
		default:
			break;



		}

	}

	printf("\n\n");
	printf("In the string Entered by you,the Vowels and the number of their occurences are as follows : \n\n");
	printf("'A' has occured = %d times !!!\n\n", count_A);
	printf("'E' has occured = %d times !!!\n\n", count_E);
	printf("'I' has occured = %d times !!!\n\n", count_I);
	printf("'O' has occured = %d times !!!\n\n", count_O);
	printf("'U' has occured = %d times !!!\n\n", count_U);

	return(0);

}

int MyStrlen(char str[])
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

