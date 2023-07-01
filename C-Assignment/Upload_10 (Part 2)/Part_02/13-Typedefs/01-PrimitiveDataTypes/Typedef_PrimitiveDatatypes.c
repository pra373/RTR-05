#include<stdio.h>

//global typedef

typedef int MY_INT;   // "type" int has been redefined as MY_INT .... Now,"MY_INT" can br treated just like int
int main(void)
{
	//function prototype
	MY_INT add(MY_INT, MY_INT);

	//typedefs

	typedef int MY_INT;  // "type" int has been redefined as MY_INT .... Now,"MY_INT" can be treated just like int

	typedef float PVG_FLOAT; // "type" Float has been redefined as PVG_FLOAT.... Now,"PVG_FLOAT" can be treated just like float


	typedef char CHARACTER;   // "type" char has been redefined as CHARACTER.... Now,"CHARACTER" can be treated just like char
	typedef double MY_DOUBLE;// "type" double has been redefined as MY_DOUBLE.... Now,"MY_DOUBLE" can be treated just like double

	// ******** JUST like in WIN32SDK *********
	typedef unsigned int UINT;
	typedef UINT HANDLE;
	typedef HANDLE HWND;
	typedef HANDLE HINSTANCE;

	//variable declaration

	MY_INT PLP_A = 10, PLP_I;
	MY_INT PLP_IARRAY[] = { 9,18,27,36,45,54,63,72,81,90 };

	PVG_FLOAT f_pvg = 30.9;
	const PVG_FLOAT f_pvg_pi = 3.142f;

	CHARACTER ch = '*';
	CHARACTER PLP_CARRAY_01[] = "Hello";
	CHARACTER PLP_CARRAY_02[][10] = { "RTR","Batch","2023-2024" };
	
	MY_DOUBLE d = 8.041997;

	//Just random numbers 

	UINT uint = 3456;
	HANDLE handle = 987;
	HWND hwnd = 9876;
	HINSTANCE hInstance = 14467;

	//code

	printf("\n\n");
	printf("Type MY_INT variable PLP_A = %d\n", PLP_A);
	printf("\n\n");

	for (PLP_I = 0; PLP_I < (sizeof(PLP_IARRAY) / sizeof(MY_INT)); PLP_I++)
	{
		printf("Type MY_INT array variable PLP_IARRAY[%d] = %d\n", PLP_I, PLP_IARRAY[PLP_I]);
	}

	printf("\n\n");
	printf("\n\n");
	printf("Type PVG_FLOAT variable f_pvg = %f\n", f_pvg);
	printf("Type constant PVG_FLOAT variable f_pvg_pi = %f\n", f_pvg_pi);
	printf("\n\n");
	printf("Type MY_DOUBLE variable d = %lf\n", d);
	printf("\n\n");
	printf("Type CHARACTER variable ch = %c\n", ch);
	printf("\n\n");
	printf("Type CHARACTER array variable PLP_CARRAY_01 = %s\n", PLP_CARRAY_01);
	printf("\n\n");

	for (PLP_I = 0; PLP_I < (sizeof(PLP_CARRAY_02) / sizeof(PLP_CARRAY_02[0])); PLP_I++)
	{
		printf("%s\t", PLP_CARRAY_02[PLP_I]);
	}
	printf("\n\n");
	printf("\n\n");

	printf("Type UINT variable uint = %u\n\n", uint);
	printf("Type HANDLE variable handle = %u\n\n", handle);
	printf("Type HWND variable hwnd = %u\n\n", hwnd);
	printf("Type HINSTANCE variable hInstance = %u\n\n", hInstance);
	printf("\n\n");

	MY_INT x = 90;
	MY_INT y = 30;
	MY_INT ret;

	ret = add(x, y);
	printf("ret = %d\n\n", ret);
	return(0);

}

MY_INT add(MY_INT a, MY_INT b)
{
	MY_INT c;
	c = a + b;
	return(c);
}


