#include<stdio.h>

	int main(void)
	{
		//variable declarations
		char option, ch = '\0';

		//code

		printf("\n\n");
		printf("Once the Infinite loop begins, Enter 'Q' or 'q' To quit the infinite FOR loop : \n\n");
		printf("Enter 'Y' or 'y' to initiate User Controlled Infinite Loop : ");
		printf("\n\n");
		option = getch();
		if (option == 'Y' || option == 'y')
		{
			for (;;) //Infinte Loop
			{
				printf("In Loop... \n");
				ch = getch();
				if (ch == 'Q' || ch == 'q')
				{
					break;
				}
					
			}
		}

		printf("\n\n");
		printf("Exiting User Controlled Infinite Loop");
		printf("\n\n");

		return(0);




		
	}

