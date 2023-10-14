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

GLfloat Translate_Line1_Y = 0.35f;
GLfloat Translate_Line2_Y = 0.0f;
GLfloat Translate_Line3_Y = 0.0f;
GLfloat Translate_Line4_Y = 0.0f;

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

	glClearColor(1.0f, 1.0f, 0.97f, 1.0f);   // First OpenGL Function After bridging APIS
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
	void Cooking_Girl(void);
	void Gas(void);
	void SteamLine1(void);
	void SteamLine2(void);
	void SteamLine3(void);
	void SteamLine4(void);
	void SteamLine5(void);
	void Fridge(void);
	//code
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -1.1f);
	glBegin(GL_QUADS);                    // floor quad

	glColor3f(0.7f, 0.3f, 0.1f);
	glVertex3f(1.0f, -0.5f, 0.0f);

	//glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(1.0f, -0.3f, 0.0f);

	//glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -0.3f, 0.0f);

	//glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -0.5f, 0.0f);

	glEnd();

	//glTranslatef(0.0f, 0.0f, -1.0f);
	/*glBegin(GL_LINES);
	
	glLineWidth(10.0f);
	glColor3f(0.9f, 0.6f, 0.4f);
	glVertex3f(1.0f, -0.3f, 0.0f);
	glVertex3f(-1.0f, -0.3f, 0.0f);

	glEnd();*/

	
	
	

	

	
	glTranslatef(0.0f, 0.0f, -0.4f);
	glBegin(GL_QUADS);                    // table lower quad

	glColor3f(0.66f, 0.66f, 0.66f);
	glVertex3f(-0.36f, -0.21f, 0.0f);

	//glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(-0.99f, -0.21f, 0.0f);

	//glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(-0.99f, -0.86f, 0.0f);

	//glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(-0.36f, -0.86f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                 // table shadow part

	glColor3f(0.39f, 0.39f, 0.39f);
	glVertex3f(-0.36f, -0.18f, 0.0f);

	//glColor3f(0.8f, 0.9f, 1.0f);
	glVertex3f(-0.99f, -0.18f, 0.0f);

	//glColor3f(0.8f, 0.9f, 1.0f);
	glVertex3f(-0.99f, -0.21f, 0.0f);

	//glColor3f(0.8f, 0.9f, 1.0f);
	glVertex3f(-0.36f, -0.21f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // table upper longer quad

	glColor3f(0.66f, 0.66f, 0.66f);
	glVertex3f(-0.26f, -0.134f, 0.0f);

	//glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -0.134f, 0.0f);

	//glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -0.18f, 0.0f);

	//glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(-0.26f, -0.18f, 0.0f);

	glEnd();

	//glLoadIdentity();
	//glScalef(2.0f, 0.0f, 0.0f);

	//glBegin(GL_QUADS);                    // Tile big quad

	//glColor3f(0.8f, 1.0f, 1.0f);
	//glVertex3f(1.0f, -0.665f, 0.0f);

	//glColor3f(0.8f, 1.0f, 1.0f);
	//glVertex3f(-0.365f, -0.665f, 0.0f);

	//glColor3f(0.8f, 1.0f, 1.0f);
	//glVertex3f(-0.366f, -0.84f, 0.0f);

	//glColor3f(0.8f, 1.0f, 1.0f);
	//glVertex3f(1.0f, -0.84f, 0.0f);

	//glEnd();

	glTranslatef(0.0f, 0.0f, -1.0f);

	glBegin(GL_QUADS);                    // left window quad outer

	glColor3f(0.0f, 0.0f, 0.4f);
	glVertex3f(0.62f, 0.91f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.4f);
	glVertex3f(0.048f, 0.91f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.4f);
	glVertex3f(0.048f, 0.04f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.4f);
	glVertex3f(0.62f, 0.04f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // left window quad left door

	glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(0.315f, 0.885f, 0.0f);

	glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(0.075f, 0.885f, 0.0f);

	glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(0.075f, 0.075f, 0.0f);

	glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(0.315f, 0.075f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // right window quad outer

	glColor3f(0.0f, 0.0f, 0.4f);
	glVertex3f(1.238f, 0.91f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.4f);
	glVertex3f(0.666f, 0.91f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.4f);
	glVertex3f(0.666f, 0.04f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.4f);
	glVertex3f(1.238f, 0.04f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // right window quad left door

	glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(0.933f, 0.885f, 0.0f);

	glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(0.693f, 0.885f, 0.0f);

	glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(0.693f, 0.075f, 0.0f);

	glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(0.933f, 0.075f, 0.0f);

	glEnd();

	

	//glLoadIdentity();
	//glScalef(0.0f, -0.1f, 0.0f);
	Cooking_Girl();

	
	glTranslatef(-2.6f, 0.48f, -4.5f);
	Gas();
	
	//glTranslatef(-0.36f, 0.35f, 0.0f);
	glTranslatef(-0.36f, Translate_Line1_Y, 0.0f);
	SteamLine1();

	glTranslatef(0.05f, 0.02f, 0.0f);
	SteamLine2();

	glTranslatef(0.05f, 0.02f, 0.0f);
	SteamLine3();

	glTranslatef(0.01f, 0.02f, 0.0f);
	SteamLine4();

	glEnable(GL_POINT_SMOOTH); 

	glLoadIdentity();
	glTranslatef(-0.17f, 0.15f, -3.0f);
	glPointSize(15.0f);
	glBegin(GL_POINTS);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(0.6f, 0.46f, 0.0f);
	glEnd();

	glLoadIdentity();
	glTranslatef(-0.06f, 0.15f, -3.0f);
	glPointSize(15.0f);
	glBegin(GL_POINTS);
	glColor3f(0.0f, 0.0f, 0.4f);
	glVertex3f(0.4f, 0.46f, 0.0f);
	glEnd();

	glLoadIdentity();
	glTranslatef(0.08f, 0.15f, -3.0f);
	glPointSize(15.0f);
	glBegin(GL_POINTS);
	glColor3f(0.0f, 0.0f, 0.4f);
	glVertex3f(1.0f, 0.46f, 0.0f);
	glEnd();

	glLoadIdentity();
	glTranslatef(-0.17f, 0.15f, -3.0f);
	glPointSize(15.0f);
	glBegin(GL_POINTS);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(1.34f, 0.46f, 0.0f);
	glEnd();
	/*glLineWidth(10.0f);


	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -1.0f);
	glBegin(GL_LINES);

	
	glColor3f(0.9f, 0.6f, 0.4f);
	glVertex3f(1.0f, -0.3f, 0.0f);
	glVertex3f(-1.0f, -0.3f, 0.0f);

	glEnd();
	glLineWidth(1.0f);

*/
	glLoadIdentity();
	glTranslatef(2.6f, -0.3f, -4.1f);
	Fridge();



	SwapBuffers(ghdc);


}

void update(void)
{
	if (Translate_Line1_Y <= 0.65f)
	{
		Translate_Line1_Y = Translate_Line1_Y + 0.0002f;
	}
	else
	{
		Translate_Line1_Y = 0.35f;
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

void Cooking_Girl(void)
{
	//glTranslatef(0.0f, -0.07f, -2.0f);
	glTranslatef(-0.2f, -0.2f, 0.45f);

	glBegin(GL_QUADS);                      //core body

	glColor3f(1.0f, 0.9f, 0.0f);

	glVertex3f(0.09f, 0.48f, 0.0f);


	glVertex3f(-0.11f, 0.48f, 0.0f);


	glVertex3f(-0.11f, 0.27f, 0.0f);

	glVertex3f(0.05f, 0.27f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);              //left arm shoulder part

	glColor3f(1.0f, 0.9f, 0.0f);

	glVertex3f(-0.11f, 0.48f, 0.0f);


	glVertex3f(-0.165f, 0.35f, 0.0f);


	glVertex3f(-0.165f, 0.295f, 0.0f);

	glVertex3f(-0.11f, 0.40f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                      //left arm wrist part

	glColor3f(1.0f, 0.8f, 0.5f);


	glVertex3f(-0.165f, 0.35f, 0.0f);


	glVertex3f(-0.34f, 0.33f, 0.0f);


	glVertex3f(-0.34f, 0.31f, 0.0f);

	glVertex3f(-0.165f, 0.295f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                      //right arm shoulder part

	glColor3f(1.0f, 0.9f, 0.0f);

	glVertex3f(0.09f, 0.48f, 0.0f);


	glVertex3f(0.07f, 0.405f, 0.0f);


	glVertex3f(0.155f, 0.31f, 0.0f);

	glVertex3f(0.21f, 0.32f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                      //right arm wrist part

	glColor3f(1.0f, 0.8f, 0.5f);

	glVertex3f(0.21f, 0.32f, 0.0f);


	glVertex3f(0.155f, 0.31f, 0.0f);


	glVertex3f(0.06f, 0.19f, 0.0f);

	glVertex3f(0.09f, 0.19f, 0.0f);

	glEnd();

	//glLoadIdentity();
	//glTranslatef(0.0f, -0.1f, 0.0f);
	//glScalef(0.0f, -0.001f, 0.0f);
	glBegin(GL_QUADS);                      //left leg

	glColor3f(1.0f, 0.8f, 0.5f);

	glVertex3f(-0.04f, -0.2f, 0.0f);


	glVertex3f(-0.095f, -0.2f, 0.0f);


	//glVertex3f(-0.072f, -0.688f, 0.0f);
	glVertex3f(-0.072f, -0.588f, 0.0f);

	//glVertex3f(-0.046f, -0.7f, 0.0f);
	glVertex3f(-0.046f, -0.60f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                      //right leg

	glColor3f(1.0f, 0.8f, 0.5f);

	glVertex3f(0.03f, -0.2f, 0.0f);


	glVertex3f(-0.025f, -0.2f, 0.0f);


	//glVertex3f(-0.018f, -0.7f, 0.0f);
	glVertex3f(-0.018f, -0.60f, 0.0f);

	//glVertex3f(0.008f, -0.688f, 0.0f);
	glVertex3f(0.008f, -0.588f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                      //right foot 

	glColor3f(1.0f, 0.8f, 0.5f);

	//glVertex3f(0.008f, -0.688f, 0.0f);
	glVertex3f(0.008f, -0.588f, 0.0f);


	//glVertex3f(-0.018f, -0.7f, 0.0f);
	glVertex3f(-0.018f, -0.60f, 0.0f);


	//glVertex3f(0.1f, -0.7352f, 0.0f);
	glVertex3f(0.1f, -0.6352f, 0.0f);

	//glVertex3f(0.12f, -0.725f, 0.0f);
	glVertex3f(0.12f, -0.625f, 0.0f);

	glEnd();

	//glBegin(GL_POLYGON);                      //right foot (shoe part)

	//glColor3f(0.0f, 1.0f, 1.0f);

	//glVertex3f(-0.0018f, -0.7f, 0.0f);


	//glVertex3f(-0.0015f, -0.711f, 0.0f);


	//glVertex3f(0.1f, -0.7352f, 0.0f);

	//glVertex3f(0.12f, -0.725f, 0.0f);

	//glVertex3f(0.086f, -0.712f, 0.0f);

	//glVertex3f(0.076f, -0.722f, 0.0f);

	//glEnd();

	glBegin(GL_QUADS);                      //left foot 

	glColor3f(1.0f, 0.8f, 0.5f);

	//glVertex3f(-0.072f, -0.688f, 0.0f);
	glVertex3f(-0.072f, -0.588f, 0.0f);


	//glVertex3f(-0.18f, -0.728f, 0.0f);
	glVertex3f(-0.18f, -0.632f, 0.0f);


	//glVertex3f(-0.16f, -0.738f, 0.0f);
	glVertex3f(-0.16f, -0.638f, 0.0f);

	//glVertex3f(-0.046f, -0.7f, 0.0f);
	glVertex3f(-0.046f, -0.60f, 0.0f);

	glEnd();

	//glTranslatef(0.0f, 0.005f, 0.0f);
	//glScalef(0.0f, 0.0f, 0.0f);

	glBegin(GL_POLYGON);                      //Skirt part

	glColor3f(0.1f, 0.4f, 0.8f);

	glVertex3f(0.05f, 0.27f, 0.0f);


	glVertex3f(0.066f, 0.236f, 0.0f);


	glVertex3f(0.083f, 0.171f, 0.0f);

	glVertex3f(0.098f, -0.206f, 0.0f);

	glVertex3f(-0.115f, -0.206f, 0.0f);

	glVertex3f(-0.11f, 0.27f, 0.0f);

	glEnd();

	
	glBegin(GL_LINES);             // right palm   

	float radious_right_palm = 0.028f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = radious_right_palm * cos((angle * M_PI) / 180.0f) + (0.08f);
		float y = radious_right_palm * sin((angle * M_PI) / 180.0f) + (0.20f);



		glColor3f(1.0f, 0.8f, 0.5f);
		glVertex3f(0.08f, 0.20f, 0.0f);


		glColor3f(1.0f, 0.8f, 0.5f);
		glVertex3f(x, y, 0.0f);




	}

	glEnd();

	glBegin(GL_LINES);             // left palm   

	float radious_left_palm = 0.028f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = radious_left_palm * cos((angle * M_PI) / 180.0f) + (-0.34f);
		float y = radious_left_palm * sin((angle * M_PI) / 180.0f) + (0.325f);



		glColor3f(1.0f, 0.8f, 0.5f);
		glVertex3f(-0.34f, 0.325f, 0.0f);


		glColor3f(1.0f, 0.8f, 0.5f);
		glVertex3f(x, y, 0.0f);




	}

	glEnd();

	glBegin(GL_POLYGON);                      //Pan handle

	glColor3f(0.4f, 0.0f, 0.0f);

	glVertex3f(-0.272f, 0.314f, 0.0f);


	glVertex3f(-0.27f, 0.302f, 0.0f);


	glVertex3f(-0.28f, 0.294f, 0.0f);

	glVertex3f(-0.34f, 0.322f, 0.0f);

	glVertex3f(-0.482f, 0.322f, 0.0f);

	glVertex3f(-0.482f, 0.346f, 0.0f);

	glVertex3f(-0.34f, 0.346f, 0.0f);

	glEnd();

	glBegin(GL_POLYGON);                      //Pan container

	glColor3f(1.0f, 0.0f, 0.0f);

	glVertex3f(-0.484f, 0.35f, 0.0f);


	glVertex3f(-0.484f, 0.3f, 0.0f);


	glVertex3f(-0.5f, 0.275f, 0.0f);

	glVertex3f(-0.525f, 0.26f, 0.0f);

	glVertex3f(-0.73f, 0.26f, 0.0f);

	glVertex3f(-0.76f, 0.275f, 0.0f);

	glVertex3f(-0.775f, 0.3f, 0.0f);

	glVertex3f(-0.775f, 0.35f, 0.0f);

	glEnd();

	glBegin(GL_POLYGON);                      //Aprin

	glColor3f(0.5f, 0.9f, 1.0f);

	glVertex3f(-0.02f, 0.265f, 0.0f);


	glVertex3f(0.055f, 0.265f, 0.0f);


	glVertex3f(0.055f, 0.25f, 0.0f);

	glVertex3f(0.045f, 0.25f, 0.0f);

	glVertex3f(0.045f, -0.06f, 0.0f);

	glVertex3f(0.03f, -0.09f, 0.0f);

	glVertex3f(-0.02f, -0.12f, 0.0f);

	glVertex3f(-0.07f, -0.12f, 0.0f);

	glVertex3f(-0.09f, -0.11f, 0.0f);

	glVertex3f(-0.11f, -0.084f, 0.0f);

	glVertex3f(-0.11f, 0.265f, 0.0f);

	glEnd();

	//glBegin(GL_POLYGON);                      //hair

	//glColor3f(0.6f, 0.3f, 0.0f);

	//glVertex3f(0.07f, 0.49f, 0.0f);


	//glVertex3f(0.085f, 0.505f, 0.0f);


	//glVertex3f(0.085f, 0.4f, 0.0f);

	//glVertex3f(0.05f, 0.47f, 0.0f);

	//glVertex3f(-0.05f, 0.5f, 0.0f);

	//glVertex3f(-0.14f, 0.46f, 0.0f);

	//glVertex3f(-0.165f, 0.675f, 0.0f);

	//glVertex3f(-0.165f, 0.505f, 0.0f);

	//glVertex3f(-0.15f, 0.49f, 0.0f);

	//glEnd();

	glBegin(GL_QUADS);                      //neck

	glColor3f(1.0f, 0.8f, 0.5f);

	glVertex3f(0.008f, 0.556f, 0.0f);


	glVertex3f(0.008f, 0.478f, 0.0f);


	glVertex3f(-0.044f, 0.478f, 0.0f);

	glVertex3f(-0.044f, 0.556f, 0.0f);

	glEnd();

	glBegin(GL_LINES);             // hair circle   

	float radious_hair = 0.078f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = radious_hair * cos((angle * M_PI) / 180.0f) + (-0.03f);
		float y = radious_hair * sin((angle * M_PI) / 180.0f) + (0.62f);



		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-0.03f, 0.62f, 0.0f);


		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);




	}

	glEnd();

	

	glBegin(GL_LINES);             // face   

	float radious_Face = 0.078f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = radious_Face * cos((angle * M_PI) / 180.0f) + (-0.03f);
		float y = radious_Face * sin((angle * M_PI) / 180.0f) + (0.60f);



		glColor3f(1.0f, 0.8f, 0.5f);
		glVertex3f(-0.03f, 0.60f, 0.0f);


		glColor3f(1.0f, 0.8f, 0.5f);
		glVertex3f(x, y, 0.0f);




	}

	glEnd();

	glBegin(GL_LINES);             // left eye   

	float radious_Left_Eye = 0.009f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = radious_Left_Eye * cos((angle * M_PI) / 180.0f) + (-0.07f);
		float y = radious_Left_Eye * sin((angle * M_PI) / 180.0f) + (0.605f);



		glColor3f(0.6f, 0.2f, 0.1f);
		glVertex3f(-0.07f, 0.605f, 0.0f);


		glColor3f(0.6f, 0.2f, 0.1f);
		glVertex3f(x, y, 0.0f);




	}

	glEnd();

	glBegin(GL_LINES);             // right eye   

	float radious_Right_Eye = 0.009f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = radious_Right_Eye * cos((angle * M_PI) / 180.0f) + (-0.03f);
		float y = radious_Right_Eye * sin((angle * M_PI) / 180.0f) + (0.605f);



		glColor3f(0.6f, 0.2f, 0.1f);
		glVertex3f(-0.03f, 0.605f, 0.0f);


		glColor3f(0.6f, 0.2f, 0.1f);
		glVertex3f(x, y, 0.0f);




	}

	glEnd();

	glBegin(GL_LINES);               // wide smile

	float Radious_Smile = 0.02f;
	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = Radious_Smile * cos((angle * M_PI) / 180.0f) + (-0.05f);
		float y = Radious_Smile * sin(angle * M_PI / 180.0f) + (0.575f);



		if (angle >= 180 && angle <= 360)
		{
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-0.05f, 0.575f, 0.0f);


			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(x, y, 0.0f);
		}





	}

	glEnd();

	glBegin(GL_LINES);               // ear

	float Radious_ear = 0.04f;
	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = Radious_ear * cos((angle * M_PI) / 180.0f) + (0.02f);
		float y = Radious_ear * sin(angle * M_PI / 180.0f) + (0.6f);



		
		
		glColor3f(1.0f, 0.8f, 0.5f);
		glVertex3f(0.02f, 0.6f, 0.0f);


		glColor3f(1.0f, 0.8f, 0.5f);
		glVertex3f(x, y, 0.0f);
		





	}

	glEnd();

	glEnable(GL_POINT_SMOOTH);              //kanatle

	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.04f, 0.57f, 0.0f);
	glEnd();

	glTranslatef(0.015f, 0.1, -0.25f);
	//glScalef(-0.02f, 0.02f, 0.0f);

	glBegin(GL_QUADS);                      //hair left side

	glColor3f(0.0f, 0.0f, 0.0f);

	glVertex3f(-0.1f, 0.603f, 0.0f);


	glVertex3f(-0.09f, 0.637f, 0.0f);


	glVertex3f(-0.14f, 0.616f, 0.0f);

	glVertex3f(-0.155f, 0.582f, 0.0f);

	glEnd();

	//glLoadIdentity();
	glTranslatef(0.005f, -0.1, -0.0f);
	//glScalef(0.02f, 0.0f, 0.0f);

	glBegin(GL_POLYGON);                      //hair right side

	glColor3f(0.0f, 0.0f, 0.0f);

	glVertex3f(-0.09f, 0.736f, 0.0f);


	glVertex3f(-0.082f, 0.714f, 0.0f);


	glVertex3f(-0.07f, 0.696f, 0.0f);

	glVertex3f(-0.05f, 0.68f, 0.0f);

	glVertex3f(-0.03f, 0.67f, 0.0f);
	glVertex3f(0.006f, 0.664f, 0.0f);

	glVertex3f(0.0f, 0.704f, 0.0f);

	glVertex3f(-0.05f, 0.742f, 0.0f);

	glEnd();

	glTranslatef(0.001f, -0.055, -0.0f);
	glBegin(GL_LINES);               // hair juuda

	float Radious_hair = 0.045f;
	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = Radious_hair * cos((angle * M_PI) / 180.0f) + (0.04f);
		float y = Radious_hair * sin(angle * M_PI / 180.0f) + (0.75f);





		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.04f, 0.75f, 0.0f);


		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);






	}

	glEnd();

}

