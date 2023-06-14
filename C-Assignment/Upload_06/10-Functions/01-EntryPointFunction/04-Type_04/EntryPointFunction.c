#include<stdio.h> 

//entry point function with valid return type (int) and 2 Parameter (int argc,char *argv[])

int main(int argc,char *argv[])
{
	//variable declaration
	int i;
	//code
	printf("\n\n");
	printf("Hello World !!!\n\n"); //Library Function
	printf("Number of command line arguments = %d\n\n", argc);
	printf("Command line arguments passed to this program are : \n\n");

	for (i = 0; i < argc; i++)
	{
		printf("Command line argument Number %d = %s\n", (i + 1), argv[i]);
	}
	printf("\n\n");
	return(0);
}