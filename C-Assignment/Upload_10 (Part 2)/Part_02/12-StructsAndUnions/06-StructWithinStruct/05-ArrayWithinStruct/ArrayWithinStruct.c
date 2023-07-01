#include<stdio.h>

struct MyNumber
{
	int PLP_NUM;
	int PLP_NUM_TABLE[10];

};

struct NumTables
{
	struct MyNumber a;
	struct MyNumber b;
	struct MyNumber c;

};

int main(void)
{
	//variable declarations
	struct NumTables tables;
	int i;

	//code

	//table of 2

	tables.a.PLP_NUM = 2;

	for (i = 0; i < 10; i++)
	{
		tables.a.PLP_NUM_TABLE[i] = tables.a.PLP_NUM * (i + 1);
	}
	printf("\n\n");
	printf("Table of %d : \n\n", tables.a.PLP_NUM);

	for (i = 0; i < 10; i++)
	{
		printf("%d * %d = %d\n",tables.a.PLP_NUM, (i + 1), tables.a.PLP_NUM_TABLE[i]);

	}

	//table of 3

	tables.b.PLP_NUM = 3;

	for (i = 0; i < 10; i++)
	{
		tables.b.PLP_NUM_TABLE[i] = tables.b.PLP_NUM * (i + 1);
	}
	printf("\n\n");
	printf("Table of %d : \n\n", tables.b.PLP_NUM);

	for (i = 0; i < 10; i++)
	{
		printf("%d * %d = %d\n",tables.b.PLP_NUM, (i + 1), tables.b.PLP_NUM_TABLE[i]);

	}

	//table of 4

	tables.c.PLP_NUM = 4;

	for (i = 0; i < 10; i++)
	{
		tables.c.PLP_NUM_TABLE[i] = tables.c.PLP_NUM * (i + 1);
	}
	printf("\n\n");
	printf("Table of %d : \n\n", tables.c.PLP_NUM);

	for (i = 0; i < 10; i++)
	{
		printf("%d * %d = %d\n",tables.c.PLP_NUM, (i + 1), tables.c.PLP_NUM_TABLE[i]);

	}

	return(0);


}