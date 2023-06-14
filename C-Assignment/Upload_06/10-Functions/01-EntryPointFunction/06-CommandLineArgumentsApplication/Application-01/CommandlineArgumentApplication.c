#include<stdio.h>   //contains declaration of printf()
#include<ctype.h>   //contains declaration of atoi()
#include<stdlib.h>  //contains declaration of exit()


/*This program adds all command line arguments given in Integer form only
 and outputs the sum*/

/**** Due to  use of atoi(), All command line arguments of type other than 'int'are ignored ***/



int main(int argc, char* argv[], char *envp[])
{
	//Variable declaration

	int PLP_I, PLP_NUM, PLP_SUM = 0;

	//code
	if (argc == 1)
	{
		printf("\n\n");
		printf("No numbers given for addition !!! Exitting now ...\n\n");
		printf("Usage : CommandLine Argument Application <first number> <second number>...\n\n");
		exit(0);
	}

	printf("\n\n");
	printf("Sum of all command line arguments is : \n\n");

	for (PLP_I = 1; PLP_I < argc; PLP_I++)
	{
		PLP_NUM = atoi(argv[PLP_I]);
		PLP_SUM = PLP_SUM + PLP_NUM;
	}

	printf("PLP_SUM = %d\n\n ", PLP_SUM);
	return(0);








}