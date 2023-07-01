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
	// variable declaration
	union MyUnion u;
	struct MyStruct s;

	//code
	printf("\n\n");
	printf("Members of struct are : \n\n");

	s.PLP_I = 9;
	s.PLP_F = 8.7f;
	s.PLP_D = 1.233453;
	s.PLP_C = 'P';

	printf("s.PLP_I = %d\n\n", s.PLP_I);
	printf("s.PLP_F = %f\n\n", s.PLP_F);
	printf("s.PLP_D = %lf\n\n",s.PLP_D);
	printf("s.PLP_C = %c\n\n", s.PLP_C);

	printf("Address of members of struct are : \n\n");
	printf("s.PLP_I = %p\n\n", &s.PLP_I);
	printf("s.PLP_F = %p\n\n", &s.PLP_F);
	printf("s.PLP_D = %p\n\n", &s.PLP_D);
	printf("s.PLP_C = %p\n\n", &s.PLP_C);

	printf("base address of MyStruct s = %p\n\n", &s);

	printf("Members of union are : \n\n");

	u.PLP_I = 3;
	printf("u.PLP_I = %d\n\n", u.PLP_I);

	u.PLP_F = 4.5f;
	printf("u.PLP_F = %f\n\n", u.PLP_F);

	u.PLP_D = 5.127643f;
	printf("u.PLP_D = %lf\n\n", u.PLP_D);

	u.PLP_C = 'L';
	printf("u.PLP_C = %c\n\n", u.PLP_C);

	printf("Address of members of union u are : \n\n");
	printf("u.PLP_I = %p\n\n", &u.PLP_I);
	printf("u.PLP_F = %p\n\n", &u.PLP_F);
	printf("u.PLP_D = %p\n\n", &u.PLP_D);
	printf("u.PLP_C = %p\n\n", &u.PLP_C);

	printf("base address of MyUnion u = %p\n\n", &u);








}