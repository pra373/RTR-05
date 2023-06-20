#include<stdio.h>

#define MAX_STRING_LENGTH 512
#define SPACE ' '
#define FULLSTOP '.'
#define EXCLAMATION '!'
#define COMMA ','
#define QUESTION_MARK '?'

int main(void)
{
	// function prototype
		int MyStrlen(char[]);
		char MyToUpper(char);

	// variable declaration
		char PLP_CARRAY[MAX_STRING_LENGTH], PLP_CARRAY_CAPITALIZE[MAX_STRING_LENGTH];
		int PLP_STRING_LENGTH, PLP_I = 0, PLP_J = 0;

		//code

		//**** String input *****

		printf("\n\n");
		printf("Enter a String : \n\n");
		gets_s(PLP_CARRAY, MAX_STRING_LENGTH);

		PLP_STRING_LENGTH = MyStrlen(PLP_CARRAY);

		for (PLP_I = 0; PLP_I < PLP_STRING_LENGTH; PLP_I++)
		{
			if (PLP_I == 0) //first letter of any sentence must be a capital letter
			{
				PLP_CARRAY_CAPITALIZE[PLP_J] = MyToUpper(PLP_CARRAY[PLP_I]);
			}

			//Words are seperated by spaces
			else if (PLP_CARRAY[PLP_I]==SPACE)
			{
				PLP_CARRAY_CAPITALIZE[PLP_J] = PLP_CARRAY[PLP_I];
				PLP_CARRAY_CAPITALIZE[PLP_J + 1] = MyToUpper(PLP_CARRAY[PLP_I + 1]);
				PLP_J++;
				PLP_I++;
			}

			else if ((PLP_CARRAY[PLP_I] == FULLSTOP || PLP_CARRAY[PLP_I] == COMMA || PLP_CARRAY[PLP_I] == EXCLAMATION || PLP_CARRAY[PLP_I] == QUESTION_MARK) && (PLP_CARRAY[PLP_I] != SPACE))
			{
				PLP_CARRAY_CAPITALIZE[PLP_J] = PLP_CARRAY[PLP_I];
				PLP_CARRAY_CAPITALIZE[PLP_J + 1] = SPACE;
				PLP_CARRAY_CAPITALIZE[PLP_J + 2] = MyToUpper(PLP_CARRAY[PLP_I + 1]);
				PLP_J = PLP_J + 2;
				PLP_I++;

			}
			else
			{
				PLP_CARRAY_CAPITALIZE[PLP_J] = PLP_CARRAY[PLP_I];
			}
			PLP_J++;



			
			
		}

		PLP_CARRAY_CAPITALIZE[PLP_J] = '\0';

		printf("\n\n");
		printf("The string entered by you Is : \n\n");
		printf("%s\n", PLP_CARRAY);

		printf("\n\n");
		printf("The string after capitalizing first letter of every word Is : \n\n");
		printf("%s\n", PLP_CARRAY_CAPITALIZE);

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

char MyToUpper(char ch)
{
	int num;
	int c;

	num = 'a' - 'A';

	if ((int)ch >= 97 && (int)ch <= 122)
	{
		c = (int)ch - num;
		return((char)c);
	}
	else
	{
		return(ch);
	}

}