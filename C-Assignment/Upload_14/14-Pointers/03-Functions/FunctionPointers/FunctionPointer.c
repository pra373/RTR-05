#include<stdio.h>

int main(void)
{
	//function declarations
	int AddIntegers(int, int);
	int SubtractIntegers(int, int);
	float AddFloats(float, float);

	//variable declarations

	typedef int (*AddIntsFnPtr)(int, int);
	AddIntsFnPtr ptrAddTwoIntegers = NULL;
	AddIntsFnPtr ptrFunc = NULL;

	typedef float(*AddFloatsFnPtr)(float, float);
	AddFloatsFnPtr ptrAddTwoFLoats = NULL;

	int iAnswer = 0;
	float fAnswer = 0.0f;

	//code
	ptrAddTwoIntegers = AddIntegers;
	iAnswer = ptrAddTwoIntegers(9, 30);
	printf("\n\n");
	printf("Sum of Integers = %d\n\n", iAnswer);

	ptrFunc = SubtractIntegers;
	iAnswer = ptrFunc(9, 30);
	printf("\n\n");
	printf("Substraction of Integers = %d\n\n", iAnswer);

	ptrAddTwoFLoats = AddFloats;
	fAnswer = ptrAddTwoFLoats(11.45f, 8.2f);
	printf("\n\n");
	printf("Sum of floating point numbers = %f\n\n", fAnswer);

	return(0);






}

int AddIntegers(int PLP_A, int PLP_B)
{
	int c;

	c = PLP_A + PLP_B;
	return(c);
}

int SubtractIntegers(int PLP_A, int PLP_B)
{
	int c;

	c = PLP_A - PLP_B;

	return(c);
}

float AddFloats(float f_num1, float f_num2)
{
	float ans;

	ans = f_num1 + f_num2;

	return(ans);
}