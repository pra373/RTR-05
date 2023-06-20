#include<stdio.h>
#include<ctype.h> //for toupper()

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function prototype
	int MyStrlen(char[]);

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
			if (PLP_I == 0)
			{
				PLP_CARRAY_CAPITALIZE[PLP_J] = toupper(PLP_CARRAY[PLP_I]);
			}

			else if (PLP_CARRAY[PLP_I] == ' ')
			{
				PLP_CARRAY_CAPITALIZE[PLP_J] = PLP_CARRAY[PLP_I];
				PLP_CARRAY_CAPITALIZE[PLP_J + 1] = toupper(PLP_CARRAY[PLP_I + 1]);

				PLP_J++;
				PLP_I++;
			}
			else
			{
				PLP_CARRAY_CAPITALIZE[PLP_J] = PLP_CARRAY[PLP_I];
			}
			PLP_J++;

			
			
		}

		PLP_CARRAY_CAPITALIZE[PLP_J] = '\0';

		//******** string output ***********

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
