#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function prototype
	int MyStrlen(char[]);

	// A string is an array of characters ... so char[] is a char array and hence, char[] is a string.
	// array of char[] is array of strings !!!!
	//hence, char[][] is an array of char arrays hence, ia an array af strings

	/*here the string array can allow a maximum number of 10 strings(10 rows)
	and each of this 10 strings can only have 15 characters maximum (15 columns)*/

	//variable declaration

	char PLP_STR_ARRAY[10][15] = { "Hello!","Welcome","To","Real","Time","Rendering","Batch","(2023-2024)","Of","ASTROMEDICOMP." };//Inline Initialization
	int STR_LENGTHS[10]; // stores lengths strings present at corresponding indices in 'PLP_STR_ARRAY' 
	int str_array_size;
	int strArray_num_rows;
	int i, j;

	//code

	str_array_size = sizeof(PLP_STR_ARRAY);
	strArray_num_rows = str_array_size / sizeof(PLP_STR_ARRAY[0]);

	//storing lengths of all the strings

	for (i = 0; i < strArray_num_rows; i++)
	{
		STR_LENGTHS[i] = MyStrlen(PLP_STR_ARRAY[i]);
	}

	printf("\n\n");
	printf("The entire string array : \n\n");
	for (i = 0; i < strArray_num_rows; i++)
	{
		printf("%s ", PLP_STR_ARRAY[i]);
	}

	printf("\n\n");
	printf("Strings in the 2D array : \n\n");

	for (i = 0; i < strArray_num_rows; i++)
	{
		printf("String number %d => %s\n\n", (i + 1), PLP_STR_ARRAY[i]);
		for (j = 0; j < STR_LENGTHS[i]; j++)
		{
			printf("character %d = %c\n", (j + 1), PLP_STR_ARRAY[i][j]);
		}
		printf("\n\n");
	}

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