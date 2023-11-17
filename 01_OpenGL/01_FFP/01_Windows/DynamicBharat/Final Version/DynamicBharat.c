//Windows header files

#include<windows.h>            //For declaration of 3.5 lakh API
#include<stdio.h>              // for file I/O
#include<stdlib.h>             // for exit()

#include "OGL.h"            //Header file in same directory

//OpenGL Header Files
#include<gl/GL.h>
#include<gl/GLU.h>

//Link with OpenGL Library

#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"winmm.lib") // for PlaySound API

//Macros

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define _USE_MATH_DEFINES
#include<math.h>

// Global Function declarations

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations

FILE *gpFile = NULL;           // (gpFile) global pointer to a file

HWND ghwnd = NULL;             // handle of window
BOOL gbActive = FALSE;
DWORD dwStyle = 0;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) }; 
BOOL gbFullScreen = FALSE;

//OpenGL Related Global variables

HDC ghdc = NULL;
HGLRC ghrc = NULL;

//Letter translation variables

GLfloat Move_Letter_B = -8.0f;
GLfloat Move_Letter_H_X = -8.675f;
GLfloat Move_Letter_H_Y = 7.0f;
GLfloat Move_Letter_A_X = -8.44f;
GLfloat Move_Letter_A_Y = -9.0f;
GLfloat Move_Letter_R_X = 11.1f;
GLfloat Move_Letter_R_Y = 9.0f;
GLfloat Move_Letter_A2_X = 14.1f;
GLfloat Move_Letter_A2_Y = -9.0f;
GLfloat Move_Letter_T_X = 13.5f;

//Plane translation variables

GLfloat Move_Middle_Plane_X = -15.0f;

GLfloat Move_Upper_Plane_X = -4.5f;
GLfloat Move_Upper_Plane_Y = 0.4f;

GLfloat Move_Lower_Plane_X = -4.5f;
GLfloat Move_Lower_Plane_Y = -0.4f;

//Shading variables

//letter T variables

GLfloat Red_T_Orange = 128.0f;
GLfloat Green_T_Orange = 128.0f;
GLfloat Blue_T_Orange = 128.0f;

GLfloat Red_T_White = 128.0f;
GLfloat Green_T_White = 128.0f;
GLfloat Blue_T_White = 128.0f;

GLfloat Red_T_Green = 128.0f;
GLfloat Green_T_Green = 128.0f;
GLfloat Blue_T_Green = 128.0f;

//letter A3 second variables

GLfloat Red_A3_Second_Orange = 128.0f;
GLfloat Green_A3_Second_Orange = 128.0f;
GLfloat Blue_A3_Second_Orange = 128.0f;

GLfloat Red_A3_Second_White = 128.0f;
GLfloat Green_A3_Second_White = 128.0f;
GLfloat Blue_A3_Second_White = 128.0f;

GLfloat Red_A3_Second_Green = 128.0f;
GLfloat Green_A3_Second_Green = 128.0f;
GLfloat Blue_A3_Second_Green = 128.0f;

//letter A3 first variable

GLfloat Red_A3_First_Orange = 128.0f;
GLfloat Green_A3_First_Orange = 128.0f;
GLfloat Blue_A3_First_Orange = 128.0f;

GLfloat Red_A3_First_White = 128.0f;
GLfloat Green_A3_First_White = 128.0f;
GLfloat Blue_A3_First_White = 128.0f;

GLfloat Red_A3_First_Green = 128.0f;
GLfloat Green_A3_First_Green = 128.0f;
GLfloat Blue_A3_First_Green = 128.0f;


//letter R variables

GLfloat Red_R_Orange = 128.0f;
GLfloat Green_R_Orange = 128.0f;
GLfloat Blue_R_Orange = 128.0f;

GLfloat Red_R_White = 128.0f;
GLfloat Green_R_White = 128.0f;
GLfloat Blue_R_White = 128.0f;

GLfloat Red_R_Green = 128.0f;
GLfloat Green_R_Green = 128.0f;
GLfloat Blue_R_Green = 128.0f;

//letter H variables

GLfloat Red_H_Orange = 128.0f;
GLfloat Green_H_Orange = 128.0f;
GLfloat Blue_H_Orange = 128.0f;

GLfloat Red_H_White = 128.0f;
GLfloat Green_H_White = 128.0f;
GLfloat Blue_H_White = 128.0f;

GLfloat Red_H_Green = 128.0f;
GLfloat Green_H_Green = 128.0f;
GLfloat Blue_H_Green = 128.0f;

//Letter B Shading variables

GLfloat Red_B_Orange = 128.0f;
GLfloat Green_B_Orange = 128.0f;
GLfloat Blue_B_Orange = 128.0f;

GLfloat Red_B_White = 128.0f;
GLfloat Green_B_White = 128.0f;
GLfloat Blue_B_White = 128.0f;

GLfloat Red_B_Green = 128.0f;
GLfloat Green_B_Green = 128.0f;
GLfloat Blue_B_Green = 128.0f;

//plane rotation variables

GLfloat Rotate_Plane_Two = 0.0f;
GLfloat Rotate_Plane_Three = 0.0f;
//Entry point function

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{

	// Local Function Declarations

	int initialize(void);
	void uninitialize(void);
	void display(void);
	void update(void);

	
	// Local variable declaration

	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("PLP Window");
	int iResult = 0;      
	BOOL bDone = FALSE;   // To check game loop while condition


	int ScreenWidth;
	int ScreenHeight;
	int x_screen_center;
	int y_screen_center;

	//code

	ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	x_screen_center = ScreenWidth / 2;
	y_screen_center = ScreenHeight / 2;

	gpFile = fopen("Log.txt", "w");
	if (gpFile == NULL)
	{
		MessageBox(NULL, TEXT("Log file cannot be opened"), TEXT("ERROR"), MB_OK);
		exit(0);
	}

	fprintf(gpFile, "Program startrd successfully\n");
	
	// WNDCLASSEX Initialization

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  // use CS_OWNDC to stop purging from OS
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbWndExtra = 0;
	wndclass.cbClsExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	RegisterClassEx(&wndclass);

	//create window

	hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		szAppName,
		TEXT("Prathamesh Laxmikant Paropkari"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		(x_screen_center - 400),
		(y_screen_center - 300),
			WIN_WIDTH,
			WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ghwnd = hwnd;

	//initialization

	iResult = initialize();

	if (iResult != 0)
	{
		MessageBox(hwnd, TEXT("Initialization() failed"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		
		DestroyWindow(hwnd);
	}


	// Show the window

	ShowWindow(hwnd, iCmdShow);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	

	

	// Game loop

	PlaySound(TEXT("Sound2.wav"), NULL, SND_FILENAME | SND_ASYNC);

	while (bDone == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				bDone = TRUE;
			}

			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		else
		{
			if (gbActive == TRUE)
			{
				//Render

				display();

				//update

				update();

			}
		}
	}

	//Uninitialization

	uninitialize();

	

	return((int)msg.wParam);

}

// call back function

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	
	//Function declaration

	void ToggleFullScreen(void);
	void resize(int, int);


	// code

	switch (iMsg)
	{
	case WM_DESTROY:

		
		PostQuitMessage(0);
		break;

	case WM_SETFOCUS:
		gbActive = TRUE;
		break;

	case WM_KILLFOCUS:
		gbActive = FALSE;
		break;

	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));   //lParam of WM_SIZE will give me WIDTH and HEIGHT
		break;

	case WM_ERASEBKGND:
		return(0);
		

	case WM_KEYDOWN:                 // use for non character keys
		switch (LOWORD(wParam))
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;

		}
		break;

	case WM_CHAR:                    // use for character keys
		switch (LOWORD(wParam))
		{
		case 'F':
		case 'f':
			if (gbFullScreen == FALSE)
			{
				ToggleFullScreen();
				gbFullScreen = TRUE;
			}
			else
			{
				ToggleFullScreen();
				gbFullScreen = FALSE;

			}

			break;




		}

		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;




	default:
		break;

		
		
	}

	return(DefWindowProc(hwnd, iMsg, wParam,lParam));  // Default Window procedure for all messages.
	

	
}

void ToggleFullScreen(void)
{
	// Local variable declarations 

	MONITORINFO mi = { sizeof(MONITORINFO) };


	//code

	if (gbFullScreen == FALSE)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);

		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}

		ShowCursor(FALSE);
	}

	else
	{
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);

	}







}

int initialize(void)
{
	//funcion declarations
	void resize(int, int);


	//code

	//variable declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;

	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));  // function to initialize all structure members to zero

	//****** initialization of PIXELFORMATDESCRIPTOR ************

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;

	//  Get the device context cause we need that as a first parameter of function ChoosePixelFormat()

	ghdc = GetDC(ghwnd);

	if (ghdc == NULL)
	{
		fprintf(gpFile, "GetDC() failed\n");
		return(-1);
	}

	// Tell OS to give the closest PIXELFORMATDESCRIPTOR in her list of PIXELFORMATDESCRIPTORS by compairing with my PIXELFORMATDESCRIPTOR structure

	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);  // successful return value is non zero positive value  

	if (iPixelFormatIndex == 0)
	{
		fprintf(gpFile, "ChoosePixelFormat() failed\n");
		return(-2);

	}

	//Set obtained Pixel format

	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		fprintf(gpFile, "SetPixelFormat() Failed\n");
		return(-3);

	}


	// WGL act as a bridging API between Windows and OpenGL.
	// Tell WGL(Windows graphic library)  to give me OpenGL Compatible DC from this ghdc.
	// Tell WGL bridging library to give OpenGL Device Context from this device context.

	// Create Opengl context from Device context.

	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
	{
		fprintf(gpFile, " wglCreateContext() Failed\n");
		return(-4);
	}

	// Now GHDC will end its role and will give control to ghrc to do further drawing

	// make rendering context current

	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		fprintf(gpFile, "wglMakeCurrent() Failed\n");
		return(-5);
	}

	// set the clear color of window to blue

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // First OpenGL Function After bridging APIS
	resize(WIN_WIDTH, WIN_HEIGHT);

	return(0);
}

