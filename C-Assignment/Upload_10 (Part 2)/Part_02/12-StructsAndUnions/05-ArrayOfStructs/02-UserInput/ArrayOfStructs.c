#include<stdio.h>
#include<ctype.h>

#define NAME_LENGTH 100


#define NUM_EMPLOYEES 5 //Simply changethis value to have as many number of employees as you like


	struct Employee
	{
		char EMP_NAME[NAME_LENGTH];
		int EMP_AGE;
		float EMP_SALARY;
		char EMP_SEX;
		char EMP_MARITAL_STATUS;
	};

	int main(void)
	{
		//function prototype

		void MyGetString(char[], int);

		//variable declaration

		struct Employee EmployeeRecord[NUM_EMPLOYEES];
		int i;

		//code
		//user Input Initialization of array of 'Struct Employee'

		for (i = 0; i < NUM_EMPLOYEES; i++)
		{
			printf("\n\n\n\n");
			printf("********** DATA ENTRY FOR EMPLOYEE NUMBER %d **********",i+1);
				printf("\n\n");
				printf("Enter Employee Name : ");
				MyGetString(EmployeeRecord[i].EMP_NAME, NAME_LENGTH);
				printf("\n\n\n");

				printf("Enter Employee's Age (in years): ");

				scanf("%d", &EmployeeRecord[i].EMP_AGE);

				printf("\n\n");

				printf("Enter Employee's Sex (M/m For Male, F/f For Female) : ");

				EmployeeRecord[i].EMP_SEX = getch();

				printf("%c",EmployeeRecord[i].EMP_SEX);

				EmployeeRecord[i].EMP_SEX = toupper(EmployeeRecord[i].EMP_SEX);

				printf("\n\n\n");
				printf("Enter Employee's Salary (in Indian Rupees): ");

				scanf("%f", &EmployeeRecord[i].EMP_SALARY);
				printf("\n\n");

				
				printf("Is The Employee Married? (Y/y For Yes, N/n For No): ");
				EmployeeRecord[i].EMP_MARITAL_STATUS = getch();
				printf("%c", EmployeeRecord[i].EMP_MARITAL_STATUS); 
				EmployeeRecord[i].EMP_MARITAL_STATUS = toupper(EmployeeRecord[i].EMP_MARITAL_STATUS);

		}

		// Display

		printf("\n\n\n\n");
		printf("******** DISPLAYING EMPLOYEE RECORDS************\n\n");
		for (i = 0; i < NUM_EMPLOYEES; i++) 
		{
			printf("****** **** EMPLOYEE NUMBER %d *************\n\n",(i+1));
			printf("Name               :%s\n", EmployeeRecord[i].EMP_NAME);
			printf("Age                :%d Years\n", EmployeeRecord[i].EMP_AGE);

			
				
			if (EmployeeRecord[i].EMP_SEX == 'M')
				printf("Sex            :Male\n");
					
			else if (EmployeeRecord[i].EMP_SEX == 'F')
				printf("Sex            :Female\n");
						
			else
				printf("Sex            :Invalid Data Entered\n");

			    printf("Salary         :Rs. %f \n", EmployeeRecord[i].EMP_SALARY);

			if (EmployeeRecord[i].EMP_MARITAL_STATUS == 'Y')
				printf("Marital Status :Married\n");
				

			else if (EmployeeRecord[i].EMP_MARITAL_STATUS == 'N')
				printf("Marital Status :Unmarried\n");

			else
				printf("Marital Status :Invalid Data Entered\n");

			printf("\n\n");
				

					
							
								
		}
		return(0);
	}

	// *** SIMPLE RUDIMENTARY IMPLEMENTATION OF gets_s() ***
	// *** IMPLEMENTED DUE TO DIFFERENT BEHAVIOUR OF gets_s() / fgets() / fscanf() ON DIFFERENT PLATFORMS***
	      
	// *** BACKSPACE/CHARACTER DELETION AND ARROW KEY CURSOR MOVEMENT NOT IMPLEMENTED ***

	void MyGetString(char str[], int str_size)
	{
		//variable declarations
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
			str[i - 1] = '\0';
		else
			str[i] = '\0';
	}


