//Windows header files

#include<windows.h>            //For declaration of 3.5 lakh
#include<stdio.h>              // for file I/O
#include<stdlib.h>             // for exit()


#include "OGL.h"            // Header file in same directory

#define _USE_MATH_DEFINES
#include<math.h>



//OpenGL Header Files
#include<gl/GL.h>

//Link with OpenGL Library

#pragma comment(lib,"OpenGL32.lib")

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

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));  // Default Window procedure for all messages.



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

	glClearColor(0.9f, 0.7f, 0.1f, 1.0f);   // First OpenGL Function After bridging APIS









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
	glLineWidth(1);

}

void display(void)
{
	//Function Declarations
	//void Person1(void);
	//void Person2(void);
	//void Person3(void);
	void Person4(void);
	void Person5(void);
	void Person6(void);
	void Flag(void);
	
	

	
	//code
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Function calls

	//Person1();
	//Person2();
	//Person3();

	Flag();
	Person4();
	Person5();
	Person6();
	
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

void Person1(void)
{
	//Local variable declaration

	float radious = 0.0825f;



	glBegin(GL_QUADS);                // Bottom left first person (core body)

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.68f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.82f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.82f, -0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.68f, -0.68f, 0.0f);

	glEnd();

	

	glEnd();

	glBegin(GL_QUADS);                    // Bottomleft first person (left leg)

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.77f, -0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.8f, -0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.8f, -0.9f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.77f, -0.9f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottomleft first person (right leg)

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.71f, -0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.74f, -0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.74f, -0.9f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.71f, -0.9f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottomleft first person (right hand)

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.66f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.69f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.69f, -0.75f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.66f, -0.75f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottomleft first person (left hand horizontal part)

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.82f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.9f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.9f, -0.39f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.82f, -0.39f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottomleft first person (Neck)

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.74f, -0.28f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.77f, -0.28f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.77f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.74f, -0.35f, 0.0f);

	glEnd();

	glBegin(GL_LINES);                                    // Bottomleft first person (face)

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = radious * cos(angle) + (-0.755);
		float y = radious * sin(angle) + (-0.2);






		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-0.755, -0.2, 0.0f);  // centre of face

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glBegin(GL_QUADS);                    // Bottomleft first person (left hand salute part)

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.8f, -0.14f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.9f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.85f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.82f, -0.24f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottomleft first person (left eye)

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.77f, -0.16f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.79f, -0.16f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.79f, -0.19f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.77f, -0.19f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottom left first person (right eye)

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.72f, -0.16f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.74f, -0.16f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.74f, -0.19f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.72f, -0.19f, 0.0f);

	glEnd();

	glBegin(GL_LINES);                  // Bottom left first person lips

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.78f, -0.24f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.73f, -0.24f, 0.0f);

	glEnd();

}

void Person2(void)
{
	//Local variable declaration

	float radious = 0.0825f;

	glBegin(GL_QUADS);                // Bottom left second person (core body)

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.38f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.52f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.52f, -0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.38f, -0.68f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottomleft Second person (left leg)

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.47f, -0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f, -0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f, -0.9f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.47f, -0.9f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottomleft Second person (right leg)

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.41f, -0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.44f, -0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.44f, -0.9f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.41f, -0.9f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottomleft Second person (right hand)

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.36f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.39f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.39f, -0.75f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.36f, -0.75f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottomleft second person (left hand horizontal part)

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.52f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.6f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.6f, -0.39f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.52f, -0.39f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottomleft second person (Neck)

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.44f, -0.28f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.47f, -0.28f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.47f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.44f, -0.35f, 0.0f);

	glEnd();

	glBegin(GL_LINES);                                    // Bottomleft Second person (face)

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = radious * cos(angle) + (-0.455);
		float y = radious * sin(angle) + (-0.2);






		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-0.455, -0.2, 0.0f);  // centre of face

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glBegin(GL_QUADS);                    // Bottomleft Second person (left hand salute part)

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f, -0.14f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.6f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.55f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.52f, -0.24f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottomleft Second person (left eye)

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.47f, -0.16f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.49f, -0.16f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.49f, -0.19f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.47f, -0.19f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottom left Second person (right eye)

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.42f, -0.16f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.44f, -0.16f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.44f, -0.19f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.42f, -0.19f, 0.0f);

	glEnd();

	glBegin(GL_LINES);                  // Bottom left second person lips

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.48f, -0.24f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.43f, -0.24f, 0.0f);

	glEnd();

}

