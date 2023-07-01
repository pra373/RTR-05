#include<stdio.h>

struct MyData
{
	int PLP_I;
	float PLP_F;
	double PLP_D;
	char PLP_C;
};

int main(void)
{
	//function prototype
	struct MyData AddStructMembers(struct MyData, struct MyData, struct MyData);

	//variable declare
	struct MyData data1, data2, data3, answer_data;

	//code
	// ********* data 1 *********

	printf("\n\n\n\n"); 
	printf(" *********************** data 1 *********************\n\n");
	printf("Enter integer value for 'PLP_I' of 'struct MyData' 'data 1' : ");
	scanf("%d", &data1.PLP_I);

	printf("\n\n");
	printf("Enter float value for 'PLP_F' of 'struct MyData' 'data 1' : ");
	scanf("%f", &data1.PLP_F);

	printf("\n\n");
	printf("Enter double value for 'PLP_D' of 'struct MyData' 'data 1' : ");
	scanf("%lf", &data1.PLP_D);

	printf("\n\n");
	printf("Enter character value for 'PLP_C' of 'struct MyData' 'data 1' : ");
	data1.PLP_C = getch();
	printf("%c", data1.PLP_C);
	



	// ********* data 2 *********

	printf("\n\n\n\n");
	printf(" *********************** data 2 *********************\n\n");
	printf("Enter integer value for 'PLP_I' of 'struct MyData' 'data 2' : ");
	scanf("%d", &data2.PLP_I);

	printf("\n\n");
	printf("Enter Float value for 'PLP_F' of 'struct MyData' 'data 2' : ");
	scanf("%f", &data2.PLP_F);

	printf("\n\n");
	printf("Enter double value for 'PLP_D' of 'struct MyData' 'data 2' : ");
	scanf("%lf", &data2.PLP_D);

	printf("\n\n");
	printf("Enter character value for 'PLP_C' of 'struct MyData' 'data 2' : ");
	data2.PLP_C = getch();
	printf("%c", data2.PLP_C);

	// ********* data 3 *********

	printf("\n\n\n\n");
	printf(" *********************** data 3 *********************\n\n");
	printf("Enter integer value for 'PLP_I' of 'struct MyData' 'data 3' : ");
	scanf("%d", &data3.PLP_I);

	printf("\n\n");
	printf("Enter Float value for 'PLP_F' of 'struct MyData' 'data 3' : ");
	scanf("%f", &data3.PLP_F);

	printf("\n\n");
	printf("Enter double value for 'PLP_D' of 'struct MyData' 'data 3' : ");
	scanf("%lf", &data3.PLP_D);

	printf("\n\n");
	printf("Enter character value for 'PLP_C' of 'struct MyData' 'data 3' : ");
	data3.PLP_C = getch();
	printf("%c", data3.PLP_C);

	// calling function AddStructMembers() which accepts three variables
	// of type Struct Mydata as parameters and adds up the respective members
	// and returns the answer in anothe structure of same type


	answer_data = AddStructMembers(data1, data2, data3);

	printf("\n\n\n\n");
	printf("********** ANSWER ***********\n\n");
	printf("answer_data.PLP_I = %d\n", answer_data.PLP_I);
	printf("answer_data.PLP_F = %f\n", answer_data.PLP_F);
	printf("answer_data.PLP_D = %lf\n\n",answer_data.PLP_D);

	answer_data.PLP_C = data1.PLP_C;
	printf("answer_data.PLP_C (From data 1) = %c \n\n", answer_data.PLP_C);

	answer_data.PLP_C = data2.PLP_C;
	printf("answer_data.PLP_C (From data 2) = %c \n\n", answer_data.PLP_C);

	answer_data.PLP_C = data3.PLP_C;
	printf("answer_data.PLP_C (From data 3) = %c \n\n", answer_data.PLP_C);

	return(0);

}

struct MyData AddStructMembers(struct MyData md_one, struct MyData md_two, struct MyData md_three)
{
	//variable declaration

	struct MyData answer;

	//code
	answer.PLP_I = md_one.PLP_I + md_two.PLP_I + md_three.PLP_I;
	answer.PLP_F = md_one.PLP_F + md_two.PLP_F + md_three.PLP_F;
	answer.PLP_D = md_one.PLP_D + md_two.PLP_D + md_three.PLP_D;

	return(answer);

	
}