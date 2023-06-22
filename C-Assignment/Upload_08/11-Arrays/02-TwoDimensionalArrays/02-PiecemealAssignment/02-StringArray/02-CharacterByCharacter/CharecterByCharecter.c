#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	// A string is an array of characters ... so char[] is a char array and hence, char[] is a string.
	// array of char[] is array of strings !!!!
	//hence, char[][] is an array of char arrays hence, ia an array af strings

	/*here the string array can allow a maximum number of 10 strings(10 rows)
	and each of this 10 strings can only have 15 characters maximum (15 columns)*/

	//variable declaration
	char PLP_STR_ARRAY[5][11]; // 5 - rows , 10 colums
	int char_size;
	int str_array_size;
	int str_array_num_elements;
	int str_array_num_rows;
	int str_array_num_columns;
	int i;

	//code

	printf("\n\n");

	char_size = sizeof(char);

	str_array_size = sizeof(PLP_STR_ARRAY);

	printf("Size of two dimentional (2D) Character array (string array) is = %d \n\n", str_array_size);

	str_array_num_rows = str_array_size / sizeof(PLP_STR_ARRAY[0]);

	printf("Number of rows in two dimentional (2D) Character array (string array) is = %d \n\n", str_array_num_rows);

	str_array_num_columns = sizeof(PLP_STR_ARRAY[0]) / char_size;

	printf("Number of columns in two dimentional (2D) Character array (string array) is = %d \n\n", str_array_num_columns);

	str_array_num_elements = str_array_num_rows * str_array_num_columns;

	printf("Number of elements in two dimentional (2D) Character array (string array) is = %d \n\n", str_array_num_elements);

	// *** PIECE - MEAL ASSIGNMENT ****
	// ****** ROW 1 / STRING 1 ******

	PLP_STR_ARRAY[0][0] = 'M';
	PLP_STR_ARRAY[0][1] = 'y';
	PLP_STR_ARRAY[0][2] = '\0'; //NULL-TERMINATING CHARACTER

	// ****** ROW 2 / STRING 2 ******

	PLP_STR_ARRAY[1][0] = 'N';
	PLP_STR_ARRAY[1][1] = 'a';
	PLP_STR_ARRAY[1][2] = 'm';
	PLP_STR_ARRAY[1][3] = 'e';
	PLP_STR_ARRAY[1][4] = '\0'; //NULL-TERMINATING CHARACTER

	// ****** ROW 3 / STRING 3 ******

	PLP_STR_ARRAY[2][0] = 'I';
	PLP_STR_ARRAY[2][1] = 's';
	PLP_STR_ARRAY[2][2] = '\0'; //NULL-TERMINATING CHARACTER

	// ****** ROW 4 / STRING 4 ******

	PLP_STR_ARRAY[3][0] = 'P';
	PLP_STR_ARRAY[3][1] = 'r';
	PLP_STR_ARRAY[3][2] = 'a';
	PLP_STR_ARRAY[3][3] = 't';
	PLP_STR_ARRAY[3][4] = 'h';
	PLP_STR_ARRAY[3][5] = 'a';
	PLP_STR_ARRAY[3][6] = 'm';
	PLP_STR_ARRAY[3][7] = 'e';
	PLP_STR_ARRAY[3][8] = 's';
	PLP_STR_ARRAY[3][9] = 'h';
	PLP_STR_ARRAY[3][10] = '\0';

	// ****** ROW 5 / STRING 5 ******

	PLP_STR_ARRAY[4][0] = 'P';
	PLP_STR_ARRAY[4][1] = 'a';
	PLP_STR_ARRAY[4][2] = 'r';
	PLP_STR_ARRAY[4][3] = 'o';
	PLP_STR_ARRAY[4][4] = 'p';
	PLP_STR_ARRAY[4][5] = 'k';
	PLP_STR_ARRAY[4][6] = 'a';
	PLP_STR_ARRAY[4][7] = 'r';
	PLP_STR_ARRAY[4][8] = 'i';
	PLP_STR_ARRAY[4][9] = '\0';

	printf("\n\n");
	printf("The string in the 2D character Array are : \n\n");

	for (i = 0; i < str_array_num_rows; i++)
	{
		printf("%s ", PLP_STR_ARRAY[i]);
	}

	printf("\n\n");
	return(0);









	








}