void resize(int width, int height)
{
	//code

	if (height <= 0)
	{
		height = 1;
	}

	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

}

void display(void)
{
	//local function declarations
	void Letter_B(void);
	void Letter_H(void);
	void Letter_A1(void);
	void Letter_A2(void);
	void Letter_A3_First(void);
	void Letter_A3_Second(void);
	void Letter_T(void);
	void Letter_R(void);
	void Plane1(void);
	void Plane2(void);
	void Plane3(void);
	void Parabola(void);

	//coloured letter functions
	 

	 
	//code
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glTranslatef(-0.39f, 0.0f, -3.0f);
	
	
	Letter_B();
	Letter_H();
	Letter_A1();
	//Letter_A3_First();
	//Letter_A3_Second();
	Letter_T();
	Letter_R();
	Letter_A2();
	Plane1();
	Plane2();
	Plane3();
	//Parabola();
	
	//Letter_A3();
	SwapBuffers(ghdc);


}

void update(void)
{
	//code
	

	if (Move_Letter_B <= -3.9f)
	{
		Move_Letter_B = Move_Letter_B + 0.00085f;
	}

	if (Move_Letter_B >= -3.9f && Move_Letter_H_Y >= -0.01f)
	{
		Move_Letter_H_X = Move_Letter_H_X + 0.0015f;
		Move_Letter_H_Y = Move_Letter_H_Y - 0.0015f;
		//fprintf(gpFile, "Move_Letter_H_X = %f\n", Move_Letter_H_X);
	}

	if (Move_Letter_H_X >= -1.664147 && Move_Letter_A_Y <= 0.03f)
	{
		Move_Letter_A_X = Move_Letter_A_X + 0.003f;
		Move_Letter_A_Y = Move_Letter_A_Y + 0.003f;
		//fprintf(gpFile, "Move_Letter_A_X = %f\n", Move_Letter_A_X);
	}

	if (Move_Letter_A_X >= -1.664147f && Move_Letter_R_Y >= 0.022f)
	{
		Move_Letter_R_X = Move_Letter_R_X - 0.0015f;
		Move_Letter_R_Y = Move_Letter_R_Y - 0.0015f;
		//fprintf(gpFile, "Move_Letter_R_X = %f\n", Move_Letter_R_X);
	}

	if (Move_Letter_R_X <= 2.121174 && Move_Letter_A2_Y <= 0.022f)
	{
		Move_Letter_A2_X = Move_Letter_A2_X - 0.003f;
		Move_Letter_A2_Y = Move_Letter_A2_Y + 0.003f;
		//fprintf(gpFile, "Move_Letter_A2_X = %f\n", Move_Letter_A2_X);
	}

	if (Move_Letter_A2_Y >= 0.022f && Move_Letter_T_X >= 5.9f)
	{
		Move_Letter_T_X = Move_Letter_T_X - 0.003f;
		
		//fprintf(gpFile, "Move_Letter_A2_X = %f\n", Move_Letter_A2_X);
	}
	
	if (Move_Letter_T_X <= 5.9f && Move_Middle_Plane_X <= 19.0f)
	{
		Move_Middle_Plane_X = Move_Middle_Plane_X + 0.004f;
	}

	if (Move_Letter_T_X <= 5.9f && Move_Upper_Plane_X <= 0.0f)
	{
		
		Move_Upper_Plane_X = Move_Upper_Plane_X + 0.003f;
		Move_Upper_Plane_Y = ((Move_Upper_Plane_X * Move_Upper_Plane_X) - 1.3f);
		
	}

	if (Move_Upper_Plane_X >= 0 && Move_Upper_Plane_X <=23.0f)
	{
		Move_Upper_Plane_X = Move_Upper_Plane_X + 0.004f;
	}

	if (Move_Upper_Plane_X >= 23.0f)
	{
		Move_Upper_Plane_X = Move_Upper_Plane_X + 0.0028f;
		Move_Upper_Plane_Y = (((Move_Upper_Plane_X - 22.999f) * (Move_Upper_Plane_X - 22.999f)) - 1.31111f);
	}
	if (Move_Letter_T_X <= 5.9f && Move_Lower_Plane_X <= 0.0f)
	{

		Move_Lower_Plane_X = Move_Lower_Plane_X + 0.003f;
		Move_Lower_Plane_Y = -((Move_Lower_Plane_X * Move_Lower_Plane_X) - 1.3f);

	}

	if (Move_Lower_Plane_X >= 0.0f && Move_Lower_Plane_X <= 23.0f)
	{
		Move_Lower_Plane_X = Move_Lower_Plane_X + 0.004f;
	}

	if (Move_Lower_Plane_X >= 23.0f)
	{
		Move_Lower_Plane_X = Move_Lower_Plane_X + 0.0028f;
		Move_Lower_Plane_Y = -(((Move_Lower_Plane_X - 22.999f) * (Move_Lower_Plane_X - 22.999f)) - 1.31111f);
	}

	//plane rotation code

	if (Rotate_Plane_Two <= 90.0f && Move_Letter_T_X <= 5.9f)
	{
		Rotate_Plane_Two = Rotate_Plane_Two + 0.06f;
	}

	if (Rotate_Plane_Three >= -90.0f && Move_Letter_T_X <= 5.9f)
	{
		Rotate_Plane_Three = Rotate_Plane_Three - 0.06f;
	}

	if (Move_Upper_Plane_X >= 23.0f && Rotate_Plane_Two <= 180.0f)
	{
		Rotate_Plane_Two = Rotate_Plane_Two + 0.05f;
	}

	if (Move_Lower_Plane_X >= 23.0f && Rotate_Plane_Three >= -180.0f)
	{
		Rotate_Plane_Three = Rotate_Plane_Three - 0.05f;
	}
	//letter T colouring

	if (Move_Middle_Plane_X >= 11.0f)
	{
		if(Red_T_Orange <= 255.0f)
		{
			Red_T_Orange = Red_T_Orange + 0.1f;
		}
		
		if (Green_T_Orange >= 103.0f)
		{
			Green_T_Orange = Green_T_Orange - 0.1f;
		}

		if (Blue_T_Orange >= 31.0f)
		{
			Blue_T_Orange = Blue_T_Orange - 0.1f;

		}
		
	}

	if (Move_Middle_Plane_X >= 11.0f)
	{
		if (Red_T_White <= 255.0f)
		{
			Red_T_White = Red_T_White + 0.1f;
		}

		if (Green_T_White <= 255.0f)
		{
			Green_T_White = Green_T_White + 0.1f;
		}

		if (Green_T_White <= 255.0f)
		{
			Blue_T_White = Blue_T_White + 0.1f;

		}
	}

	if (Move_Middle_Plane_X >= 11.0f)
	{
		if (Red_T_Green >= 4.0f)
		{
			Red_T_Green = Red_T_Green - 0.1f;
		}

		if (Green_T_Green >= 106.0f)
		{
			Green_T_Green = Green_T_Green - 0.1f;
		}

		if (Blue_T_Green >= 56.0f)
		{
			Blue_T_Green = Blue_T_Green - 0.1f;

		}
	}

	//Letter A3 Second Coloring

	if (Move_Middle_Plane_X >= 7.0f)
	{
		if (Red_A3_Second_Orange <= 255.0f)
		{
			Red_A3_Second_Orange = Red_A3_Second_Orange + 0.1f;
		}

		if (Green_A3_Second_Orange >= 103.0f)
		{
			Green_A3_Second_Orange = Green_A3_Second_Orange - 0.1f;
		}

		if (Blue_A3_Second_Orange >= 31.0f)
		{
			Blue_A3_Second_Orange = Blue_A3_Second_Orange - 0.1f;

		}

	}

	if (Move_Middle_Plane_X >= 7.0f)
	{
		if (Red_A3_Second_White <= 255.0f)
		{
			Red_A3_Second_White = Red_A3_Second_White + 0.1f;
		}

		if (Green_A3_Second_White <= 255.0f)
		{
			Green_A3_Second_White = Green_A3_Second_White + 0.1f;
		}

		if (Blue_A3_Second_White <= 255.0f)
		{
			Blue_A3_Second_White = Blue_A3_Second_White + 0.1f;

		}

	}

	if (Move_Middle_Plane_X >= 7.0f)
	{
		if (Red_A3_Second_Green >= 4.0f)
		{
			Red_A3_Second_Green = Red_A3_Second_Green - 0.1f;
		}

		if (Green_A3_Second_Green >= 106.0f)
		{
			Green_A3_Second_Green = Green_A3_Second_Green - 0.1f;
		}

		if (Blue_A3_Second_Green >= 56.0f)
		{
			Blue_A3_Second_Green = Blue_A3_Second_Green - 0.1f;

		}

	}

	//Letter A3 First Coloring

	if (Move_Middle_Plane_X >= 0.5f)
	{
		if (Red_A3_First_Orange <= 255.0f)
		{
			Red_A3_First_Orange = Red_A3_First_Orange + 0.1f;
		}

		if (Green_A3_First_Orange >= 103.0f)
		{
			Green_A3_First_Orange = Green_A3_First_Orange - 0.1f;
		}

		if (Blue_A3_First_Orange >= 31.0f)
		{
			Blue_A3_First_Orange = Blue_A3_First_Orange - 0.1f;

		}

	}

	if (Move_Middle_Plane_X >= 0.5f)
	{
		if (Red_A3_First_White <= 255.0f)
		{
			Red_A3_First_White = Red_A3_First_White + 0.1f;
		}

		if (Green_A3_First_White <= 255.0f)
		{
			Green_A3_First_White = Green_A3_First_White + 0.1f;
		}

		if (Blue_A3_First_White <= 255.0f)
		{
			Blue_A3_First_White = Blue_A3_First_White + 0.1f;

		}

	}

	if (Move_Middle_Plane_X >= 0.5f)
	{
		if (Red_A3_First_Green >= 4.0f)
		{
			Red_A3_First_Green = Red_A3_First_Green - 0.1f;
		}

		if (Green_A3_First_Green >= 106.0f)
		{
			Green_A3_First_Green = Green_A3_First_Green - 0.1f;
		}

		if (Blue_A3_First_Green >= 56.0f)
		{
			Blue_A3_First_Green = Blue_A3_First_Green - 0.1f;

		}

	}


	//Letter R colouring

	if (Move_Middle_Plane_X >= 4.5f)
	{
		if (Red_R_Orange <= 255.0f)
		{
			Red_R_Orange = Red_R_Orange + 0.1f;
		}

		if (Green_R_Orange >= 103.0f)
		{
			Green_R_Orange = Green_R_Orange - 0.1f;
		}

		if (Blue_R_Orange >= 31.0f)
		{
			Blue_R_Orange = Blue_R_Orange - 0.1f;

		}

	}

	if (Move_Middle_Plane_X >= 4.5f)
	{
		if (Red_R_White <= 255.0f)
		{
			Red_R_White = Red_R_White + 0.1f;
		}

		if (Green_R_White <= 255.0f)
		{
			Green_R_White = Green_R_White + 0.1f;
		}

		if (Blue_R_White <= 255.0f)
		{
			Blue_R_White = Blue_R_White + 0.1f;

		}

	}

	if (Move_Middle_Plane_X >= 4.5f)
	{
		if (Red_R_Green >= 4.0f)
		{
			Red_R_Green = Red_R_Green - 0.1f;
		}

		if (Green_R_Green >= 106.0f)
		{
			Green_R_Green = Green_R_Green - 0.1f;
		}

		if (Blue_R_Green >= 56.0f)
		{
			Blue_R_Green = Blue_R_Green - 0.1f;

		}

	}

	//letter H Colouring

	if (Move_Middle_Plane_X >= -3.5f)
	{
		if (Red_H_Orange <= 255.0f)
		{
			Red_H_Orange = Red_H_Orange + 0.1f;
		}

		if (Green_H_Orange >= 103.0f)
		{
			Green_H_Orange = Green_H_Orange - 0.1f;
		}

		if (Blue_H_Orange >= 31.0f)
		{
			Blue_H_Orange = Blue_H_Orange - 0.1f;

		}

	}

	if (Move_Middle_Plane_X >= -3.5f)
	{
		if (Red_H_White <= 255.0f)
		{
			Red_H_White = Red_H_White + 0.1f;
		}

		if (Green_H_White <= 255.0f)
		{
			Green_H_White = Green_H_White + 0.1f;
		}

		if (Blue_H_White <= 255.0f)
		{
			Blue_H_White = Blue_H_White + 0.1f;

		}

	}

	if (Move_Middle_Plane_X >= -3.5f)
	{
		if (Red_H_Green >= 4.0f)
		{
			Red_H_Green = Red_H_Green - 0.1f;
		}

		if (Green_H_Green >= 106.0f)
		{
			Green_H_Green = Green_H_Green - 0.1f;
		}

		if (Blue_H_Green >= 56.0f)
		{
			Blue_H_Green = Blue_H_Green - 0.1f;

		}

	}
	//letter B Colouring

	if (Move_Middle_Plane_X >= -7.5f)
	{
		if (Red_B_Orange <= 255.0f)
		{
			Red_B_Orange = Red_B_Orange + 0.1f;
		}

		if (Green_B_Orange >= 103.0f)
		{
			Green_B_Orange = Green_B_Orange - 0.1f;
		}

		if (Blue_B_Orange >= 31.0f)
		{
			Blue_B_Orange = Blue_B_Orange - 0.1f;

		}

	}

	if (Move_Middle_Plane_X >= -7.5f)
	{
		if (Red_B_White <= 255.0f)
		{
			Red_B_White = Red_B_White + 0.1f;
		}

		if (Green_B_White <= 255.0f)
		{
			Green_B_White = Green_B_White + 0.1f;
		}

		if (Blue_B_White <= 255.0f)
		{
			Blue_B_White = Blue_B_White + 0.1f;

		}

	}

	if (Move_Middle_Plane_X >= -7.5f)
	{
		if (Red_B_Green >= 4.0f)
		{
			Red_B_Green = Red_B_Green - 0.1f;
		}

		if (Green_B_Green >= 106.0f)
		{
			Green_B_Green = Green_B_Green - 0.1f;
		}

		if (Blue_B_Green >= 56.0f)
		{
			Blue_B_Green = Blue_B_Green - 0.1f;

		}

	}





	
}

