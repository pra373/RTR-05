#include<stdio.h>
#include<stdlib.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function declaration
	char * ReplaceVowelsWithHashSymbol(char *);

	//variable declaration
	char string[MAX_STRING_LENGTH];
	char *replaced_string = NULL;

	//code

	printf("\n\n");
	printf("Enter string : ");
	gets_s(string, MAX_STRING_LENGTH);

	replaced_string = ReplaceVowelsWithHashSymbol(string);

	if (replaced_string == NULL)
	{
		printf("ReplaceVowelsWithHashSymbol() function has failed !!! Exiting now ... \n\n");
		exit(0);
	}

	printf("\n\n");
	printf("Replaced string is : \n\n");
	printf("%s\n\n", replaced_string);

	if (replaced_string)
	{
		free(replaced_string);
		replaced_string = NULL;


	}

	return(0);



}

char * ReplaceVowelsWithHashSymbol(char *s)
{
	//function prototype
	void MyStrCopy(char *, char *);
	int MyStrLen(char *);

	//variable declaration
	char *new_string = NULL;
	int i;

	//code

	new_string = (char *)malloc(MyStrLen(s) * sizeof(char));
	if (new_string == NULL)
	{
		printf("Could not allocate memory for new string !!!\n\n");
		return(NULL);
	}

	MyStrCopy(new_string, s);

	for (i = 0; i < MyStrLen(new_string); i++)
	{
		switch (new_string[i])
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
			new_string[i] = '#';
			break;
		default:
			break;

		}
	}

	return(new_string);




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


