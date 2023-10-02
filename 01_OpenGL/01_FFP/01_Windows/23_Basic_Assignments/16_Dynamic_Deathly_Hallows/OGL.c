//Windows header files

#include<windows.h>            //For declaration of 3.5 lakh API
#include<stdio.h>              // for file I/O
#include<stdlib.h>             // for exit()

#include "OGL.h"            // Header file in same directory
#define _USE_MATH_DEFINES
#include<math.h>

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

// Global Function declarations

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations

FILE *gpFile = NULL;           // (gpFile) global pointer to a file

HWND ghwnd = NULL;             // handle of window
BOOL gbActive = FALSE;
DWORD dwStyle = 0;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) }; 
BOOL gbFullScreen = FALSE;

GLfloat Angle_Rotate_Triangle = 0.0f;
GLfloat Angle_Rotate_Circle = 0.0f;
GLfloat Translate_Line_Y = 4.0f;
GLfloat Translate_Triangle_X = -4.5f;
GLfloat Translate_Triangle_Y = -3.0f;
GLfloat Translate_Circle_X = 4.5f;
GLfloat Translate_Circle_Y = -3.0f;


//OpenGL Related Global variables

HDC ghdc = NULL;
HGLRC ghrc = NULL;

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

	

	
	PlaySound(MAKEINTRESOURCE(MySound), NULL, SND_RESOURCE | SND_ASYNC);
	// Game loop
	

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
	// Tell WGL bridging library to give OpenGL Device COntect from this device context.

	// Create Opengl contect from Device context.

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
	void DrawTriangle(void);
	void DrawIncircle(void);
	void DrawAltitude(void);
	
	//code
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glTranslatef(Translate_Triangle_X, Translate_Triangle_Y, -6.0f);
	glRotatef(Angle_Rotate_Triangle, 0.0f, 1.0f, 0.0f);


	DrawTriangle();
	//DrawIncircle();
	//DrawAltitude();

	glLoadIdentity();
	glTranslatef(Translate_Circle_X, Translate_Circle_Y, -6.0f);
	glRotatef(Angle_Rotate_Circle, 0.0f, 1.0f, 0.0f);
	DrawIncircle();

	glLoadIdentity();
	glTranslatef(0.0f, Translate_Line_Y, -6.0f);
	DrawAltitude();





	

	
	

	SwapBuffers(ghdc);


}