void uninitialize(void)
{
	//Function declarations

	void ToggleFullScreen(void);

	//code
	
	// if application is exiting in full screen

	if (gbFullScreen == TRUE)
	{
		ToggleFullScreen();
		gbFullScreen = FALSE;

	}

	//make the HDC as current context

	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}

	//Delete rendering Context

	if (ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}


	//Relese the HDC
	
	if (ghdc)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}
	
	// Distroy Window

	if (ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd = NULL;

	}

	//close the log file

	if (gpFile)
	{
		fprintf(gpFile, "Program Ended successfully\n");
		fclose(gpFile);
		gpFile = NULL;

	}


}

void Letter_B(void)
{ 
	
	glPushMatrix();
	
	glScalef(0.25f, 0.25f, 0.0f);
	//glTranslatef(-4.0f, 0.0f, 0.0f);
	glTranslatef(Move_Letter_B, 0.0f, 0.0f);
	//fprintf(gpFile, "Move_Letter_B = %f\n", Move_Letter_B);
	glColor3f(0.5f, 0.5f, 0.5f);
	//glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);                 //upper left quad vertical strip
	
	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_White / 256.0f), (Green_B_White / 256.0f), (Blue_B_White / 256.0f));
	}

	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));
	glVertex3f(-0.98f, 0.22f, 0.0f);

	
	glVertex3f(-0.3f, 0.22f, 0.0f);    

	
	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_Orange / 256.0f), (Green_B_Orange / 256.0f), (Blue_B_Orange / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));
	glVertex3f(-0.3f, 0.557f, 0.0f);
	
	
	glVertex3f(-0.98f, 0.99f, 0.0f);

	glEnd();

	

	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));
	glBegin(GL_QUADS);            //middle quad vertical strip


	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_White / 256.0f), (Green_B_White / 256.0f), (Blue_B_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.3f, 0.22f, 0.0f);


	glVertex3f(-0.98f, 0.22f, 0.0f);

	
	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_White / 256.0f), (Green_B_White / 256.0f), (Blue_B_White / 256.0f));
	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(-0.98f, -0.175f, 0.0f);

	glVertex3f(-0.3f, -0.175f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);            //lower quad vertical strip

	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_White / 256.0f), (Green_B_White / 256.0f), (Blue_B_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));
	glVertex3f(-0.3f, -0.175f, 0.0f);


	glVertex3f(-0.98f, -0.175f, 0.0f);

	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_Green / 256.0f), (Green_B_Green / 256.0f), (Blue_B_Green / 256.0f));
	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));
	glVertex3f(-0.98f, -0.99f, 0.0f);

	glVertex3f(-0.3f, -0.595f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);            //upper horizontal quad 

	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_Orange / 256.0f), (Green_B_Orange / 256.0f), (Blue_B_Orange / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.54f, 0.99f, 0.0f);


	glVertex3f(-0.98f, 0.99f, 0.0f);


	glVertex3f(-0.3f, 0.557f, 0.0f);

	glVertex3f(0.17f, 0.557f, 0.0f);

	glEnd();



	glBegin(GL_QUADS);            //lower horizontal quad 

	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_Green / 256.0f), (Green_B_Green / 256.0f), (Blue_B_Green / 256.0f));
	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));
	glVertex3f(0.17f, -0.594f, 0.0f);


	glVertex3f(-0.3f, -0.594f, 0.0f);


	glVertex3f(-0.98f, -0.99f, 0.0f);

	glVertex3f(0.54f, -0.99f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);            //upper slant quad 

	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_Orange / 256.0f), (Green_B_Orange / 256.0f), (Blue_B_Orange / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));
	glVertex3f(0.98f, 0.7f, 0.0f);


	glVertex3f(0.54f, 0.99f, 0.0f);


	glVertex3f(0.17f, 0.557f, 0.0f);

	glVertex3f(0.3f, 0.49f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);            //upper verticle quad 

	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_Orange / 256.0f), (Green_B_Orange / 256.0f), (Blue_B_Orange / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.98f, 0.7f, 0.0f);


	glVertex3f(0.3f, 0.49f, 0.0f);


	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_White / 256.0f), (Green_B_White / 256.0f), (Blue_B_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.3f, 0.3f, 0.0f);

	glVertex3f(0.98f, 0.2f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);            //upper slant quad triangle adjecent 

	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_White / 256.0f), (Green_B_White / 256.0f), (Blue_B_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));
	glVertex3f(0.98f, 0.2f, 0.0f);


	glVertex3f(0.3f, 0.3f, 0.0f);

	

	glVertex3f(0.19f, 0.22f, 0.0f);

	glVertex3f(0.7f, 0.02f, 0.0f);

	glEnd();

	glBegin(GL_TRIANGLES);            //middle triangle

	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_White / 256.0f), (Green_B_White / 256.0f), (Blue_B_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.7f, 0.02f, 0.0f);
	glVertex3f(0.19f, 0.22f, 0.0f);
	glVertex3f(0.19f, -0.17f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);            //lower slant quad triangle adjecent

	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_White / 256.0f), (Green_B_White / 256.0f), (Blue_B_White / 256.0f));
	}

	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.7f, 0.02f, 0.0f);


	glVertex3f(0.19f, -0.17f, 0.0f);


	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_White / 256.0f), (Green_B_White / 256.0f), (Blue_B_White / 256.0f));
	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(0.3f, -0.26f, 0.0f);

	glVertex3f(0.98f, -0.16f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);            //lower vertical quad triangle adjecent 

	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_White / 256.0f), (Green_B_White / 256.0f), (Blue_B_White / 256.0f));
	}

	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.98f, -0.16f, 0.0f);


	glVertex3f(0.3f, -0.26f, 0.0f);

	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_Green / 256.0f), (Green_B_Green / 256.0f), (Blue_B_Green / 256.0f));
	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(0.3f, -0.5f, 0.0f);

	glVertex3f(0.98f, -0.72f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);            //lower slant quad


	glVertex3f(0.98f, -0.72f, 0.0f);


	glVertex3f(0.3f, -0.5f, 0.0f);


	glVertex3f(0.17f, -0.595f, 0.0f);

	glVertex3f(0.54f, -0.99f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);            //horizontal middle quad

	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_White / 256.0f), (Green_B_White / 256.0f), (Blue_B_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.19f, 0.22f, 0.0f);


	glVertex3f(-0.3f, 0.22f, 0.0f);


	glVertex3f(-0.3f, -0.175f, 0.0f);

	glVertex3f(0.19f, -0.17f, 0.0f);

	glEnd();

	glPopMatrix();
}
void Letter_H(void)
{
	glPushMatrix();
	glScalef(0.24f, 0.245f, 0.0f);
	//glTranslatef(-1.7f, -0.01f, 0.0f);
	glTranslatef(Move_Letter_H_X, Move_Letter_H_Y, 0.0f);
	//glColor3f(1.0f, 0.0f, 0.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_QUADS);                //left side upper quad

	if (Move_Middle_Plane_X >= -3.5f)
	{
		glColor3f((Red_H_Orange / 256.0f), (Green_H_Orange / 256.0f), (Blue_H_Orange / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(-0.4f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);

	if (Move_Middle_Plane_X >= -3.5f)
	{
		glColor3f((Red_H_White / 256.0f), (Green_H_White / 256.0f), (Blue_H_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-1.0f, 0.2f, 0.0f);
	glVertex3f(-0.4f, 0.2f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);                //left middle  quad

	if (Move_Middle_Plane_X >= -3.5f)
	{
		glColor3f((Red_H_White / 256.0f), (Green_H_White / 256.0f), (Blue_H_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.4f, 0.2f, 0.0f);
	glVertex3f(-1.0f, 0.2f, 0.0f);
	glVertex3f(-1.0f, -0.2f, 0.0f);
	glVertex3f(-0.4f, -0.2f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);                //left lower quad

	if (Move_Middle_Plane_X >= -3.5f)
	{
		glColor3f((Red_H_White / 256.0f), (Green_H_White / 256.0f), (Blue_H_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.4f, -0.2f, 0.0f);
	glVertex3f(-1.0f, -0.2f, 0.0f);

	if (Move_Middle_Plane_X >= -3.5f)
	{
		glColor3f((Red_H_Green / 256.0f), (Green_H_Green / 256.0f), (Blue_H_Green / 256.0f));
	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(-0.4f, -1.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);                //middle horizontal line

	if (Move_Middle_Plane_X >= -3.5f)
	{
		glColor3f((Red_H_White / 256.0f), (Green_H_White / 256.0f), (Blue_H_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.4f, 0.2f, 0.0f);
	glVertex3f(-0.4f, 0.2f, 0.0f);
	glVertex3f(-0.4f, -0.2f, 0.0f);
	glVertex3f(0.4f, -0.2f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);                //right side upper quad

	if (Move_Middle_Plane_X >= -3.5f)
	{
		glColor3f((Red_H_Orange / 256.0f), (Green_H_Orange / 256.0f), (Blue_H_Orange / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.4f, 1.0f, 0.0f);

	if (Move_Middle_Plane_X >= -3.5f)
	{
		glColor3f((Red_H_White / 256.0f), (Green_H_White / 256.0f), (Blue_H_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.4f, 0.2f, 0.0f);
	glVertex3f(1.0f, 0.2f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);                //right side middle quad
	
	if (Move_Middle_Plane_X >= -3.5f)
	{
		glColor3f((Red_H_White / 256.0f), (Green_H_White / 256.0f), (Blue_H_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(1.0f, 0.2f, 0.0f);
	glVertex3f(0.4f, 0.2f, 0.0f);
	glVertex3f(0.4f, -0.2f, 0.0f);
	glVertex3f(1.0f, -0.2f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);                //right side lower quad

	if (Move_Middle_Plane_X >= -3.5f)
	{
		glColor3f((Red_H_White / 256.0f), (Green_H_White / 256.0f), (Blue_H_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(1.0f, -0.2f, 0.0f);
	glVertex3f(0.4f, -0.2f, 0.0f);

	if (Move_Middle_Plane_X >= -3.5f)
	{
		glColor3f((Red_H_Green / 256.0f), (Green_H_Green / 256.0f), (Blue_H_Green / 256.0f));
	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(0.4f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glEnd();
	glPopMatrix();
}
void Letter_A1(void)
{
	glPushMatrix();
	//glScalef(0.24f, 0.25f, 0.0f);
	 glScalef(0.24f, 0.260f, 0.0f);
	//glTranslatef(0.6f, -0.01f, 0.0f);
	//glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(Move_Letter_A_X, Move_Letter_A_Y, 0.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_TRIANGLES);           //middle triangle

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_Orange / 256.0f), (Green_A3_First_Orange / 256.0f), (Blue_A3_First_Orange / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.25f, 1.0f, 0.0f);
	glVertex3f(-0.25f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.3f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);               //left side slant quad

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_Orange / 256.0f), (Green_A3_First_Orange / 256.0f), (Blue_A3_First_Orange / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.0f, 0.3f, 0.0f);
	glVertex3f(-0.25f, 1.0f, 0.0f);

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.68f, -0.14f, 0.0f);
	glVertex3f(-0.16f, -0.14f, 0.0f);
	
	glEnd();

	glBegin(GL_QUADS);               //left side slant quad adjecent horizontal quad 

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.16f, -0.14f, 0.0f);
	glVertex3f(-0.68f, -0.14f, 0.0f);
	glVertex3f(-0.84f, -0.56f, 0.0f);
	glVertex3f(-0.32f, -0.56f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);               //left side slant quad bottom

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.32f, -0.56f, 0.0f);
	glVertex3f(-0.84f, -0.56f, 0.0f);

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_Green / 256.0f), (Green_A3_First_Green / 256.0f), (Blue_A3_First_Green / 256.0f));

	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(-0.5f, -1.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);              //right side slant quad upper

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_Orange / 256.0f), (Green_A3_First_Orange / 256.0f), (Blue_A3_First_Orange / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.25f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.3f, 0.0f);

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.16f, -0.14f, 0.0f);
	glVertex3f(0.68f, -0.14f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);               //right side slant quad adjecent to horizontal quad

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.68f, -0.14f, 0.0f);
	glVertex3f(0.16f, -0.14f, 0.0f);
	glVertex3f(0.32f, -0.56f, 0.0f);
	glVertex3f(0.84f, -0.56f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);               //right side slant quad bottom

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.84f, -0.56f, 0.0f);
	glVertex3f(0.32f, -0.56f, 0.0f);

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_Green / 256.0f), (Green_A3_First_Green / 256.0f), (Blue_A3_First_Green / 256.0f));

	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(0.5f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);               //Middle horizontal quad

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.16f, -0.14f, 0.0f);
	glVertex3f(-0.16f, -0.14f, 0.0f);
	glVertex3f(-0.32f, -0.56f, 0.0f);
	glVertex3f(0.32f, -0.56f, 0.0f);
	glEnd();
	glPopMatrix();
}
void Letter_A2(void)
{
	glPushMatrix();
	//glScalef(0.24f, 0.25f, 0.0f);
	glScalef(0.24f, 0.260f, 0.0f);
	//glTranslatef(5.0f, 0.0f, 0.0f);
	//glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(Move_Letter_A2_X, Move_Letter_A2_Y, 0.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_TRIANGLES);           //middle triangle

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_Orange / 256.0f), (Green_A3_Second_Orange / 256.0f), (Blue_A3_Second_Orange / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.25f, 1.0f, 0.0f);
	glVertex3f(-0.25f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.3f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);               //left side slant quad

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_Orange / 256.0f), (Green_A3_Second_Orange / 256.0f), (Blue_A3_Second_Orange / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.0f, 0.3f, 0.0f);
	glVertex3f(-0.25f, 1.0f, 0.0f);

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.68f, -0.14f, 0.0f);
	glVertex3f(-0.16f, -0.14f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);               //left side slant quad adjecent horizontal quad 

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.16f, -0.14f, 0.0f);
	glVertex3f(-0.68f, -0.14f, 0.0f);
	glVertex3f(-0.84f, -0.56f, 0.0f);
	glVertex3f(-0.32f, -0.56f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);               //left side slant quad bottom

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));
	}

	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.32f, -0.56f, 0.0f);
	glVertex3f(-0.84f, -0.56f, 0.0f);

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_Green / 256.0f), (Green_A3_Second_Green / 256.0f), (Blue_A3_Second_Green / 256.0f));
	}

	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(-0.5f, -1.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);              //right side slant quad upper


	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_Orange / 256.0f), (Green_A3_Second_Orange / 256.0f), (Blue_A3_Second_Orange / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.25f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.3f, 0.0f);

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.16f, -0.14f, 0.0f);
	glVertex3f(0.68f, -0.14f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);               //right side slant quad adjecent to horizontal quad

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.68f, -0.14f, 0.0f);
	glVertex3f(0.16f, -0.14f, 0.0f);
	glVertex3f(0.32f, -0.56f, 0.0f);
	glVertex3f(0.84f, -0.56f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);               //right side slant quad bottom

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.84f, -0.56f, 0.0f);
	glVertex3f(0.32f, -0.56f, 0.0f);

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_Green / 256.0f), (Green_A3_Second_Green / 256.0f), (Blue_A3_Second_Green / 256.0f));
	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(0.5f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);               //Middle horizontal quad

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.16f, -0.14f, 0.0f);
	glVertex3f(-0.16f, -0.14f, 0.0f);
	glVertex3f(-0.32f, -0.56f, 0.0f);
	glVertex3f(0.32f, -0.56f, 0.0f);
	glEnd();
	glPopMatrix();
}
void Letter_T(void)
{
	glPushMatrix();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glScalef(0.3f, 0.249f, 0.0f);
	//glTranslatef(5.89f, -0.01f, 0.0f);
	glTranslatef(Move_Letter_T_X, -0.01f, 0.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_QUADS);           //Horizontal quad

	if (Move_Middle_Plane_X >= 11.0f)
	{
		glColor3f((Red_T_Orange / 256.0f), (Green_T_Orange / 256.0f), (Blue_T_Orange / 256.0f));

	}

	glVertex3f(0.95f, 1.0f, 0.0f);
	glVertex3f(-0.95f, 1.0f, 0.0f);
	glVertex3f(-0.95f, 0.6f, 0.0f);
	glVertex3f(0.95f, 0.6f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);           //Vertical quad upper

	if (Move_Middle_Plane_X >= 11.0f)
	{
		glColor3f((Red_T_Orange / 256.0f), (Green_T_Orange / 256.0f), (Blue_T_Orange / 256.0f));

	}

	glVertex3f(0.2f, 0.6f, 0.0f);
	glVertex3f(-0.2f, 0.6f, 0.0f);

	if (Move_Middle_Plane_X >= 11.0f)
	{
		glColor3f((Red_T_White / 256.0f), (Green_T_White / 256.0f), (Blue_T_White / 256.0f));

	}

	glVertex3f(-0.2f, 0.2f, 0.0f);
	glVertex3f(0.2f, 0.2f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	
	if (Move_Middle_Plane_X >= 11.0f)
	{
		glColor3f((Red_T_White / 256.0f), (Green_T_White / 256.0f), (Blue_T_White / 256.0f));

	}

	glVertex3f(0.2f, 0.2f, 0.0f);
	glVertex3f(-0.2f, 0.2f, 0.0f);
	glVertex3f(-0.2f, -0.22f, 0.0f);
	glVertex3f(0.2f, -0.22f, 0.0f);
	glEnd();
	glBegin(GL_QUADS);           //Vertical quad bottom

	if (Move_Middle_Plane_X >= 11.0f)
	{
		glColor3f((Red_T_White / 256.0f), (Green_T_White / 256.0f), (Blue_T_White / 256.0f));

	}

	glVertex3f(0.2f, -0.22f, 0.0f);
	glVertex3f(-0.2f, -0.22f, 0.0f);
	
	if (Move_Middle_Plane_X >= 11.0f)
	{
		glColor3f((Red_T_Green / 256.0f), (Green_T_Green / 256.0f), (Blue_T_Green / 256.0f));

	}

	glVertex3f(-0.2f, -1.0f, 0.0f);
	glVertex3f(0.2f, -1.0f, 0.0f);
	glEnd();

	glDisable(GL_BLEND);
	glPopMatrix();
}
void Letter_R(void)
{
	glPushMatrix();

	glScalef(0.32f, 0.29f, 0.0f);
	//glTranslatef(2.1f, -0.01f, 0.0f);
	glTranslatef(Move_Letter_R_X, Move_Letter_R_Y, 0.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_QUADS);           //left side upper vertical quad

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_Orange / 256.0f), (Green_R_Orange / 256.0f), (Blue_R_Orange / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(-0.35f, 0.9f, 0.0f);
	glVertex3f(-0.7f, 0.9f, 0.0f);

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_White / 256.0f), (Green_R_White / 256.0f), (Blue_R_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.7f, 0.15f, 0.0f);
	glVertex3f(-0.35f, 0.15f, 0.0f);
	glEnd();
	//glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);           //left side middle vertical quad


	/*glVertex3f(-0.35f, 0.15f, 0.0f);
	glVertex3f(-0.7f, 0.15f, 0.0f);
	glVertex3f(-0.7f, -0.1f, 0.0f);
	glVertex3f(-0.35f, -0.1f, 0.0f);*/

	glVertex3f(-0.35f, 0.18f, 0.0f);
	glVertex3f(-0.7f, 0.18f, 0.0f);
	glVertex3f(-0.7f, -0.13f, 0.0f);
	glVertex3f(-0.35f, -0.13f, 0.0f);
	glEnd();
	//glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);           //left side bottom vertical quad

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_White / 256.0f), (Green_R_White / 256.0f), (Blue_R_White / 256.0f));
	}

	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.35f, -0.13f, 0.0f);
	glVertex3f(-0.7f, -0.13f, 0.0f);

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_Green / 256.0f), (Green_R_Green / 256.0f), (Blue_R_Green / 256.0f));
	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(-0.7f, -0.9f, 0.0f);
	glVertex3f(-0.35f, -0.9f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);             // upper horizontal quad

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_Orange / 256.0f), (Green_R_Orange / 256.0f), (Blue_R_Orange / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.4f, 0.9f, 0.0f);
	glVertex3f(-0.35f, 0.9f, 0.0f);
	glVertex3f(-0.35f, 0.65f, 0.0f);
	glVertex3f(0.2f, 0.65f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);             // right side slant quad upper

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_Orange / 256.0f), (Green_R_Orange / 256.0f), (Blue_R_Orange / 256.0f));
	}
