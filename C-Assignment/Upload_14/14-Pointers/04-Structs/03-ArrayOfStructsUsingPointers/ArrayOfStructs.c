#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>

#define NAME_LENGTH 100
#define MARITAL_STATUS 10

struct Employee
{
	char name[NAME_LENGTH];
	int age;
	char sex;
	float salary;
	char marital_status;
};

int main(void)
{
	//function prototype

	void MyGetString(char[], int);

	//variable declaration

	struct Employee *PTR_EMPLOYEE_RECORD = NULL;
	int num_employees, i;

	//code

	printf("\n\n");
	printf("Enter Number of employees whose details you want to record : ");
	scanf("%d", &num_employees);

	printf("\n\n");

	PTR_EMPLOYEE_RECORD = (struct Employee *)malloc(sizeof(struct Employee) * num_employees);
	
	if (PTR_EMPLOYEE_RECORD == NULL)
	{
		printf("Failed to allocate memory to %d Employees !!! Exiting NOW ... \n\n",num_employees);

		exit(0);
	}

	else
	{
		printf("Successfully allocated memory for %d Employees !!!\n\n", num_employees);
	}

	// ************** User Input initialization of array of 'struct Employee' *******

	for (i = 0; i < num_employees; i++)
	{
		printf("\n\n\n\n");
		printf("*************** Data entery for employee number %d **********\n", (i + 1));

		printf("\n\n");
		printf("Enter Employee Name : ");
		MyGetString(PTR_EMPLOYEE_RECORD[i].name, NAME_LENGTH);

		printf("\n\n\n");
		printf("Enter Employee's age (in years) : ");
		scanf("%d", &PTR_EMPLOYEE_RECORD[i].age);

		printf("\n\n");
		printf("Enter Employee's sex (M/m for Male, F/f for Female) : ");
		PTR_EMPLOYEE_RECORD[i].sex = getch();
		printf("%c", PTR_EMPLOYEE_RECORD[i].sex);
		PTR_EMPLOYEE_RECORD[i].sex = toupper(PTR_EMPLOYEE_RECORD[i].sex);

		printf("\n\n\n");
		printf("Enter Employee's salary (in Indian Rupees) : ");
		scanf("%f", &PTR_EMPLOYEE_RECORD[i].salary);

		printf("\n\n");
		printf("Is the employee married ? (Y/y for yes, N/n for no) : ");
		PTR_EMPLOYEE_RECORD[i].marital_status = getch();
		printf("%c", PTR_EMPLOYEE_RECORD[i].marital_status);

		PTR_EMPLOYEE_RECORD[i].marital_status = toupper(PTR_EMPLOYEE_RECORD[i].marital_status);





	}

	//******* Display ******

	printf("\n\n\n\n");
	printf("*************** Display Employee Records ************\n\n");
	for (i = 0; i < num_employees; i++)
	{
		printf("***************** Employee Number %d **************\n\n", (i + 1));
		printf("Name               :%s\n", PTR_EMPLOYEE_RECORD[i].name);
		printf("Age                :%d years \n", PTR_EMPLOYEE_RECORD[i].age);

		if (PTR_EMPLOYEE_RECORD[i].sex == 'M' || PTR_EMPLOYEE_RECORD[i].sex == 'm')
		{
			printf("Sex               :Male\n");
		}
		else if (PTR_EMPLOYEE_RECORD[i].sex == 'F' || PTR_EMPLOYEE_RECORD[i].sex == 'f')
		{
			printf("Sex               :Female\n");

		}

		else
		{
			printf("Sex            :Invalid Data Entered\n");
		}

		printf("Salary             :Rs. %f\n", PTR_EMPLOYEE_RECORD[i].salary);

		if (PTR_EMPLOYEE_RECORD[i].marital_status == 'Y' || PTR_EMPLOYEE_RECORD[i].marital_status == 'y')
		{
			printf("Marital Status   : Married\n");
		}

		else if (PTR_EMPLOYEE_RECORD[i].marital_status == 'N' || PTR_EMPLOYEE_RECORD[i].marital_status == 'n')
		{
			printf("Marital Status   :Unmarried\n");
		}

		else
		{
			printf("Marutal Status :Invalid Data Entered\n");
		}

		printf("\n\n");

		



	}

	if (PTR_EMPLOYEE_RECORD)
	{
		free(PTR_EMPLOYEE_RECORD);
		PTR_EMPLOYEE_RECORD = NULL;
		printf("Memory Allocated to %d employees has been successfully freed \n\n", num_employees);
	}

	return(0);
}

//  **** simple rudementry implementation of gets_s() *****
// ***** Implemented due to different behavior of gets_s() / fgets(0 / fscanf() on different platform 
// *** BACKSPACE / Character eletion and key cursor movement not implemented

void MyGetString(char str[], int str_size)
{
	//variable declaration

	int i;
	char ch = '\0';

	//code
	i = 0;
	do
	{
		ch = getch();
		str[i] = ch;
		printf("%c", str[i]);
		i++;
	} while ((ch != '\r') && (i < str_size));

	if (i == str_size)
	{
		str[i - 1] = '\0';
	}
	else
	{
		str[i] = '\0';
	}
}