void update(void)
{
	if (Angle_Rotate_Triangle <= 360.0f)
	{
		Angle_Rotate_Triangle = Angle_Rotate_Triangle + 0.1f;
	}
	else
	{
		Angle_Rotate_Triangle = 0.0f;
	}

	if (Angle_Rotate_Circle <= 360.0f)
	{
		Angle_Rotate_Circle = Angle_Rotate_Circle + 0.1f;
	}
	else
	{
		Angle_Rotate_Circle = 0.0f;
	}

	if (Translate_Line_Y > 0.0f)
	{
		/*Translate_Line_Y = Translate_Line_Y - 0.00044f;*/

		Translate_Line_Y = Translate_Line_Y - 0.00044f;
	}

	if (Translate_Triangle_X < 0.0f )
	{
		/*Translate_Triangle_X = Translate_Triangle_X + 0.0005;
		Translate_Triangle_Y = Translate_Triangle_Y + 0.000335;*/

		Translate_Triangle_X = Translate_Triangle_X + 0.0005;
		Translate_Triangle_Y = Translate_Triangle_Y + 0.000335;
	}

	if (Translate_Circle_X > 0.0f)
	{
		/*Translate_Circle_X = Translate_Circle_X - 0.0005;
		Translate_Circle_Y = Translate_Circle_Y + 0.000335;*/

		Translate_Circle_X = Translate_Circle_X - 0.0005;
		Translate_Circle_Y = Translate_Circle_Y + 0.000335;
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

void DrawTriangle(void)
{
	glBegin(GL_LINE_STRIP);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);


	glVertex3f(-1.0f, -1.0f, 0.0f);


	glVertex3f(1.0f, -1.0f, 0.0f);

	glVertex3f(0.0f, 1.0f, 0.0f);

	glEnd();

}

void DrawIncircle(void)
{

	GLdouble x1 = 0.0f;
	GLdouble x2 = -1.0f;
	GLdouble x3 = 1.0f;

	GLdouble y1 = 1.0f;
	GLdouble y2 = -1.0f;
	GLdouble y3 = -1.0f;

	GLdouble Semiperimeter;
	GLdouble Area;
	GLdouble Height;
	GLdouble Inradious;
	GLdouble Incenter_X;
	GLdouble Incenter_Y;

	/*GLdouble Length_A = sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
	GLdouble Length_B = sqrt(((x3 - x2) * (x3 - x2)) + ((y3 - y2) * (y3 - y2)));
	GLdouble Length_C = sqrt(((x3 - x1) * (x3 - x1)) + ((y3 - y1) * (y3 - y1)));*/


	GLdouble Length_A = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
	GLdouble Length_B = sqrt(pow((x3 - x2), 2) + pow((y3 - y2), 2));
	GLdouble Length_C = sqrt(pow((x3 - x1), 2) + pow((y3 - y1), 2));

	//calculate semiperimeter

	Semiperimeter = (Length_A + Length_B + Length_C) / 2.0000000000;

	//calculate area of triangle using heron's formula.

	Area = sqrt(Semiperimeter * (Semiperimeter - Length_A) * (Semiperimeter - Length_B) * (Semiperimeter - Length_C));

	//calculating height of triangle

	Height = Area / ((0.500000000) * Length_B);

	//calculating Inradious
	Inradious = Area / Semiperimeter;

	//Calculating x coordinate of Incentre

	Incenter_X = (((Length_A * x1) + (Length_B * x2) + (Length_C * x3)) / (Length_A + Length_B + Length_C));
	Incenter_Y = (((Length_A * y1) + (Length_B * y2) + (Length_C * y3)) / (Length_A + Length_B + Length_C));


	fprintf(gpFile, "Length of A = %lf, Length of B = %lf, Length of C = %lf\n\n", Length_A, Length_B, Length_C);
	fprintf(gpFile, "Semiperimeter = %lf\n\n", Semiperimeter);
	fprintf(gpFile, "Area = %lf\n\n", Area);
	fprintf(gpFile, "height = %lf\n\n", Height);
	fprintf(gpFile, "Inradious = %lf\n\n", Inradious);
	fprintf(gpFile, "InCenter X Coordinate = %f\n\n", floor(Incenter_X));
	fprintf(gpFile, "InCenter Y Coordinate = %f\n\n", floor(Incenter_Y));

	glBegin(GL_POINTS);             //incircle 

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		/*float x = Inradious * cos((angle * M_PI) / 180.0f) + (0.0f);
		float y = Inradious * sin((angle * M_PI) / 180.0f) + (0.38f);*/
		float x = Inradious * cos(angle) + (floor(Incenter_X));
		float y = Inradious * sin(angle) + (Incenter_Y - 0.07);


		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(floor(Incenter_X), (Incenter_Y - 0.07), 0.0f);


		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(x, y, 0.0f);




	}

	glEnd();

	//double Inradious = 0.618033988;
	//glBegin(GL_POINTS);             //incircle starts

	//for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	//{

	//	//float radianangle = angle * (M_PI * 180.0f);
	//	float x = Inradious * cos((angle * M_PI) / 180.0f) + (0.0f);
	//	float y = Inradious * sin((angle * M_PI) / 180.0f) + (-0.38f);



	//	glColor3f(1.0f, 1.0f, 1.0f);
	//	glVertex3f(0.0f, -0.38f, 0.0f);


	//	glColor3f(1.0f, 1.0f, 1.0f);
	//	glVertex3f(x, y, 0.0f);




	//}

	//glEnd();
}

void DrawAltitude(void)
{
	glBegin(GL_LINES);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);

	glEnd();

	
}