//	glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.4f, 0.9f, 0.0f);
	glVertex3f(0.2f, 0.65f, 0.0f);
	glVertex3f(0.35f, 0.55f, 0.0f);
	glVertex3f(0.7f, 0.62f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);             // right side verticle quad 

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_Orange / 256.0f), (Green_R_Orange / 256.0f), (Blue_R_Orange / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.7f, 0.62f, 0.0f);
	glVertex3f(0.35f, 0.55f, 0.0f);
	glVertex3f(0.35f, 0.25f, 0.0f);
	glVertex3f(0.7f, 0.25f, 0.0f);
	glEnd();
	//glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);             // right side slant quad adjecent to triangle
	/*glVertex3f(0.7f, 0.25f, 0.0f);
	glVertex3f(0.35f, 0.25f, 0.0f);
	glVertex3f(0.2f, 0.15f, 0.0f);
	glVertex3f(0.5f, 0.05f, 0.0f);*/

	glVertex3f(0.7f, 0.25f, 0.0f);
	glVertex3f(0.35f, 0.25f, 0.0f);

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_White / 256.0f), (Green_R_White / 256.0f), (Blue_R_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.2f, 0.18f, 0.0f);
	glVertex3f(0.5f, 0.05f, 0.0f);
	glEnd();
	//glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLES);             //  Central Triangle

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_White / 256.0f), (Green_R_White / 256.0f), (Blue_R_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.2f, 0.18f, 0.0f);
	glVertex3f(0.5f, 0.05f, 0.0f);
	glVertex3f(0.2f, -0.13f, 0.0f);
	
	glEnd();
	//glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);             // right side slant quad adjecent to triangle bottom

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_White / 256.0f), (Green_R_White / 256.0f), (Blue_R_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.5f, 0.05f, 0.0f);
	glVertex3f(0.2f, -0.1f, 0.0f);

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_White / 256.0f), (Green_R_White / 256.0f), (Blue_R_White / 256.0f));
	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(0.34f, -0.2f, 0.0f);
	glVertex3f(0.7f, -0.14f, 0.0f);
	glEnd();
	//glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);             // right side verticle quad bottom

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_White / 256.0f), (Green_R_White / 256.0f), (Blue_R_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.7f, -0.14f, 0.0f);
	glVertex3f(0.34f, -0.2f, 0.0f);

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_Green / 256.0f), (Green_R_Green / 256.0f), (Blue_R_Green / 256.0f));
	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(0.34f, -0.9f, 0.0f);
	glVertex3f(0.7f, -0.9f, 0.0f);
	glEnd();
	//glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);             // Central horizontal quad
	/*glVertex3f(0.2f, 0.15f, 0.0f);
	glVertex3f(-0.35f, 0.15f, 0.0f);
	glVertex3f(-0.35f, -0.1f, 0.0f);
	glVertex3f(0.2f, -0.1f, 0.0f);*/

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_White / 256.0f), (Green_R_White / 256.0f), (Blue_R_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.2f, 0.18f, 0.0f);
	glVertex3f(-0.35f, 0.18f, 0.0f);
	glVertex3f(-0.35f, -0.13f, 0.0f);
	glVertex3f(0.2f, -0.13f, 0.0f);
	glEnd();
	glPopMatrix();
}
void Letter_A3_First(void)
{
	glPushMatrix();
	glScalef(0.24f, 0.260f, 0.0f);
	glTranslatef(Move_Letter_A_X, Move_Letter_A_Y, 0.0f);
	//glColor3f(1.0f, 0.0f, 0.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_QUADS);                   //left side bottom vertical quad

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.2f, -0.2f, 0.0f); //right top
	glVertex3f(-0.9f, -0.2f, 0.0f); //left top

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_Green / 256.0f), (Green_A3_First_Green / 256.0f), (Blue_A3_First_Green / 256.0f));

	}

	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));


	glVertex3f(-0.9f, -1.0f, 0.0f); //left bottom
	glVertex3f(-0.2f, -1.0f, 0.0f); //right bottom
	glEnd();

	glBegin(GL_QUADS);                //left side middle vertical quad

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.2f, 0.2f, 0.0f); //right top
	glVertex3f(-0.9f, 0.2f, 0.0f); //left top
	glVertex3f(-0.9f, -0.2f, 0.0f); //left bottom
	glVertex3f(-0.2f, -0.2f, 0.0f); //right bottom
	glEnd();
	//glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);                      //left side upper  quad

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_Orange / 256.0f), (Green_A3_First_Orange / 256.0f), (Blue_A3_First_Orange / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(-0.2f, 0.95f, 0.0f); //right top
	glVertex3f(-0.9f, 0.95f, 0.0f); //left top

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.9f, 0.2f, 0.0f); //left bottom
	glVertex3f(-0.2f, 0.2f, 0.0f); //right bottom
	glEnd();

	glBegin(GL_QUADS);                      //upper horizontal quad

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_Orange / 256.0f), (Green_A3_First_Orange / 256.0f), (Blue_A3_First_Orange / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.9f, 0.95f, 0.0f); //right top
	glVertex3f(-0.2f, 0.95f, 0.0f); //left top
	glVertex3f(-0.2f, 0.6f, 0.0f); //left bottom
	glVertex3f(0.9f, 0.6f, 0.0f); //right bottom
	glEnd();

	glBegin(GL_QUADS);                      //right side upper vertical quad

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_Orange / 256.0f), (Green_A3_First_Orange / 256.0f), (Blue_A3_First_Orange / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.9f, 0.6f, 0.0f); //right top
	glVertex3f(0.2f, 0.6f, 0.0f); //left top

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.2f, 0.2f, 0.0f); //left bottom
	glVertex3f(0.9f, 0.2f, 0.0f); //right bottom
	glEnd();

	glBegin(GL_QUADS);                      //right side middle vertical quad

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.9f, 0.2f, 0.0f); //right top
	glVertex3f(0.2f, 0.2f, 0.0f); //left top
	glVertex3f(0.2f, -0.2f, 0.0f); //left bottom
	glVertex3f(0.9f, -0.2f, 0.0f); //right bottom
	glEnd();
	//glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);                      //right side bottom vertical quad

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.9f, -0.2f, 0.0f); //right top
	glVertex3f(0.2f, -0.2f, 0.0f); //left top

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_Green / 256.0f), (Green_A3_First_Green / 256.0f), (Blue_A3_First_Green / 256.0f));

	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(0.2f, -1.0f, 0.0f); //left bottom
	glVertex3f(0.9f, -1.0f, 0.0f); //right bottom
	glEnd();
	
	glBegin(GL_QUADS);                      //middle horizontal quad

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.2f, 0.2f, 0.0f); //right top
	glVertex3f(-0.2f, 0.2f, 0.0f); //left top
	glVertex3f(-0.2f, -0.2f, 0.0f); //left bottom
	glVertex3f(0.2f, -0.2f, 0.0f); //right bottom
	glEnd();

	glPopMatrix();
}
void Letter_A3_Second(void)
{
	glPushMatrix();
	glScalef(0.24f, 0.260f, 0.0f);
	//glTranslatef(5.0f, -0.01f, 0.0f);
	glTranslatef(Move_Letter_A2_X, Move_Letter_A2_Y, 0.0f);
	//glColor3f(1.0f, 0.0f, 0.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_QUADS);                   //left side bottom vertical quad

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.2f, -0.2f, 0.0f); //right top
	glVertex3f(-0.9f, -0.2f, 0.0f); //left top

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_Green / 256.0f), (Green_A3_Second_Green / 256.0f), (Blue_A3_Second_Green / 256.0f));

	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));


	glVertex3f(-0.9f, -1.0f, 0.0f); //left bottom
	glVertex3f(-0.2f, -1.0f, 0.0f); //right bottom
	glEnd();

	glBegin(GL_QUADS);                //left side middle vertical quad

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.2f, 0.2f, 0.0f); //right top
	glVertex3f(-0.9f, 0.2f, 0.0f); //left top
	glVertex3f(-0.9f, -0.2f, 0.0f); //left bottom
	glVertex3f(-0.2f, -0.2f, 0.0f); //right bottom
	glEnd();
	//glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);                      //left side upper  quad

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_Orange / 256.0f), (Green_A3_Second_Orange / 256.0f), (Blue_A3_Second_Orange / 256.0f));

	}

	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(-0.2f, 0.95f, 0.0f); //right top
	glVertex3f(-0.9f, 0.95f, 0.0f); //left top

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.9f, 0.2f, 0.0f); //left bottom
	glVertex3f(-0.2f, 0.2f, 0.0f); //right bottom
	glEnd();

	glBegin(GL_QUADS);                      //upper horizontal quad

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_Orange / 256.0f), (Green_A3_Second_Orange / 256.0f), (Blue_A3_Second_Orange / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.9f, 0.95f, 0.0f); //right top
	glVertex3f(-0.2f, 0.95f, 0.0f); //left top
	glVertex3f(-0.2f, 0.6f, 0.0f); //left bottom
	glVertex3f(0.9f, 0.6f, 0.0f); //right bottom
	glEnd();

	glBegin(GL_QUADS);                      //right side upper vertical quad

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_Orange / 256.0f), (Green_A3_Second_Orange / 256.0f), (Blue_A3_Second_Orange / 256.0f));

	}

	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.9f, 0.6f, 0.0f); //right top
	glVertex3f(0.2f, 0.6f, 0.0f); //left top

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.2f, 0.2f, 0.0f); //left bottom
	glVertex3f(0.9f, 0.2f, 0.0f); //right bottom
	glEnd();

	glBegin(GL_QUADS);                      //right side middle vertical quad

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.9f, 0.2f, 0.0f); //right top
	glVertex3f(0.2f, 0.2f, 0.0f); //left top
	glVertex3f(0.2f, -0.2f, 0.0f); //left bottom
	glVertex3f(0.9f, -0.2f, 0.0f); //right bottom
	glEnd();
	//glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);                      //right side bottom vertical quad

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.9f, -0.2f, 0.0f); //right top
	glVertex3f(0.2f, -0.2f, 0.0f); //left top

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_Green / 256.0f), (Green_A3_Second_Green / 256.0f), (Blue_A3_Second_Green / 256.0f));

	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(0.2f, -1.0f, 0.0f); //left bottom
	glVertex3f(0.9f, -1.0f, 0.0f); //right bottom
	glEnd();

	glBegin(GL_QUADS);                      //middle horizontal quad

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.2f, 0.2f, 0.0f); //right top
	glVertex3f(-0.2f, 0.2f, 0.0f); //left top
	glVertex3f(-0.2f, -0.2f, 0.0f); //left bottom
	glVertex3f(0.2f, -0.2f, 0.0f); //right bottom
	glEnd();

	glPopMatrix();
	
}
void Plane1(void)
{
	glPushMatrix();
	glScalef(0.16f, 0.18f, 0.0f);
	glTranslatef(Move_Middle_Plane_X, 0.0f, 0.0f);
	glRotatef(270, 0.0f, 0.0f, 1.0f);
	glColor3f((183.0f / 260.0f), (212.0f / 260.0f), (218.0f / 260.0f));

	glBegin(GL_QUADS);                     //left tail


	glVertex3f(-0.08f, -0.36f, 0.0f);


	glVertex3f(-0.24f, -0.56f, 0.0f);


	glVertex3f(-0.22f, -0.62f, 0.0f);


	glVertex3f(-0.06f, -0.52f, 0.0f);

	//right tail

	glVertex3f(0.08f, -0.36f, 0.0f);


	glVertex3f(0.24f, -0.56f, 0.0f);


	glVertex3f(0.22f, -0.62f, 0.0f);


	glVertex3f(0.06f, -0.52f, 0.0f);

	//middle quad between tails
	glVertex3f(-0.08f, -0.36f, 0.0f);


	glVertex3f(-0.06f, -0.52f, 0.0f);


	glVertex3f(0.06f, -0.52f, 0.0f);


	glVertex3f(0.08f, -0.36f, 0.0f);

	glEnd();

	//tail middle triangle
	glColor3f((111.0f / 260.0f), (111.0f / 260.0f), (119.0f / 260.0f));

	glBegin(GL_TRIANGLES);

	glVertex3f(0.06f, -0.52f, 0.0f);
	glVertex3f(-0.06f, -0.52f, 0.0f);
	glVertex3f(0.0f, -0.6f, 0.0f);

	glEnd();

	//plane middle part quad
	//rgb(101, 170, 199)
	glColor3f((101.0f / 260.0f), (170.0f / 260.0f), (199.0f / 260.0f));
	glBegin(GL_QUADS);

	glVertex3f(0.12f, 0.46f, 0.0f);
	glVertex3f(-0.12f, 0.46f, 0.0f);
	glVertex3f(-0.08f, -0.36f, 0.0f);
	glVertex3f(0.08f, -0.36f, 0.0f);

	//left wing
	//rgb(183, 212, 218)
	glColor3f((183.0f / 260.0f), (212.0f / 260.0f), (218.0f / 260.0f));

	glVertex3f(-0.1f, 0.16f, 0.0f);
	glVertex3f(-0.45f, -0.12f, 0.0f);
	glVertex3f(-0.48f, -0.35f, 0.0f);
	glVertex3f(-0.08f, -0.29f, 0.0f);

	//right wing
	//rgb(183, 212, 218)
	glColor3f((183.0f / 260.0f), (212.0f / 260.0f), (218.0f / 260.0f));

	glVertex3f(0.1f, 0.16f, 0.0f);
	glVertex3f(0.45f, -0.12f, 0.0f);
	glVertex3f(0.48f, -0.35f, 0.0f);
	glVertex3f(0.08f, -0.29f, 0.0f);

	//left wing outer quad
	//rgb(111, 111, 119)
	glColor3f((111.0f / 260.0f), (111.0f / 260.0f), (119.0f / 260.0f));

	glVertex3f(-0.52f, 0.08f, 0.0f);
	glVertex3f(-0.52f, -0.4f, 0.0f);
	glVertex3f(-0.48f, -0.35f, 0.0f);
	glVertex3f(-0.45f, -0.12f, 0.0f);

	//right wing outer quad
	//rgb(111, 111, 119)
	glColor3f((111.0f / 260.0f), (111.0f / 260.0f), (119.0f / 260.0f));

	glVertex3f(0.52f, 0.08f, 0.0f);
	glVertex3f(0.52f, -0.4f, 0.0f);
	glVertex3f(0.48f, -0.35f, 0.0f);
	glVertex3f(0.45f, -0.12f, 0.0f);


	glEnd();

	//upper Triangle

	glBegin(GL_TRIANGLES);

	glVertex3f(-0.06f, 0.46f, 0.0f);
	glVertex3f(0.0f, 0.9f, 0.0f);
	glVertex3f(0.06f, 0.46f, 0.0f);

	glEnd();

	glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glBegin(GL_LINES);  //orange circle                                


	float Radious_Orange = 0.06f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = Radious_Orange * cos(angle) + (0.0f);
		float y = Radious_Orange * sin(angle) + (-0.25f);



		// Circle Origin


		glVertex3f(0.0, -0.25, 0.0f);



		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));
	glBegin(GL_LINES);  //white circle                                


	float Radious_White = 0.04f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = Radious_White * cos(angle) + (0.0f);
		float y = Radious_White * sin(angle) + (-0.25f);



		// Circle Origin


		glVertex3f(0.0, -0.25, 0.0f);



		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));
	glBegin(GL_LINES);  //Green circle                                


	float Radious_Green = 0.02f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = Radious_Green * cos(angle) + (0.0f);
		float y = Radious_Green * sin(angle) + (-0.25f);



		// Circle Origin


		glVertex3f(0.0, -0.25, 0.0f);



		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	//engine fire of plane

	glBegin(GL_QUADS);
	glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.13f, -0.62f, 0.0f);
	glVertex3f(-0.13f, -0.62f, 0.0f);
	glVertex3f(-0.13f, -0.64f, 0.0f);
	glVertex3f(0.13f, -0.64f, 0.0f);

	glEnd();

	glBegin(GL_TRIANGLES);

	glVertex3f(-0.0434f, -0.64f, 0.0f);   //right base point
	glVertex3f(-0.13f, -0.64f, 0.0f);     //left base point
	//glVertex3f(-0.08667f, -0.68f, 0.0f);  //apex point
	glVertex3f(-0.05f, -0.70f, 0.0f);  //apex point



	glEnd();
	glBegin(GL_TRIANGLES);

	glVertex3f(0.0432f, -0.64f, 0.0f);   //right base point
	glVertex3f(-0.0434f, -0.64f, 0.0f);     //left base point
	glVertex3f(-0.0001f, -0.9f, 0.0f);  //apex point


	glEnd();

	glBegin(GL_TRIANGLES);

	glVertex3f(0.1298f, -0.64f, 0.0f);   //right base point
	glVertex3f(0.0432f, -0.64f, 0.0f);  //left base point
	//glVertex3f(0.0865, -0.68f, 0.0f);  //apex point
	glVertex3f(0.05f, -0.70f, 0.0f);

	glEnd();
	
	glPopMatrix();
}
void Plane2(void)
{
	glPushMatrix();
	glScalef(0.16f, 0.18f, 0.0f);
	
	glTranslatef(-9.0f, 2.5f, 0.0f);
	
	//glRotatef(Rotate_Plane_Two, 0.0f, 0.0f, 1.0f);
	
	
	
	glTranslatef(Move_Upper_Plane_X, Move_Upper_Plane_Y, 0.0f);
	glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(Rotate_Plane_Two, 0.0f, 0.0f, 1.0f);
	glColor3f((183.0f / 260.0f), (212.0f / 260.0f), (218.0f / 260.0f));

	glBegin(GL_QUADS);                     //left tail


	glVertex3f(-0.08f, -0.36f, 0.0f);


	glVertex3f(-0.24f, -0.56f, 0.0f);


	glVertex3f(-0.22f, -0.62f, 0.0f);


	glVertex3f(-0.06f, -0.52f, 0.0f);

	//right tail

	glVertex3f(0.08f, -0.36f, 0.0f);


	glVertex3f(0.24f, -0.56f, 0.0f);


	glVertex3f(0.22f, -0.62f, 0.0f);


	glVertex3f(0.06f, -0.52f, 0.0f);

	//middle quad between tails
	glVertex3f(-0.08f, -0.36f, 0.0f);


	glVertex3f(-0.06f, -0.52f, 0.0f);


	glVertex3f(0.06f, -0.52f, 0.0f);


	glVertex3f(0.08f, -0.36f, 0.0f);

	glEnd();

	//tail middle triangle
	glColor3f((111.0f / 260.0f), (111.0f / 260.0f), (119.0f / 260.0f));

	glBegin(GL_TRIANGLES);

	glVertex3f(0.06f, -0.52f, 0.0f);
	glVertex3f(-0.06f, -0.52f, 0.0f);
	glVertex3f(0.0f, -0.6f, 0.0f);

	glEnd();

	//plane middle part quad
	//rgb(101, 170, 199)
	glColor3f((101.0f / 260.0f), (170.0f / 260.0f), (199.0f / 260.0f));
	glBegin(GL_QUADS);

	glVertex3f(0.12f, 0.46f, 0.0f);
	glVertex3f(-0.12f, 0.46f, 0.0f);
	glVertex3f(-0.08f, -0.36f, 0.0f);
	glVertex3f(0.08f, -0.36f, 0.0f);

	//left wing
	//rgb(183, 212, 218)
	glColor3f((183.0f / 260.0f), (212.0f / 260.0f), (218.0f / 260.0f));

	glVertex3f(-0.1f, 0.16f, 0.0f);
	glVertex3f(-0.45f, -0.12f, 0.0f);
	glVertex3f(-0.48f, -0.35f, 0.0f);
	glVertex3f(-0.08f, -0.29f, 0.0f);

	//right wing
	//rgb(183, 212, 218)
	glColor3f((183.0f / 260.0f), (212.0f / 260.0f), (218.0f / 260.0f));

	glVertex3f(0.1f, 0.16f, 0.0f);
	glVertex3f(0.45f, -0.12f, 0.0f);
	glVertex3f(0.48f, -0.35f, 0.0f);
	glVertex3f(0.08f, -0.29f, 0.0f);

	//left wing outer quad
	//rgb(111, 111, 119)
	glColor3f((111.0f / 260.0f), (111.0f / 260.0f), (119.0f / 260.0f));

	glVertex3f(-0.52f, 0.08f, 0.0f);
	glVertex3f(-0.52f, -0.4f, 0.0f);
	glVertex3f(-0.48f, -0.35f, 0.0f);
	glVertex3f(-0.45f, -0.12f, 0.0f);

	//right wing outer quad
	//rgb(111, 111, 119)
	glColor3f((111.0f / 260.0f), (111.0f / 260.0f), (119.0f / 260.0f));

	glVertex3f(0.52f, 0.08f, 0.0f);
	glVertex3f(0.52f, -0.4f, 0.0f);
	glVertex3f(0.48f, -0.35f, 0.0f);
	glVertex3f(0.45f, -0.12f, 0.0f);


	glEnd();

	//upper Triangle

	glBegin(GL_TRIANGLES);

	glVertex3f(-0.06f, 0.46f, 0.0f);
	glVertex3f(0.0f, 0.9f, 0.0f);
	glVertex3f(0.06f, 0.46f, 0.0f);

	glEnd();

	glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glBegin(GL_LINES);  //orange circle                                


	float Radious_Orange = 0.06f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = Radious_Orange * cos(angle) + (0.0f);
		float y = Radious_Orange * sin(angle) + (-0.25f);



		// Circle Origin


		glVertex3f(0.0, -0.25, 0.0f);



		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));
	glBegin(GL_LINES);  //white circle                                


	float Radious_White = 0.04f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = Radious_White * cos(angle) + (0.0f);
		float y = Radious_White * sin(angle) + (-0.25f);



		// Circle Origin


		glVertex3f(0.0, -0.25, 0.0f);



		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));
	glBegin(GL_LINES);  //Green circle                                


	float Radious_Green = 0.02f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = Radious_Green * cos(angle) + (0.0f);
		float y = Radious_Green * sin(angle) + (-0.25f);



		// Circle Origin


		glVertex3f(0.0, -0.25, 0.0f);



		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	//engine fire of plane

	glBegin(GL_QUADS);
	glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.13f, -0.62f, 0.0f);
	glVertex3f(-0.13f, -0.62f, 0.0f);
	glVertex3f(-0.13f, -0.64f, 0.0f);
	glVertex3f(0.13f, -0.64f, 0.0f);

	glEnd();

	glBegin(GL_TRIANGLES);

	glVertex3f(-0.0434f, -0.64f, 0.0f);   //right base point
	glVertex3f(-0.13f, -0.64f, 0.0f);     //left base point
	//glVertex3f(-0.08667f, -0.68f, 0.0f);  //apex point
	glVertex3f(-0.05f, -0.70f, 0.0f);  //apex point



	glEnd();
	glBegin(GL_TRIANGLES);

	glVertex3f(0.0432f, -0.64f, 0.0f);   //right base point
	glVertex3f(-0.0434f, -0.64f, 0.0f);     //left base point
	glVertex3f(-0.0001f, -0.9f, 0.0f);  //apex point


	glEnd();

	glBegin(GL_TRIANGLES);

	glVertex3f(0.1298f, -0.64f, 0.0f);   //right base point
	glVertex3f(0.0432f, -0.64f, 0.0f);  //left base point
	//glVertex3f(0.0865, -0.68f, 0.0f);  //apex point
	glVertex3f(0.05f, -0.70f, 0.0f);

	glEnd();
	glPopMatrix();
}
void Plane3(void)
{
	glPushMatrix();
	glScalef(0.16f, 0.18f, 0.0f);
	glTranslatef(-9.0f, -2.5f, 0.0f);
	glTranslatef(Move_Lower_Plane_X, Move_Lower_Plane_Y, 0.0f);
	glRotatef(Rotate_Plane_Three, 0.0f, 0.0f, 1.0f);
	//glRotatef(270, 0.0f, 0.0f, 1.0f);
	glColor3f((183.0f / 260.0f), (212.0f / 260.0f), (218.0f / 260.0f));

	glBegin(GL_QUADS);                     //left tail


	glVertex3f(-0.08f, -0.36f, 0.0f);


	glVertex3f(-0.24f, -0.56f, 0.0f);


	glVertex3f(-0.22f, -0.62f, 0.0f);


	glVertex3f(-0.06f, -0.52f, 0.0f);

	//right tail

	glVertex3f(0.08f, -0.36f, 0.0f);


	glVertex3f(0.24f, -0.56f, 0.0f);


	glVertex3f(0.22f, -0.62f, 0.0f);


	glVertex3f(0.06f, -0.52f, 0.0f);

	//middle quad between tails
	glVertex3f(-0.08f, -0.36f, 0.0f);


	glVertex3f(-0.06f, -0.52f, 0.0f);


	glVertex3f(0.06f, -0.52f, 0.0f);


	glVertex3f(0.08f, -0.36f, 0.0f);

	glEnd();

	//tail middle triangle
	glColor3f((111.0f / 260.0f), (111.0f / 260.0f), (119.0f / 260.0f));

	glBegin(GL_TRIANGLES);

	glVertex3f(0.06f, -0.52f, 0.0f);
	glVertex3f(-0.06f, -0.52f, 0.0f);
	glVertex3f(0.0f, -0.6f, 0.0f);

	glEnd();

	//plane middle part quad
	//rgb(101, 170, 199)
	glColor3f((101.0f / 260.0f), (170.0f / 260.0f), (199.0f / 260.0f));
	glBegin(GL_QUADS);

	glVertex3f(0.12f, 0.46f, 0.0f);
	glVertex3f(-0.12f, 0.46f, 0.0f);
	glVertex3f(-0.08f, -0.36f, 0.0f);
	glVertex3f(0.08f, -0.36f, 0.0f);

	//left wing
	//rgb(183, 212, 218)
	glColor3f((183.0f / 260.0f), (212.0f / 260.0f), (218.0f / 260.0f));

	glVertex3f(-0.1f, 0.16f, 0.0f);
	glVertex3f(-0.45f, -0.12f, 0.0f);
	glVertex3f(-0.48f, -0.35f, 0.0f);
	glVertex3f(-0.08f, -0.29f, 0.0f);

	//right wing
	//rgb(183, 212, 218)
	glColor3f((183.0f / 260.0f), (212.0f / 260.0f), (218.0f / 260.0f));

	glVertex3f(0.1f, 0.16f, 0.0f);
	glVertex3f(0.45f, -0.12f, 0.0f);
	glVertex3f(0.48f, -0.35f, 0.0f);
	glVertex3f(0.08f, -0.29f, 0.0f);

	//left wing outer quad
	//rgb(111, 111, 119)
	glColor3f((111.0f / 260.0f), (111.0f / 260.0f), (119.0f / 260.0f));

	glVertex3f(-0.52f, 0.08f, 0.0f);
	glVertex3f(-0.52f, -0.4f, 0.0f);
	glVertex3f(-0.48f, -0.35f, 0.0f);
	glVertex3f(-0.45f, -0.12f, 0.0f);

	//right wing outer quad
	//rgb(111, 111, 119)
	glColor3f((111.0f / 260.0f), (111.0f / 260.0f), (119.0f / 260.0f));

	glVertex3f(0.52f, 0.08f, 0.0f);
	glVertex3f(0.52f, -0.4f, 0.0f);
	glVertex3f(0.48f, -0.35f, 0.0f);
	glVertex3f(0.45f, -0.12f, 0.0f);


	glEnd();

	//upper Triangle

	glBegin(GL_TRIANGLES);

	glVertex3f(-0.06f, 0.46f, 0.0f);
	glVertex3f(0.0f, 0.9f, 0.0f);
	glVertex3f(0.06f, 0.46f, 0.0f);

	glEnd();

	glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glBegin(GL_LINES);  //orange circle                                


	float Radious_Orange = 0.06f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = Radious_Orange * cos(angle) + (0.0f);
		float y = Radious_Orange * sin(angle) + (-0.25f);



		// Circle Origin


		glVertex3f(0.0, -0.25, 0.0f);



		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));
	glBegin(GL_LINES);  //white circle                                


	float Radious_White = 0.04f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = Radious_White * cos(angle) + (0.0f);
		float y = Radious_White * sin(angle) + (-0.25f);



		// Circle Origin


		glVertex3f(0.0, -0.25, 0.0f);



		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));
	glBegin(GL_LINES);  //Green circle                                


	float Radious_Green = 0.02f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = Radious_Green * cos(angle) + (0.0f);
		float y = Radious_Green * sin(angle) + (-0.25f);



		// Circle Origin


		glVertex3f(0.0, -0.25, 0.0f);



		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	//engine fire of plane

	glBegin(GL_QUADS);
	glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(0.13f, -0.62f, 0.0f);
	glVertex3f(-0.13f, -0.62f, 0.0f);
	glVertex3f(-0.13f, -0.64f, 0.0f);
	glVertex3f(0.13f, -0.64f, 0.0f);

	glEnd();

	glBegin(GL_TRIANGLES);

	glVertex3f(-0.0434f, -0.64f, 0.0f);   //right base point
	glVertex3f(-0.13f, -0.64f, 0.0f);     //left base point
	//glVertex3f(-0.08667f, -0.68f, 0.0f);  //apex point
	glVertex3f(-0.05f, -0.70f, 0.0f);  //apex point



	glEnd();
	glBegin(GL_TRIANGLES);

	glVertex3f(0.0432f, -0.64f, 0.0f);   //right base point
	glVertex3f(-0.0434f, -0.64f, 0.0f);     //left base point
	glVertex3f(-0.0001f, -0.9f, 0.0f);  //apex point


	glEnd();

	glBegin(GL_TRIANGLES);

	glVertex3f(0.1298f, -0.64f, 0.0f);   //right base point
	glVertex3f(0.0432f, -0.64f, 0.0f);  //left base point
	//glVertex3f(0.0865, -0.68f, 0.0f);  //apex point
	glVertex3f(0.05f, -0.70f, 0.0f);

	glEnd();


	glPopMatrix();
}


void Parabola(void)
{
	glPushMatrix();
	glTranslatef(-1.0f, 0.2f, 0.0f);
	glBegin(GL_POINTS);

	for (float x = -2.0f; x <= 0.0f; x = x + 0.001f)
	{
		float y = (x * x);
		glVertex2f(x, y);
	}
	glEnd();
	glPopMatrix();
}






