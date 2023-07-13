#include<stdio.h>
#include<string.h> // for toupper()

int main(void)
{
	//variable declaration
	char PLP_CH, PLP_CH_I;
	unsigned int PLP_ASCII_CH = 0;

	//code

	printf("\n\n");
	printf("Enter first characte of your first name : ");
	PLP_CH = getch();
	
	PLP_CH = toupper(PLP_CH);

	for (PLP_CH_I = 'A'; PLP_CH_I <= 'z'; PLP_CH_I++)
	{
		if (PLP_CH == PLP_CH_I)
		{
			PLP_ASCII_CH = (unsigned int)PLP_CH;
			goto result_output; // progrom flow jumps directly to label "result_output"
		}
	}

	printf("\n\n");
	printf("GOTO statement not executed , so printing \"Hello Wolrld !!!\"\n");  //This statement will be omited if goto statement is exicuted

	result_output:  // label itself does not alter flow of program. following code is exicuted regardless of weather goto statement is executed or not.
	printf("\n\n");

	if (PLP_ASCII_CH == 0)
	{
		printf("You mst have a strange name ! Could not find character '%c' in the entire english alphabet!\n",PLP_CH);
	}

	else
	{
		printf("Character '%c' found. It has ASCII value %u.\n", PLP_CH, PLP_ASCII_CH);
	}

	printf("\n\n");

	return(0);



}