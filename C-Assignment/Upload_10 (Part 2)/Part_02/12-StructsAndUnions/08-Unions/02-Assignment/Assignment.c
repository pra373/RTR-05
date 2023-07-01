#include<stdio.h>

union MyUnion
{
	int PLP_I;
	float PLP_F;
	double PLP_D;
	char PLP_C;
};

int main(void)
{
	//variable declaration
	union MyUnion u1, u2;

	//code
	// *** MyUnion u1 ****
	printf("\n\n");
	printf("Members of union u1 are : \n\n");

	u1.PLP_I = 6;
	u1.PLP_F = 1.2f;
	u1.PLP_D = 8.333333;
	u1.PLP_C = 'P';

	printf("u1.PLP_I = %d\n\n", u1.PLP_I);
	printf("u1.PLP_F = %f\n\n", u1.PLP_F);
	printf("u1.PLP_D = %lf\n\n", u1.PLP_D);
	printf("u1.PLP_C = %c\n\n", u1.PLP_C);

	printf("Address of members of union u1 are : \n\n");
	printf("u1.PLP_I = %p\n\n", &u1.PLP_I);
	printf("u1.PLP_F = %p\n\n", &u1.PLP_F);
	printf("u1.PLP_D = %p\n\n", &u1.PLP_D);
	printf("u1.PLP_C = %p\n\n", &u1.PLP_C);

	printf("base address of MyUnion u1 = %p\n\n", &u1);

	// ***** MyUnion u2 *****

	printf("\n\n");
	printf("Members of union u2 are : \n\n");

	u2.PLP_I = 3;
	printf("u2.PLP_I = %d\n\n", u2.PLP_I);

	u2.PLP_F = 4.5f;
	printf("u2.PLP_F = %f\n\n", u2.PLP_F);

	u2.PLP_D = 5.12764;
	printf("u2.PLP_D = %lf\n\n", u2.PLP_D);

	u2.PLP_C = 'L';
	printf("u2.PLP_C = %c\n\n", u2.PLP_C);

	printf("Address of members of union u1 are : \n\n");
	printf("u2.PLP_I = %p\n\n", &u2.PLP_I);
	printf("u2.PLP_F = %p\n\n", &u2.PLP_F);
	printf("u2.PLP_D = %p\n\n", &u2.PLP_D);
	printf("u2.PLP_C = %p\n\n", &u2.PLP_C);


	printf("base address of MyUnion u2 = %p\n\n", &u2);

	return(0);




}