void Person3(void)
{
	//Local variable declaration

	float radious = 0.0825f;

	glBegin(GL_QUADS);                // Bottom Third person (core body)

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.08f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.22f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.22f, -0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.08f, -0.68f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottom Third person (left leg)

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.17f, -0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.2f, -0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.2f, -0.9f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.17f, -0.9f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottom third person (right leg)

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.11f, -0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.14f, -0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.14f, -0.9f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.11f, -0.9f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottom Third person (right hand)

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.06f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.09f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.09f, -0.75f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.06f, -0.75f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottom Third person (left hand horizontal part)

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.22f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.3f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.3f, -0.39f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.22f, -0.39f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottomleft third person (Neck)

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.14f, -0.28f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.17f, -0.28f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.17f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.14f, -0.35f, 0.0f);

	glEnd();

	glBegin(GL_LINES);                                    // Bottom Third person (face)

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = radious * cos(angle) + (-0.155);
		float y = radious * sin(angle) + (-0.2);






		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-0.155, -0.2, 0.0f);  // centre of face

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glBegin(GL_QUADS);                    // Bottomleft third person (left hand salute part)

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.2f, -0.14f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.3f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.25f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.22f, -0.24f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottomleft third person (left eye)

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.17f, -0.16f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.19f, -0.16f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.19f, -0.19f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.17f, -0.19f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottom left Third person (right eye)

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.12f, -0.16f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.14f, -0.16f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.14f, -0.19f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.12f, -0.19f, 0.0f);

	glEnd();

	glBegin(GL_LINES);                  // Bottom left Third person lips

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.18f, -0.24f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.13f, -0.24f, 0.0f);

	glEnd();

}

void Person4(void)
{
	//Local variable declaration

	float radious = 0.0825f;

	glBegin(GL_QUADS);                // Bottom Fourth person (core body)

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.22f, -0.35f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.08f, -0.35f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.08f, -0.68f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.22f, -0.68f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottom Fourth person (left leg)

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.13f, -0.68f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.1f, -0.68f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.1f, -0.86f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.13f, -0.86f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottom fourth person (right leg)

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.19f, -0.68f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.16f, -0.68f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.16f, -0.86f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.19f, -0.86f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                // Bottom Fourth person (Skirt part)

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.22f, -0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.08f, -0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.02f, -0.78f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.27f, -0.78f, 0.0f);

	

	glEnd();



	

	

	//glBegin(GL_QUADS);                    // Bottom fourth person (right hand)
	                                        // code before switching the salute hand.
											// Please ignore this commented code
	//glColor3f(0.0f, 0.0f, 0.0f);
	//glVertex3f(0.24f, -0.35f, 0.0f);

	//glColor3f(0.0f, 0.0f, 0.0f);
	//glVertex3f(0.21f, -0.35f, 0.0f);

	//glColor3f(0.0f, 0.0f, 0.0f);
	//glVertex3f(0.21f, -0.75f, 0.0f);

	//glColor3f(0.0f, 0.0f, 0.0f);
	//glVertex3f(0.24f, -0.75f, 0.0f);

	//glEnd();

	glBegin(GL_QUADS);                    // Bottom fourth person (left hand)

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.08f, -0.35f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.05f, -0.35f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.05f, -0.75f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.08f, -0.75f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottom fourth person (right hand horizontal part)

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.3f, -0.35f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.22f, -0.35f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.22f, -0.39f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.3f, -0.39f, 0.0f);

	glEnd();

	

	glBegin(GL_QUADS);                    // Bottom fourth person (Neck)

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.16f, -0.32f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.13f, -0.32f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.13f, -0.35f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.16f, -0.35f, 0.0f);

	glEnd();

	glLineWidth(1.0f);

	glBegin(GL_LINES);                                    // Bottom fourth person (face)

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = radious * cos(angle) + (0.145);
		float y = radious * sin(angle) + (-0.24);

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.145, -0.2, 0.0f);      // centre of face

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);
    }

	glEnd();

	glBegin(GL_QUADS);                    // Bottomleft  person (right hand salute part)

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.3f, -0.35f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.22f, -0.17f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.22f, -0.27f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.25f, -0.35f, 0.0f);

	glEnd();

	//glBegin(GL_QUADS);                    // Bottom fourth person (left eye)
	                                        // code for left eye.Please ignore this code
	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.13f, -0.16f, 0.0f);

	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.11f, -0.16f, 0.0f);

	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.11f, -0.19f, 0.0f);

	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.13f, -0.19f, 0.0f);

	//glEnd();

	//glBegin(GL_QUADS);                    // Bottom left fourth person (right eye)
											// code for right eye.Please ignore this code

	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.18f, -0.16f, 0.0f);

	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.16f, -0.16f, 0.0f);

	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.16f, -0.19f, 0.0f);

	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.18f, -0.19f, 0.0f);

	//glEnd();

	//glBegin(GL_LINES);                  // Bottom left Third person lips
										  // code for lips.Please ignore this code
	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.12f, -0.24f, 0.0f);

	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.17f, -0.24f, 0.0f);

	//glEnd();

}


