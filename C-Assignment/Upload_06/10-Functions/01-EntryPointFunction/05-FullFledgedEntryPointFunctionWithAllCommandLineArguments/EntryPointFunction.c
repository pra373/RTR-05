#include<stdio.h>

//Entry point function with valid retuen type (int) and 3 parameters (int argc,char *argv[],char *envp[])

int main(int argc,char *argv[],char *envp[])
{
	//variable declaration
	int i;

	//code

	printf("\n\n");
	printf("Hello World !!!\n\n"); //library function

	printf("Number of command Line arguments = %d\n\n", argc);
	printf("command line arguments passed to this programe are : \n\n");

	for (i = 0; i < argc; i++)
	{
		printf("Command Line argument Number %d = %s\n", (i + 1), argv[i]);

	}

	printf("\n\n");

	printf("First 20 Environmental Variables Passed to this programe are : \n\n");
	
	for (i = 0; i < 20; i++)
	{
		printf("Environmental Variable Number %d = %s\n", (i + 1), envp[i]);
	}

	printf("\n\n");

	return(0);


}