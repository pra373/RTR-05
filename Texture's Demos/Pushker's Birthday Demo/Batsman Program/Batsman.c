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
	//Local Function Declarations

	void Core_Body(void);
	void Left_Leg(void);
	void Right_Leg(void);
	void Left_Hand(void);
	void Right_Hand(void);
	void LeftHand_Glov(void);
	void RightHand_Glov(void);
	void Right_Shoe(void);
	void Left_Shoe(void);
	void Face(void);
	void Cap(void);
	void Bat(void);
	void Neck(void);


	

	//code

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glTranslatef(0.0f, 0.0f, -3.0f);

	Core_Body();

	Left_Leg();

	Right_Leg();

	Left_Hand();

	Right_Hand();

	Left_Shoe();

	Right_Shoe();

	LeftHand_Glov();

	Bat();

	RightHand_Glov();

	Neck();

	Face();

	Cap();



	SwapBuffers(ghdc);       
}

void update(void)
{
	//code

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

void Core_Body(void)
{
	glBegin(GL_QUADS);                //core body

	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(0.045f, 0.56f, 0.0f);

	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(-0.11f, 0.03f, 0.0f);

	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(0.16f, -0.05f, 0.0f);

	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(0.265f, 0.37f, 0.0f);

	glEnd();
}

void Left_Leg(void)
{
	glBegin(GL_QUAD_STRIP);                //Left leg

	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex3f(-0.11f, 0.03f, 0.0f);

	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex3f(0.16f, -0.05f, 0.0f);

	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex3f(-0.09f, -0.22f, 0.0f);

	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex3f(0.056f, -0.22f, 0.0f);

	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex3f(-0.12f, -0.44f, 0.0f);

	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex3f(-0.022f, -0.485f, 0.0f);

	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex3f(-0.33f, -0.81f, 0.0f);

	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex3f(-0.25f, -0.84f, 0.0f);


	glEnd();
}

void Right_Leg(void)
{
	glBegin(GL_QUAD_STRIP);               //right leg

	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex3f(-0.11f, 0.03f, 0.0f);

	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex3f(0.16f, -0.05f, 0.0f);

	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex3f(0.235f, -0.41f, 0.0f);

	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex3f(0.37f, -0.37f, 0.0f);

	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex3f(0.23f, -0.86f, 0.0f);

	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex3f(0.32f, -0.86f, 0.0f);


	glEnd();

}

void Left_Hand(void)
{
	glBegin(GL_QUADS);               //left hand shoulder part

	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex3f(-0.0005f, 0.405f, 0.0f);

	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex3f(0.045f, 0.56f, 0.0f);

	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex3f(-0.1f, 0.62f, 0.0f);

	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex3f(-0.14f, 0.49f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);               //left hand elbow part

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(-0.14f, 0.49f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(-0.1f, 0.62f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(-0.19f, 0.65f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(-0.19f, 0.49f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);               //left hand wrist part

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(-0.14f, 0.49f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(-0.19f, 0.49f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(-0.17f, 0.34f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(-0.13f, 0.34f, 0.0f);

	glEnd();
}

void Right_Hand(void)
{
	glBegin(GL_POLYGON);               //right hand shoulder part

	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex3f(0.252f, 0.252f, 0.0f);

	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex3f(0.265f, 0.37f, 0.0f);

	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex3f(0.21f, 0.3f, 0.0f);

	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex3f(0.15f, 0.152f, 0.0f);

	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex3f(0.22f, 0.1f, 0.0f);

	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex3f(0.252f, 0.252f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);               //right hand elbow part

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.15f, 0.152f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.14f, 0.14f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.152f, 0.01f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.22f, 0.1f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);               //right hand wrist part

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.14f, 0.14f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(-0.04f, 0.151f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(-0.015f, 0.07f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.152f, 0.01f, 0.0f);

	glEnd();
}

void Left_Shoe(void)
{
	glBegin(GL_POLYGON);               //Left shoe

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.33f, -0.81f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.375f, -0.875f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.32f, -0.95f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.235f, -0.98f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.16f, -0.98f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.16, -0.96f, 0.0f);



	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.18f, -0.935f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.255f, -0.9f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.25f, -0.84f, 0.0f);

	glEnd();

}

void Right_Shoe(void)
{
	glBegin(GL_POLYGON);               //right shoe

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(0.23f, -0.86f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(0.22f, -0.955f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(0.44f, -0.97f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(0.44f, -0.935f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(0.32f, -0.86f, 0.0f);


	glEnd();
}

void LeftHand_Glov(void)
{
	glBegin(GL_POLYGON);               //left hand glove Thumb part

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.17f, 0.34f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.16f, 0.28f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.15f, 0.215f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.125f, 0.21f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.12f, 0.3f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.13f, 0.34f, 0.0f);


	glEnd();

	glBegin(GL_POLYGON);               //left hand glove other fingers part

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.17f, 0.34f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.24f, 0.22f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.155f, 0.12f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.124f, 0.15f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.17f, 0.21f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.16f, 0.28f, 0.0f);







	glEnd();

}

void Bat(void)
{
	glBegin(GL_QUADS);               //Bat handle

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.19f, 0.28f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.21f, 0.255f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.05f, 0.03f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.034f, 0.05f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);               //Bat handle (between handle and rectagular bat)

	glColor3f(1.0f, 0.9f, 0.6f);
	glVertex3f(-0.21f, 0.255f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.6f);
	glVertex3f(-0.19f, 0.28f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.6f);
	glVertex3f(-0.182f, 0.318f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.6f);
	glVertex3f(-0.236f, 0.254f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);               //Bat handle (Willow part)

	glColor3f(1.0f, 0.9f, 0.6f);
	glVertex3f(-0.182f, 0.318f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.6f);
	glVertex3f(-0.51f, 0.75f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.6f);
	glVertex3f(-0.57f, 0.695f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.6f);
	glVertex3f(-0.236f, 0.254f, 0.0f);

	glEnd();

	glBegin(GL_TRIANGLES);               //Bat triangle (3D effect)

	glColor3f(1.0f, 0.9f, 0.6f);
	glVertex3f(-0.51f, 0.75f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.6f);
	glVertex3f(-0.5f, 0.76f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.6f);
	glVertex3f(-0.33f, 0.51f, 0.0f);



	glEnd();

	glBegin(GL_TRIANGLES);               //Bat triangle (3D effect)

	glColor3f(1.0f, 0.3f, 0.1f);
	glVertex3f(-0.51f, 0.75f, 0.0f);

	glColor3f(1.0f, 0.3f, 0.1f);
	glVertex3f(-0.5f, 0.76f, 0.0f);

	glColor3f(1.0f, 0.3f, 0.1f);
	glVertex3f(-0.57f, 0.695f, 0.0f);



	glEnd();
}

void RightHand_Glov(void)
{
	glBegin(GL_POLYGON);               //right hand glove

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.04f, 0.151f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.105f, 0.185f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.096f, 0.16f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.17f, 0.115f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.09f, 0.02f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(-0.015f, 0.07f, 0.0f);


	glEnd();
}

void Neck(void)
{
	glBegin(GL_QUADS);               //Nech upper part

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.132f, 0.48f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.218f, 0.412f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.26f, 0.46f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.16f, 0.55f, 0.0f);

	glEnd();
}

void Face(void)
{
	glBegin(GL_POLYGON);               //face

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.26f, 0.46f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.3f, 0.435f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.32f, 0.44f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.34f, 0.53f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.365f, 0.53f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.355f, 0.575f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.355f, 0.59f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.36f, 0.61f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.36f, 0.64f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.158f, 0.64f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.162f, 0.57f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.16f, 0.55f, 0.0f);

	glEnd();

	glBegin(GL_LINES);             // Human eye   

	float radious_eye = 0.007f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = radious_eye * cos((angle * M_PI) / 180.0f) + (0.335f);
		float y = radious_eye * sin((angle * M_PI) / 180.0f) + (0.58f);



		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.335f, 0.58f, 0.0f);


		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);




	}

	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);      // Eyebrows

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.354f, 0.61f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.334f, 0.62f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.334f, 0.62f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.32f, 0.61f, 0.0f);

	glEnd();


}

void Cap(void)
{
	glBegin(GL_LINES);             // Semicircle for cap    

	float radious_cap = 0.113f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = radious_cap * cos((angle * M_PI) / 180.0f) + (0.26f);
		float y = radious_cap * sin((angle * M_PI) / 180.0f) + (0.64f);

		if (angle <= 180.0f)
		{
			glColor3f(0.0f, 0.2f, 0.5f);

			glVertex3f(0.26f, 0.64f, 0.0f);


			glColor3f(0.0f, 0.2f, 0.5f);

			glVertex3f(x, y, 0.0f);
		}



	}

	glEnd();

	glLineWidth(4.0f);    //cap horozontal line

	glBegin(GL_LINES);

	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex3f(0.36f, 0.64f, 0.0f);

	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex3f(0.42, 0.64, 0.0f);

	glEnd();
	glLineWidth(1.0f);




}