void Person5(void)
{
	//Local variable declaration

	float radious = 0.0825f;

	glBegin(GL_QUADS);                // Bottom fifth person (core body)

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.52f, -0.35f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.38f, -0.35f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.38f, -0.68f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.52f, -0.68f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottom fifth person (left leg)

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.43f, -0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.4f, -0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.4f, -0.86f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.43f, -0.86f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottom fifth person (right leg)

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.49f, -0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.46f, -0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.46f, -0.86f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.49f, -0.86f, 0.0f);

	glEnd();

	//glBegin(GL_QUADS);                    // Bottom fifth person (right hand)

	//glColor3f(0.0f, 0.0f, 0.0f);
	//glVertex3f(0.54f, -0.35f, 0.0f);

	//glColor3f(0.0f, 0.0f, 0.0f);
	//glVertex3f(0.51f, -0.35f, 0.0f);

	//glColor3f(0.0f, 0.0f, 0.0f);
	//glVertex3f(0.51f, -0.75f, 0.0f);

	//glColor3f(0.0f, 0.0f, 0.0f);
	//glVertex3f(0.54f, -0.75f, 0.0f);

	//glEnd();

	glBegin(GL_QUADS);                    // Bottom fifth person (left hand)

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.38f, -0.35f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.35f, -0.35f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.35f, -0.75f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.38f, -0.75f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottom fifth person (right hand horizontal part)

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.6f, -0.35f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.52f, -0.35f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.52f, -0.39f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.6f, -0.39f, 0.0f);

	glEnd();



	glBegin(GL_QUADS);                    // Bottom fifth person (Neck)

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.46f, -0.32f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.43f, -0.32f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.43f, -0.35f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.46f, -0.35f, 0.0f);

	glEnd();

	glLineWidth(1.0f);

	glBegin(GL_LINES);                                    // Bottom fifth person (face)

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = radious * cos(angle) + (0.445);
		float y = radious * sin(angle) + (-0.24);

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.445, -0.2, 0.0f);      // centre of face

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);
	}

	glEnd();

	glBegin(GL_QUADS);                    // Bottomleft fifth person (right hand salute part)

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.6f, -0.35f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.52f, -0.17f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.52f, -0.27f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.55f, -0.35f, 0.0f);

	glEnd();

	//glBegin(GL_QUADS);                    // Bottom fifth person (left eye)
											//code for left eye please ignore this code
	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.43f, -0.16f, 0.0f);

	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.41f, -0.16f, 0.0f);

	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.41f, -0.19f, 0.0f);

	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.43f, -0.19f, 0.0f);

	//glEnd();

	//glBegin(GL_QUADS);                    // Bottom left fifth person (right eye)
											//code for right eye please ignore this code
	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.48f, -0.16f, 0.0f);

	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.46f, -0.16f, 0.0f);

	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.46f, -0.19f, 0.0f);

	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.48f, -0.19f, 0.0f);

	//glEnd();

	//glBegin(GL_LINES);                  // Bottom left fifth person lips
										  //Please ignore this code (code for lips)
	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.42f, -0.24f, 0.0f);

	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.47f, -0.24f, 0.0f);

	//glEnd();
}

