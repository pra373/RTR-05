#include<stdio.h> //Contains declaration of printf(0)
#include<stdlib.h> //Contains declaration of exit(0)

int main(int argc, char* argv[], char *envp[])
{
	//variable declaration
	int PLP_I;

	//code
	if (argc != 4)  //Programe name + first name +Middle name + Last name = 4 command line arguments are required
	{
		printf("\n\n");
		printf("Invalid Usage !!! Exitting Now ... \n\n");
		printf("Usage : Command line argument application <First name> <middle name> <Last name> \n\n");
		exit(0);
	}

	/* This programe prints your full name as entered in the command line arguments */

	printf("\n\n");
	printf("Your Full name is : ");

	for (PLP_I = 1; PLP_I < argc; PLP_I++)
	{
		printf("%s ", argv[PLP_I]);
	}

	printf("\n\n");

	return(0);



}