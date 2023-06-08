#include<stdio.h>
#include<conio.h>

int main(void)
{
	//variable declaration

	int PLP_I;
	char ch;

	//code

	printf("\n\n");

	printf("Printing Even Numbers From 1 to 100 For Every User Input. Exiting the loop Whwn user enters Character 'Q' or 'q' : \n\n");
	printf("Enter Character 'Q' or 'q' to exit Loop : \n\n");

	for (PLP_I = 1; PLP_I <= 100; PLP_I++)
	{
		

		printf("\t%d\n", PLP_I);
		ch = getch();
		if (ch == 'Q' || ch == 'q')
		{
			break;
		}
		
	}

	printf("\n\n");
	printf("Exiting Loop\n");
	printf("\n\n");

	return(0);


	
}