void Person6(void)
{
	//Local variable declaration

	float radious = 0.0825f;

	glBegin(GL_QUADS);                // Bottom sixth person (core body)

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.82f, -0.35f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.68f, -0.35f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.68f, -0.68f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.82f, -0.68f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottom sixth person (left leg)

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.73f, -0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.7f, -0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.7f, -0.86f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.73f, -0.86f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottom sixth person (right leg)

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.79f, -0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.76f, -0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.76f, -0.86f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.79f, -0.86f, 0.0f);

	glEnd();

	//glBegin(GL_QUADS);                    // Bottom sixth person (right hand)
	                                        // code of right hand before switching the saluting hand
	//glColor3f(0.0f, 0.0f, 0.0f);          //Please ignore this code.
	//glVertex3f(0.84f, -0.35f, 0.0f);

	//glColor3f(0.0f, 0.0f, 0.0f);
	//glVertex3f(0.81f, -0.35f, 0.0f);

	//glColor3f(0.0f, 0.0f, 0.0f);
	//glVertex3f(0.81f, -0.75f, 0.0f);

	//glColor3f(0.0f, 0.0f, 0.0f);
	//glVertex3f(0.84f, -0.75f, 0.0f);

	//glEnd();

	glBegin(GL_QUADS);                    // Bottom sixth person (left hand)

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.68f, -0.35f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.65f, -0.35f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.65f, -0.75f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.68f, -0.75f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // Bottom sixth person (right hand horizontal part)

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.9f, -0.35f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.82f, -0.35f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.82f, -0.39f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.9f, -0.39f, 0.0f);

	glEnd();



	glBegin(GL_QUADS);                    // Bottom sixth person (Neck)

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.76f, -0.32f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.73f, -0.32f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.73f, -0.35f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.5f);
	glVertex3f(0.76f, -0.35f, 0.0f);

	glEnd();

	glLineWidth(1.0f);

	glBegin(GL_LINES);                                    // Bottom sixth person (face)

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = radious * cos(angle) + (0.745);
		float y = radious * sin(angle) + (-0.24);

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.745, -0.2, 0.0f);      // centre of face

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);

    }

	glEnd();

	glBegin(GL_QUADS);                    // Bottomleft sixth person (right hand salute part)

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.9f, -0.35f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.82f, -0.17f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.82f, -0.27f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.85f, -0.35f, 0.0f);

	glEnd();

	//glBegin(GL_QUADS);                    // Bottom sixth person (left eye)
											// code for left eye. Please ignore this code

	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.73f, -0.16f, 0.0f);

	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.71f, -0.16f, 0.0f);

	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.71f, -0.19f, 0.0f);

	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.73f, -0.19f, 0.0f);

	//glEnd();

	//glBegin(GL_QUADS);                    // Bottom left sixth person (right eye)
		                                    // code for right eye. Please ignore this code
	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.78f, -0.16f, 0.0f);

	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.76f, -0.16f, 0.0f);

	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.76f, -0.19f, 0.0f);

	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.78f, -0.19f, 0.0f);

	//glEnd();

	//glBegin(GL_LINES);                  // Bottom left sixth person lips
										  // code for lips.Please ignore this code
	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.72f, -0.24f, 0.0f);

	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.77f, -0.24f, 0.0f);

	//glEnd();


}





