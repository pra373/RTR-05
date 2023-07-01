#include<stdio.h>
#include<string.h>

#define NAME_LENGTH 100
#define MARITAL_STATUS 10

struct Employee
{
	char EMP_NAME[NAME_LENGTH];
	int EMP_AGE;
	float EMP_SALARY;
	char EMP_SEX;
	char EMP_MARITAL_STATUS[MARITAL_STATUS];
};

int main(void)
{
	//variable declarations
	struct Employee EmployeeRecord[5]; // an array of 5 structs each of type struct Employee

	char employee_rajesh[] = "Rajesh";
	char employee_prathamesh[] = "Prathamesh";
	char employee_Kalyani[] = "Kalyani";
	char employee_sonali[] = "Sonali";
	char employee_shantanu[] = "Shantanu";

	int i;

	// Hard-coaded initialization of array of struct employee 

	//employee - 01

	strcpy(EmployeeRecord[0].EMP_NAME, employee_rajesh);
	EmployeeRecord[0].EMP_AGE = 30;
	EmployeeRecord[0].EMP_SEX = 'M';
	EmployeeRecord[0].EMP_SALARY = 50000.0f;
	strcpy(EmployeeRecord[0].EMP_MARITAL_STATUS, "Unmarried");

	//employee - 02

	strcpy(EmployeeRecord[1].EMP_NAME, employee_prathamesh);
	EmployeeRecord[1].EMP_AGE = 23;
	EmployeeRecord[1].EMP_SEX = 'M';
	EmployeeRecord[1].EMP_SALARY = 1000000.0f;
	strcpy(EmployeeRecord[1].EMP_MARITAL_STATUS, "Unmarried");

	// employee - 03

	strcpy(EmployeeRecord[2].EMP_NAME, employee_Kalyani);
	EmployeeRecord[2].EMP_AGE = 29;
	EmployeeRecord[2].EMP_SEX = 'F';
	EmployeeRecord[2].EMP_SALARY = 60000.0f;
	strcpy(EmployeeRecord[2].EMP_MARITAL_STATUS, "Unmarried");

	// employee - 04

	strcpy(EmployeeRecord[3].EMP_NAME, employee_sonali);
	EmployeeRecord[3].EMP_AGE = 35;
	EmployeeRecord[3].EMP_SEX = 'F';
	EmployeeRecord[3].EMP_SALARY = 50000.0f;
	strcpy(EmployeeRecord[3].EMP_MARITAL_STATUS, "Married");

	// employee - 05

	strcpy(EmployeeRecord[4].EMP_NAME, employee_shantanu);
	EmployeeRecord[4].EMP_AGE = 35;
	EmployeeRecord[4].EMP_SEX = 'M';
	EmployeeRecord[4].EMP_SALARY = 55000.0f;
	strcpy(EmployeeRecord[4].EMP_MARITAL_STATUS, "Married");

	// Diaplay structure members

	printf("\n\n");
	printf("************* Displaying Employee Records*************\n\n");
	for (i = 0; i < 5; i++)
	{
		    printf("******** Employee Number %d**********\n\n",(i+1));
		    printf("Name           : %s\n", EmployeeRecord[i].EMP_NAME);
		    printf("Age            : %d\n", EmployeeRecord[i].EMP_AGE);

		if (EmployeeRecord[i].EMP_SEX == 'M' || EmployeeRecord[i].EMP_SEX == 'm')
		{
			printf("Sex            : Male\n");
		}
		else
		{
			printf("Sex            : Female\n");

		}

		    printf("Salary         : %f\n", EmployeeRecord[i].EMP_SALARY);
		    printf("Marital Status : %s\n", EmployeeRecord[i].EMP_MARITAL_STATUS);
		    printf("\n\n");
	}
	return(0);










}