void Gas(void)
{
	glBegin(GL_POLYGON);                //central polygon

	glColor3f(0.3f, 0.f, 0.6f);

	glVertex3f(0.96f, -0.78f, 0.0f);


	glVertex3f(0.99f, -0.74f, 0.0f);


	glVertex3f(0.9f, -0.3f, 0.0f);

	glVertex3f(0.85f, -0.26f, 0.0f);

	glVertex3f(-0.85f, -0.26f, 0.0f);

	glVertex3f(-0.9f, -0.3f, 0.0f);

	glVertex3f(-0.99f, -0.74f, 0.0f);

	glVertex3f(-0.96f, -0.78f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                //left leg 

	glColor3f(0.0f, 0.0f, 0.0f);

	glVertex3f(-0.63f, -0.78f, 0.0f);


	glVertex3f(-0.87f, -0.78f, 0.0f);


	glVertex3f(-0.87f, -0.84f, 0.0f);

	glVertex3f(-0.63f, -0.84f, 0.0f);



	glEnd();

	glBegin(GL_QUADS);                //right leg 

	glColor3f(0.0f, 0.0f, 0.0f);

	glVertex3f(0.63f, -0.78f, 0.0f);


	glVertex3f(0.87f, -0.78f, 0.0f);


	glVertex3f(0.87f, -0.84f, 0.0f);

	glVertex3f(0.63f, -0.84f, 0.0f);



	glEnd();

	glBegin(GL_QUADS);                //central quad

	glColor3f(0.0f, 0.0f, 0.0f);

	glVertex3f(0.54f, -0.21f, 0.0f);


	glVertex3f(-0.54f, -0.21f, 0.0f);


	glVertex3f(-0.54f, -0.26f, 0.0f);

	glVertex3f(0.54f, -0.26f, 0.0f);



	glEnd();


	glBegin(GL_QUADS);                //burner central quad

	glColor3f(0.0f, 0.0f, 0.0f);

	glVertex3f(0.04f, -0.07f, 0.0f);


	glVertex3f(-0.04f, -0.07f, 0.0f);


	glVertex3f(-0.04f, -0.26f, 0.0f);

	glVertex3f(0.04f, -0.26f, 0.0f);



	glEnd();

	glBegin(GL_POLYGON);                //burner left polygon

	glColor3f(0.0f, 0.0f, 0.0f);

	/*glVertex3f(-0.63f, -0.26f, 0.0f);


	glVertex3f(-0.63f, -0.2f, 0.0f);


	glVertex3f(-0.58f, -0.15f, 0.0f);

	glVertex3f(-0.2f, -0.15f, 0.0f);*/



	glVertex3f(-0.6f, -0.07f, 0.0f);

	glVertex3f(-0.7f, -0.18f, 0.0f);

	glVertex3f(-0.7f, -0.26f, 0.0f);

	glVertex3f(-0.63f, -0.26f, 0.0f);


	glVertex3f(-0.63f, -0.2f, 0.0f);


	glVertex3f(-0.58f, -0.15f, 0.0f);

	glVertex3f(-0.2f, -0.15f, 0.0f);

	glVertex3f(-0.2f, -0.07f, 0.0f);

	glEnd();



	glBegin(GL_POLYGON);                //burner right polygon

	glColor3f(0.0f, 0.0f, 0.0f);

	/*glVertex3f(-0.63f, -0.26f, 0.0f);


	glVertex3f(-0.63f, -0.2f, 0.0f);


	glVertex3f(-0.58f, -0.15f, 0.0f);

	glVertex3f(-0.2f, -0.15f, 0.0f);*/



	glVertex3f(0.6f, -0.07f, 0.0f);

	glVertex3f(0.7f, -0.18f, 0.0f);

	glVertex3f(0.7f, -0.26f, 0.0f);

	glVertex3f(0.63f, -0.26f, 0.0f);


	glVertex3f(0.63f, -0.2f, 0.0f);


	glVertex3f(0.58f, -0.15f, 0.0f);

	glVertex3f(0.2f, -0.15f, 0.0f);

	glVertex3f(0.2f, -0.07f, 0.0f);

	glEnd();

	GLfloat x1 = -0.45f;
	GLfloat y1 = -0.2f;
	GLfloat x2 = -0.475f;
	GLfloat y2 = -0.15f;
	GLfloat x3 = -0.5f;
	GLfloat y3 = -0.2f;
	while (x1 <= 0.5f)
	{
		glBegin(GL_TRIANGLES);
		glColor3f((251.0f / 256.0f), (183.0f / 256.0f), (65.0f / 256.0f));
		glVertex3f(x1, y1, 0.0f);
		glVertex3f(x2, y2, 0.0f);
		glVertex3f(x3, y3, 0.0f);
		glEnd();

		x1 = x1 + 0.05;
		x2 = x2 + 0.05f;
		x3 = x3 + 0.05;

	}


}

void SteamLine1(void)
{
	//glTranslatef(0.0f, 0.0f, -3.0f);

	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.00f, 0.0f, 0.0f);
	glVertex3f(0.00f, 0.3f, 0.0f);

	glEnd();
}