void Flag(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);

	glColor3f(0.1f, 0.7f, 0.9f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);

	glColor3f(0.1f, 0.9f, 0.4f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glEnd();



	glBegin(GL_QUADS);

	// 1st rectangle of stair

	glColor3f(0.7f, 0.4f, 0.3f);
	glVertex3f(-0.65f, -0.4f, 0.0f);

	glColor3f(0.7f, 0.4f, 0.3f);
	glVertex3f(-0.65f, -0.5f, 0.0f);


	glColor3f(0.7f, 0.4f, 0.3f);
	glVertex3f(-0.25f, -0.5f, 0.0f);

	glColor3f(0.7f, 0.4f, 0.3f);
	glVertex3f(-0.25f, -0.4f, 0.0f);

	// 2nd rectangle of stair

	glColor3f(0.7f, 0.4f, 0.3f);
	glVertex3f(-0.3f, -0.3f, 0.0f);

	glColor3f(0.7f, 0.4f, 0.3f);
	glVertex3f(-0.6f, -0.3f, 0.0f);

	glColor3f(0.7f, 0.4f, 0.3f);
	glVertex3f(-0.6f, -0.4f, 0.0f);

	glColor3f(0.7f, 0.4f, 0.3f);
	glVertex3f(-0.3f, -0.4f, 0.0f);

	// 3rd rectangle of stair

	glColor3f(0.7f, 0.4f, 0.3f);
	glVertex3f(-0.35f, -0.2f, 0.0f);

	glColor3f(0.7f, 0.4f, 0.3f);
	glVertex3f(-0.55f, -0.2f, 0.0f);

	glColor3f(0.7f, 0.4f, 0.3f);
	glVertex3f(-0.55f, -0.3f, 0.0f);

	glColor3f(0.7f, 0.4f, 0.3f);
	glVertex3f(-0.35f, -0.3f, 0.0f);
	glEnd();

	// pole 

	glLineWidth(10);
	glBegin(GL_LINES);

	glColor3f(0.7f, 0.7f, 0.6f);
	glVertex3f(-0.45f, -0.2f, 0.0f);

	glColor3f(0.7f, 0.7f, 0.6f);
	glVertex3f(-0.45f, 0.8f, 0.0f);
	glEnd();

	// flag rectangle - orange
	glBegin(GL_QUADS);

	glColor3f(1.0f, 0.6f, 0.1f);
	glVertex3f(-0.1f, 0.8f, 0.0f);

	glColor3f(1.0f, 0.6f, 0.1f);
	glVertex3f(-0.44f, 0.8f, 0.0f);

	glColor3f(1.0f, 0.6f, 0.1f);
	glVertex3f(-0.44f, 0.7f, 0.0f);

	glColor3f(1.0f, 0.6f, 0.1f);
	glVertex3f(-0.1f, 0.7f, 0.0f);

	//  flag rectangle -white coolur

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.1f, 0.7f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.44f, 0.7f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.44f, 0.6f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.1f, 0.6f, 0.0f);

	// flag rectangle - green colur 

	glColor3f(0.3f, 0.6f, 0.1f);
	glVertex3f(-0.1f, 0.6f, 0.0f);

	glColor3f(0.3f, 0.6f, 0.1f);
	glVertex3f(-0.44f, 0.6f, 0.0f);

	glColor3f(0.3f, 0.6f, 0.1f);
	glVertex3f(-0.44f, 0.5f, 0.0f);

	glColor3f(0.3f, 0.6f, 0.1f);
	glVertex3f(-0.1f, 0.5f, 0.0f);
	glEnd();

	// ashokchakra

	

	glLineWidth(1.0f);		
	//glBegin(GL_LINES);                // Ashokchakra spokes code
	float radius = 0.040f;
	for (float angle = 0.0; angle < 360.0f; angle = angle + 15.0f)
	{
		glBegin(GL_LINES);
		float x1 = radius * cos((angle*M_PI)/180.0f)  + (-0.27f);
		float y1 = radius * sin((angle*M_PI)/180.0f) + (0.65f);

		

		// Circle centre
		glColor3f(0.0f, 0.0f, 0.7f); 			//Blue color	
		glVertex3f(-0.27f, 0.65f, 0.0f);

		glColor3f(0.0f, 0.0f, 0.7f);
		glVertex3f(x1, y1, 0.0f);

		glEnd();
		float x2, y2;

		if (angle < 180.0f)   //code for dots on ashokchakra
		{
			 x2 = (radius * cos((angle * M_PI) / 180.0f) + (-0.27f) + radius * cos(((angle + 15) * M_PI) / 180.0f) + (-0.27f)) / 2.0f;
			 y2 = (radius * sin((angle * M_PI) / 180.0f) + (0.649f) + radius * sin(((angle + 15) * M_PI) / 180.0f) + (0.649f)) / 2.0f;
		}

		else
		{
			 x2 = (radius * cos((angle * M_PI) / 180.0f) + (-0.27f) + radius * cos(((angle + 15) * M_PI) / 180.0f) + (-0.27f)) / 2.0f;
			 y2 = (radius * sin((angle * M_PI) / 180.0f) + (0.651f) + radius * sin(((angle + 15) * M_PI) / 180.0f) + (0.651f)) / 2.0f;
		}

		

		glEnable(GL_POINT_SMOOTH);
		glPointSize(3.0f);

		glBegin(GL_POINTS);
		glColor3f(0.0f, 0.0f, (128.0f / 255.0f));
		/*glColor3f(1.0f, 0.0f, 0.0f);*/

		glVertex3f(x2, y2, 0.0f);
		glEnd();

	}
	//glEnd();

	glEnable(GL_POINT_SMOOTH);

	glPointSize(10.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.27f, 0.65f, 0.0f);
	glEnd();

	//glEnable(GL_POINT_SMOOTH);

	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3f(0.0f, 0.0f, (128.0f / 255.0f));
	/*glColor3f(1.0f, 0.0f, 0.0f);*/

	glVertex3f(-0.27f, 0.65f, 0.0f);
	glEnd();

	glPointSize(1.0f);


	glLineWidth(2.0f);
	glBegin(GL_POINTS);                          // Ashokchakra circumference code
	//float radius = 0.040f;
	for (float angle = 0.0f; angle < 360.0f; angle = angle + 0.1f)
	{
		float x1 = radius * cos(angle) + (-0.27f);
		float y1 = radius * sin(angle) + (0.65f);

		//y1 = y1 + 0.1;

		// Circle Origin
		glColor3f(0.0f, 0.0f, 0.7f); 			//Blue color	
		//glVertex3f(-0.27f, 0.65f, 0.0f);

		glColor3f(0.0f, 0.0f, 0.7f);
		glVertex3f(x1, y1, 0.0f);
	}
	glEnd();


	// ********************************* code for ashok chakra using glLines by manually taking the coordinates ************************************
	//Please ignore below code

	// lines of ashokchakra

	/*glLineWidth(2);
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.5f);
	glVertex3f(-0.27f, 0.69f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.5f);
	glVertex3f(-0.27f, 0.61f, 0.0f);

	// 2nd chakra line

	glColor3f(0.0f, 0.0f, 0.5f);
	glVertex3f(-0.31f, 0.65f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.5f);
	glVertex3f(-0.23f, 0.65f, 0.0f);


	// 3d  line

	glColor3f(0.0f, 0.0f, 0.5f);
	glVertex3f(-0.30f, 0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.5f);
	glVertex3f(-0.24f, 0.62f, 0.0f);


	// 4th  line

	glColor3f(0.0f, 0.0f, 0.5f);
	glVertex3f(-0.24f, 0.68f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.5f);
	glVertex3f(-0.30f, 0.62f, 0.0f);

	// 5th line 

	glColor3f(0.0f, 0.0f, 0.5f);
	glVertex3f(-0.285, 0.69f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.5f);
	glVertex3f(-0.255f, 0.61f, 0.0f);

	// 6th line 

	glColor3f(0.0f, 0.0f, 0.5f);
	glVertex3f(-0.255, 0.69f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.5f);
	glVertex3f(-0.285f, 0.61f, 0.0f);

	// 7th line 

	glColor3f(0.0f, 0.0f, 0.5f);
	glVertex3f(-0.305, 0.665f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.5f);
	glVertex3f(-0.23f, 0.63f, 0.0f);

	// 8th line

	glColor3f(0.0f, 0.0f, 0.5f);
	glVertex3f(-0.23, 0.67f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.5f);
	glVertex3f(-0.31f, 0.63f, 0.0f);


	glEnd();*/
}







