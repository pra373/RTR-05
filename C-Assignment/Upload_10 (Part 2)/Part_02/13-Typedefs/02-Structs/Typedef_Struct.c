#include<stdio.h>

#define MAX_NAME_LENGTH 100

struct Employee
{
	char name[MAX_NAME_LENGTH];
	unsigned int age;
	char gender;
	double salary;
};

struct MyData
{
	int PLP_I;
	float PLP_F;
	double PLP_D;
	char PLP_C;
};

int main(void)
{
	//typedefs
	typedef struct Employee MY_EMPLOYEE_TYPE;
	typedef struct MyData MY_DATA_TYPE;

	//variable declaration
	struct Employee emp = { "Funny",25,'M',10000.00 };
	MY_EMPLOYEE_TYPE emp_typedef = { "Bunny",23,'F',20400.00 };

	struct MyData md = { 30,11.45f,26.122017,'X' };
	MY_DATA_TYPE md_typedef;

	//code
	md_typedef.PLP_I = 9;
	md_typedef.PLP_F = 1.5f;
	md_typedef.PLP_D = 8.041997;
	md_typedef.PLP_C = 'P';

	printf("\n\n");
	printf("Struct Employee : \n\n");
	printf("emp.name = %s\n", emp.name);
	printf("emp.age = %d\n", emp.age);
	printf("emp.gender = %c\n", emp.gender);
	printf("emp.salary = %lf\n", emp.salary);

	printf("\n\n");
	printf("MY_EMPLOYEE_TYPE : \n\n");
	printf("emp_typedef.name = %s\n", emp_typedef.name);
	printf("emp_typedef.age = %d\n", emp_typedef.age);
	printf("emp_typedef.gender = %c\n", emp_typedef.gender);
	printf("emp_typedef.salary = %lf\n", emp_typedef.salary);

	printf("\n\n");
	printf("Struct MyData : \n\n");
	printf("md.PLP_I = %d\n",md.PLP_I);
	printf("md.PLP_F = %f\n",md.PLP_F);
	printf("md.PLP_D = %lf\n",md.PLP_D);
	printf("md.PLP_C = %c\n",md.PLP_C);

	printf("\n\n");
	printf("MY_DATA_TYPE : \n\n");
	printf("md_typedef.PLP_I = %d\n",md_typedef.PLP_I);
	printf("md_typedef.PLP_F = %f\n",md_typedef.PLP_F);
	printf("md_typedef.PLP_D = %lf\n",md_typedef.PLP_D);
	printf("md_typedef.PLP_C = %c\n",md_typedef.PLP_C);

	printf("\n\n");
	return(0);
	



}