void SteamLine2(void)
{
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.13f, 0.2f, 0.0f);
	glVertex3f(0.13f, 0.5f, 0.0f);

	glEnd();
}

void SteamLine3(void)
{
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.3f, 0.0f, 0.0f);
	glVertex3f(0.3f, 0.3f, 0.0f);

	glEnd();
}

void SteamLine4(void)
{
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.6f, 0.0f, 0.0f);
	glVertex3f(0.6f, 0.3f, 0.0f);

	glEnd();
}

//void SteamLine5(void)
//{
//	glBegin(GL_LINES);
//	glColor3f(0.0f, 0.0f, 0.0f);
//	glVertex3f(0.8f, 0.0f, 0.0f);
//	glVertex3f(0.8f, 0.3f, 0.0f);
//
//	glEnd();
//
//}

void Fridge(void)
{
	glBegin(GL_QUADS);              //big quad fridge

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.5f, 1.0f, 0.0f);  


	glVertex3f(-0.5f, 1.0f, 0.0f); 


	glVertex3f(-0.5f, -0.9f, 0.0f);

	glVertex3f(0.5f, -0.9f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);               //upper handle

	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex3f(-0.35f, 0.95f, 0.0f);


	glVertex3f(-0.4f, 0.95f, 0.0f);


	glVertex3f(-0.4f, 0.35f, 0.0f);

	glVertex3f(-0.35f, 0.35f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                  //lower handle

	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex3f(-0.35f, 0.1f, 0.0f);


	glVertex3f(-0.4f, 0.1f, 0.0f);


	glVertex3f(-0.4f, -0.7f, 0.0f);

	glVertex3f(-0.35f, -0.7f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                     //left stand quad

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.3f, -0.90f, 0.0f);


	glVertex3f(-0.4f, -0.90f, 0.0f);


	glVertex3f(-0.4f, -0.98f, 0.0f);

	glVertex3f(-0.3f, -0.98f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                     //left stand quad

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.4f, -0.90f, 0.0f);


	glVertex3f(0.3f, -0.90f, 0.0f);


	glVertex3f(0.3f, -0.98f, 0.0f);

	glVertex3f(0.4f, -0.98f, 0.0f);

	glEnd();




	glLineWidth(7.0f);
	glBegin(GL_LINES);
	glColor3f(0.6f, 0.2f, 0.1f);

	glVertex3f(-0.5f, 0.256f, 0.0f);
	glVertex3f(0.5f, 0.256f, 0.0f);

	glEnd();
	glLineWidth(1.0f);

}







