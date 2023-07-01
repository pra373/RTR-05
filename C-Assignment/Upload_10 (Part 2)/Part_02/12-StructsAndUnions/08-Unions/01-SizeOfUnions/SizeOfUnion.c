#include<stdio.h>

struct MyStruct
{
	int PLP_I;
	float PLP_F;
	double PLP_D;
	char PLP_C;
};

union MyUnion
{
	int PLP_I;
	float PLP_F;
	double PLP_D;
	char PLP_C;
};

int main(void)
{
	//variable declarations
	struct MyStruct s;
	union MyUnion u;

	//code

	printf("\n\n");
	printf("size of MyStruct = %zu\n", sizeof(s));
	printf("\n\n");
	printf("size of MyUnion = %zu\n", sizeof(u));
	return(0);
}