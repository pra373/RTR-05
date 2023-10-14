//Windows header files

#include<windows.h>            //For declaration of 3.5 lakh API
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

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // First OpenGL Function After bridging APIS









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

}

void display(void)
{   
	//local function declaration
	void ParkInstrument2(void);

	//code
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	ParkInstrument2();

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

void ParkInstrument2(void)
{
	//local variable declaration
	float radius1 = 0.025f;

	glLineWidth(15.0f);
	glBegin(GL_LINES);               // Horizontal line right part

	glColor3f(0.6f, 0.1f, 0.5f);
	glVertex3f(0.0f, 0.25f, 0.0f);

	glColor3f(0.6f, 0.1f, 0.5f);
	glVertex3f(0.5f, 0.25f, 0.0f);


	glEnd();
	glLineWidth(17.0f);
	glBegin(GL_LINES);                //slant line right

	glColor3f(0.6f, 0.1f, 0.5f);
	glVertex3f(0.5f, 0.25f, 0.0f);

	glColor3f(0.6f, 0.1f, 0.5f);
	glVertex3f(0.7f, -0.3f, 0.0f);


	glEnd();

	glBegin(GL_LINES);                   //slant line left

	glColor3f(0.6f, 0.1f, 0.5f);
	glVertex3f(0.3f, 0.25f, 0.0f);

	glColor3f(0.6f, 0.1f, 0.5f);
	glVertex3f(0.5f, -0.3f, 0.0f);


	glEnd();

	glLineWidth(15.0f);
	glBegin(GL_LINES);                     //Horizontal line left part

	glColor3f(0.6f, 0.1f, 0.5f);
	glVertex3f(0.0f, 0.25f, 0.0f);

	glColor3f(0.6f, 0.1f, 0.5f);
	glVertex3f(-0.3f, 0.25f, 0.0f);


	glEnd();

	glLineWidth(1.0f);

	glBegin(GL_LINES);                   // left part horizontal net line 1

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(-0.3f, 0.2f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(0.0f, 0.2f, 0.0f);


	glEnd();

	glLineWidth(1.0f);

	glBegin(GL_LINES);                      // left part horizontal net line 2

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(-0.3f, 0.15f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(0.0f, 0.15f, 0.0f);


	glEnd();

	glBegin(GL_LINES);                        // left part horizontal net line 3

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(-0.3f, 0.10f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(0.0f, 0.10f, 0.0f);


	glEnd();

	glBegin(GL_LINES);                       // left part horizontal net line 4

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(-0.3f, 0.10f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(0.0f, 0.10f, 0.0f);


	glEnd();


	glBegin(GL_LINES);                         // left part horizontal net line 5

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(-0.3f, 0.05f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(0.0f, 0.05f, 0.0f);


	glEnd();

	glBegin(GL_LINES);                        // left part horizontal net line 6

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(-0.3f, 0.0f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(0.0f, 0.0f, 0.0f);


	glEnd();

	glBegin(GL_LINES);                           // left part horizontal net line 7


	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(-0.3f, -0.05f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(0.0f, -0.05f, 0.0f);


	glEnd();

	glBegin(GL_LINES);                              // left part horizontal net line 8
	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(-0.3f, -0.1f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(0.0f, -0.1f, 0.0f);


	glEnd();

	glBegin(GL_LINES);                            // left part horizontal net line 9

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(-0.3f, -0.15f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(0.0f, -0.15f, 0.0f);


	glEnd();

	glBegin(GL_LINES);                           // left part horizontal net line 10

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(-0.3f, -0.2f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(0.0f, -0.2f, 0.0f);


	glEnd();

	glBegin(GL_LINES);                      // left part horizontal net line 11

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(-0.3f, -0.25f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(0.0f, -0.25f, 0.0f);


	glEnd();
	glLineWidth(15.0f);
	glBegin(GL_LINES);                 // Vertical bar left part

	glColor3f(0.6f, 0.1f, 0.5f);
	glVertex3f(-0.3f, 0.25f, 0.0f);

	glColor3f(0.6f, 0.1f, 0.5f);
	glVertex3f(-0.3f, -0.3f, 0.0f);


	glEnd();
	

	glLineWidth(1.0f);
	glBegin(GL_LINES);                 // Vertical net line 1

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(-0.25f, 0.25f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(-0.25f, -0.3f, 0.0f);


	glEnd();

	glBegin(GL_LINES);                 // Vertical net line 2

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(-0.2f, 0.25f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(-0.2f, -0.3f, 0.0f);


	glEnd();

	glBegin(GL_LINES);                 // Vertical net line 3

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(-0.15f, 0.25f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(-0.15f, -0.3f, 0.0f);


	glEnd();

	glBegin(GL_LINES);                 // Vertical net line 4

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(-0.1f, 0.25f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(-0.1f, -0.3f, 0.0f);


	glEnd();

	glBegin(GL_LINES);                 // Vertical net line 5

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(-0.05f, 0.25f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.2f);
	glVertex3f(-0.05f, -0.3f, 0.0f);


	glEnd();

	// code for net dots

	glEnable(GL_POINT_SMOOTH);

	glPointSize(5.0f);

	float y = 0.2f;
	for (float x = -0.25f; x < 0.0f; x = x + 0.05f)        // for loop for dots of first horizontal line
	{
		glBegin(GL_POINTS);
		glColor3f(0.4f, 1.0f, 0.0f);
		glVertex3f(x, y, 0.0f);
		glEnd();
	}
	y = y - 0.05f;      // decrementing y axis to go to next horizontal line

	for (float x = -0.25f; x < 0.0f; x = x + 0.05f)        // for loop for dots of second horizontal line
	{
		glBegin(GL_POINTS);
		glColor3f(0.4f, 1.0f, 0.0f);
		glVertex3f(x, y, 0.0f);
		glEnd();
	}

	y = y - 0.05f;

	for (float x = -0.25f; x < 0.0f; x = x + 0.05f)        // for loop for dots of third horizontal line
	{
		glBegin(GL_POINTS);
		glColor3f(0.4f, 1.0f, 0.0f);
		glVertex3f(x, y, 0.0f);
		glEnd();
	}

	y = y - 0.05f;

	for (float x = -0.25f; x < 0.0f; x = x + 0.05f)        // for loop for dots of forth horizontal line
	{
		glBegin(GL_POINTS);
		glColor3f(0.4f, 1.0f, 0.0f);
		glVertex3f(x, y, 0.0f);
		glEnd();
	}

	y = y - 0.05f;

	for (float x = -0.25f; x < 0.0f; x = x + 0.05f)        // for loop for dots of fifth horizontal line
	{
		glBegin(GL_POINTS);
		glColor3f(0.4f, 1.0f, 0.0f);
		glVertex3f(x, y, 0.0f);
		glEnd();
	}

	y = y - 0.05f;

	for (float x = -0.25f; x < 0.0f; x = x + 0.05f)        // for loop for dots of sixth horizontal line
	{
		glBegin(GL_POINTS);
		glColor3f(0.4f, 1.0f, 0.0f);
		glVertex3f(x, y, 0.0f);
		glEnd();
	}

	y = y - 0.05f;

	for (float x = -0.25f; x < 0.0f; x = x + 0.05f)        // for loop for dots of seventh horizontal line
	{
		glBegin(GL_POINTS);
		glColor3f(0.4f, 1.0f, 0.0f);
		glVertex3f(x, y, 0.0f);
		glEnd();
	}

	y = y - 0.05f;

	for (float x = -0.25f; x < 0.0f; x = x + 0.05f)        // for loop for dots of eighth horizontal line
	{
		glBegin(GL_POINTS);
		glColor3f(0.4f, 1.0f, 0.0f);
		glVertex3f(x, y, 0.0f);
		glEnd();
	}

	y = y - 0.05f;

	for (float x = -0.25f; x < 0.0f; x = x + 0.05f)        // for loop for dots of ninth horizontal line
	{
		glBegin(GL_POINTS);
		glColor3f(0.4f, 1.0f, 0.0f);
		glVertex3f(x, y, 0.0f);
		glEnd();
	}

	y = y - 0.05f;

	for (float x = -0.25f; x < 0.0f; x = x + 0.05f)        // for loop for dots of tenth horizontal line
	{
		glBegin(GL_POINTS);
		glColor3f(0.4f, 1.0f, 0.0f);
		glVertex3f(x, y, 0.0f);
		glEnd();
	}

	glLineWidth(15.0f);
	glBegin(GL_LINES);                 // Vertical middle bar

	glColor3f(0.6f, 0.1f, 0.5f);
	glVertex3f(0.0f, 0.25f, 0.0f);

	glColor3f(0.6f, 0.1f, 0.5f);
	glVertex3f(0.0f, -0.3f, 0.0f);


	glEnd();

	glLineWidth(5.0f);

	glBegin(GL_LINES);                 // First horizontal line of ladder

	glColor3f(1.0f, 0.8f, 0.0f);
	glVertex3f(0.33f, 0.15f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.0f);
	glVertex3f(0.53f, 0.15f, 0.0f);

	glEnd();

	glBegin(GL_LINES);                 // second horizontal line of ladder

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.37f, 0.05f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.57f, 0.05f, 0.0f);

	glEnd();

	glBegin(GL_LINES);                 // third horizontal line of ladder

	glColor3f(0.2f, 0.8f, 1.0f);
	glVertex3f(0.41f, -0.05f, 0.0f);

	glColor3f(0.2f, 0.8f, 1.0f);
	glVertex3f(0.61f, -0.05f, 0.0f);

	glEnd();

	glBegin(GL_LINES);                 // fourth horizontal line of ladder

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.45f, -0.15f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.65f, -0.15f, 0.0f);

	glEnd();

	glBegin(GL_LINES);                 // Roman rings left rope

	glColor3f(1.0f, 0.9f, 0.0f);
	glVertex3f(0.1f, 0.25f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.0f);
	glVertex3f(0.1f, -0.05f, 0.0f);

	glEnd();

	glBegin(GL_LINES);                 // Roman rings right rope

	glColor3f(1.0f, 0.9f, 0.0f);
	glVertex3f(0.2f, 0.25f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.0f);
	glVertex3f(0.2f, -0.05f, 0.0f);

	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_POINTS);                          // Roman rings left rope circle

	//float radius = 0.040f;
	for (float angle = 0.0f; angle < 360.0f; angle = angle + 0.1f)
	{
		float x1 = radius1 * cos((angle * M_PI) / 180.0f) + (0.1f);
		float y1 = radius1 * sin((angle * M_PI) / 180.0f) + (-0.075f);



		// Circle Origin
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.1f, -0.075f, 0.0f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(x1, y1, 0.0f);
	}
	glEnd();

	glBegin(GL_POINTS);                          // Roman rings left rope circle

	//float radius = 0.040f;
	for (float angle = 0.0f; angle < 360.0f; angle = angle + 0.1f)
	{
		float x1 = radius1 * cos((angle * M_PI) / 180.0f) + (0.2f);
		float y1 = radius1 * sin((angle * M_PI) / 180.0f) + (-0.075f);



		// Circle Origin
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.2f, -0.075f, 0.0f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(x1, y1, 0.0f);
	}
	glEnd();


}







