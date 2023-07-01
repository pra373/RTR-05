#include<stdio.h>

struct MyNumber
{
	int PLP_NUM;
	int PLP_NUM_TABLE[10];

};

struct NumTables
{
	struct MyNumber n;
};

int main(void)
{
	// variable declarations
	struct NumTables tables[10]; //array od 10 structure variables
	int i, j;

	//code
	for (i = 0; i < 10; i++)
	{
		tables[i].n.PLP_NUM = (i + 1);
	}

	for (i = 0; i < 10; i++)
	{
		printf("\n\n");
		printf("Table of %d : \n\n", tables[i].n.PLP_NUM);

		for (j = 0; j < 10; j++)
		{
			tables[i].n.PLP_NUM_TABLE[j] = tables[i].n.PLP_NUM * (j + 1);
			printf("%d * %d = %d\n", tables[i].n.PLP_NUM, (j + 1), tables[i].n.PLP_NUM_TABLE[j]);


		}
	}

	return(0);

}

