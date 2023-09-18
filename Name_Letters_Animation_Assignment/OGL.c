//Windows header files

#include<windows.h>            //For declaration of 3.5 lakh API
#include<stdio.h>              // for file I/O
#include<stdlib.h>             // for exit()
#define _USE_MATH_DEFINES
#include<math.h>

#include "OGL.h"            // Header file in same directory

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

void Letter_P(void);
void Letter_R(void);
void Letter_A(void);
void Letter_T(void);
void Letter_H(void);
void Letter_M(void);
void Letter_E(void);
void Letter_S(void);


//global variable declarations

FILE *gpFile = NULL;           // (gpFile) global pointer to a file

HWND ghwnd = NULL;             // handle of window
BOOL gbActive = FALSE;
DWORD dwStyle = 0;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
BOOL gbFullScreen = FALSE;

float radious_P1 = 0.237f;
float radious_P2 = 0.12f;

float radious_R1 = 0.5f;

GLfloat Translate_P_X = -3.5f;
GLfloat Translate_H_X = 3.5f;

GLfloat Translate_R_X = -3.5f;
GLfloat Translate_S_X = 3.5f;
GLfloat Translate_A_Y = 2.5f;
GLfloat Translate_E_Y = 2.5f;
GLfloat Translate_T_Y = -2.5f;
GLfloat Translate_M_Y = -2.5f;
GLfloat Translate_H2_Y = 2.5f;
GLfloat Translate_A2_Y = -2.5f;








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



	PlaySound(TEXT("Homecoming.wav"), NULL, SND_FILENAME | SND_ASYNC);

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


	//code
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//glTranslatef(-1.3f, 0.0f, -3.0f);
	//glTranslatef(-1.6f, 0.0f, -3.50f);
	glTranslatef(Translate_P_X, 0.0f, -3.50f);
	glScalef(0.53f, 0.53f, 0.0f);


	Letter_P();

	

	

	glLoadIdentity();
	//glTranslatef(-1.3f, 0.0f, -3.50f);
	glTranslatef(Translate_R_X, 0.0f, -3.50f);
	
	glScalef(0.5f, 0.5f, 0.0f);

	



	Letter_R();

	glLoadIdentity();
	
	//glTranslatef(-0.89f, -0.02f, -3.0f);
	//glTranslatef(-0.99f, -0.02f, -3.50f);
	glTranslatef(-0.99f, Translate_A_Y, -3.50f);
	glScalef(0.19f, 0.21f, 0.0f);

	Letter_A();

	glLoadIdentity();
	//glTranslatef(-0.56f, -0.02f, -3.0f);
	//glTranslatef(-0.66f, -0.02f, -3.50f);
	glTranslatef(-0.66f, Translate_T_Y, -3.50f);
	glScalef(0.295f, 0.295f, 0.0f);
	Letter_T();

	glLoadIdentity();
	//glTranslatef(-0.14f, -0.02f, -3.0f);
	//glTranslatef(-0.24f, -0.02f, -3.50f);
	glTranslatef(-0.24f, Translate_H2_Y, -3.50f);
	glScalef(0.35f, 0.43f, 0.0f);
	Letter_H();

	glLoadIdentity();
	//glTranslatef(0.29f, -0.02f, -3.0f);
	//glTranslatef(0.19f, -0.02f, -3.50f);
	glTranslatef(0.19f, Translate_A2_Y, -3.50f);
	glScalef(0.19f, 0.21f, 0.0f);

	Letter_A();

	glLoadIdentity();
	//glTranslatef(0.7f, -0.03f, -3.0f);
	//glTranslatef(0.6f, -0.03f, -3.50f);
	glTranslatef(0.6f, Translate_M_Y, -3.50f);
	glScalef(0.19f, 0.26f, 0.0f);

	Letter_M();

	glLoadIdentity();
	//glTranslatef(1.05f, -0.03f, -3.0f);
	//glTranslatef(0.95f, -0.03f, -3.50f);
	glTranslatef(0.95f, Translate_E_Y, -3.50f);
	glScalef(0.19f, 0.28f, 0.0f);
	Letter_E();

	glLoadIdentity();
	//glTranslatef(1.3f, 0.02f, -3.0f);
	//glTranslatef(1.2f, 0.02f, -3.50f);
	glTranslatef(Translate_S_X, 0.02f, -3.50f);
	glScalef(0.25f, 0.39f, 0.0f);
	Letter_S();

	glLoadIdentity();
	//glTranslatef(1.65f, -0.04f, -3.0f);
	//glTranslatef(1.55f, Translate_H_Y, -3.50f);
	glTranslatef(Translate_H_X, -0.04f, -3.50f);
	glScalef(0.35f, 0.44f, 0.0f);
	Letter_H();












	SwapBuffers(ghdc);


}

void update(void)
{
	//code
	
	
	if (Translate_P_X <= -1.61f )
	{
		Translate_P_X = Translate_P_X + 0.0001f;
		//Translate_R_X = Translate_R_X + 0.0002f;
		/*Translate_P_Y = 1.5f;*/
	}

	//if (Translate_R_X <= -1.3f)
	//{
	//	Translate_R_X = Translate_R_X + 0.0002f;
	//	/*Translate_P_Y = 1.5f;*/
	//}

	if (Translate_H_X >= 1.55f)
	{
		Translate_H_X = Translate_H_X - 0.00015f;
		/*Translate_P_Y = 1.5f;*/
	}

	if (Translate_P_X >= -1.61f && Translate_H_X <= 1.55f)
	{
		if (Translate_R_X <= -1.3f)
		{
		Translate_R_X = Translate_R_X + 0.0002f;
		/*Translate_P_Y = 1.5f;*/
		}

		if (Translate_S_X >= 1.2f)
		{
			Translate_S_X = Translate_S_X - 0.0002f;
			/*Translate_P_Y = 1.5f;*/
		}


	}

	if (Translate_R_X >= -1.3f && Translate_S_X <= 1.2)
	{
		if (Translate_A_Y >= -0.02f)
		{
			Translate_A_Y = Translate_A_Y - 0.0002;
		}

		if (Translate_E_Y >= -0.02f)
		{
			Translate_E_Y = Translate_E_Y - 0.0002;
		}
	}

	if (Translate_A_Y <= -0.02f && Translate_E_Y <= -0.02f)
	{
		if (Translate_T_Y <= -0.02f)
		{
			Translate_T_Y = Translate_T_Y + 0.0002;
		}

		if (Translate_M_Y <= -0.03f)
		{
			Translate_M_Y = Translate_M_Y + 0.0002;
		}
	}

	if (Translate_T_Y >= -0.02f )
	{
		if (Translate_H2_Y >= -0.02f)
		{
			Translate_H2_Y = Translate_H2_Y - 0.0002f;
		}
	}

	if (Translate_H2_Y <= -0.02f)
	{
		if (Translate_A2_Y <= -0.02f)
		{
			Translate_A2_Y = Translate_A2_Y + 0.0002f;
		}
		
	}

	

	//if (Translate_S_X >= 1.2f)
	//{
	//	Translate_S_X = Translate_S_X - 0.0002f;
	//	/*Translate_P_Y = 1.5f;*/
	//}



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

void Letter_P(void)
{
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();




	
		
		glBegin(GL_QUADS);                       // big vertical quad

		glColor3f(1.0f, 0.0f, 0.0f);

		glVertex3f(-0.13f, 0.41f, 0.0f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-0.25f, 0.41f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-0.25f, -0.454f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-0.13f, -0.454f, 0.0f);



		glEnd();

	

	glBegin(GL_QUADS);                     // upper small horizontal quad

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.41f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.13f, 0.41f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-0.13f, 0.3f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.3f, 0.0f);


	glEnd();

	glBegin(GL_QUADS);                              // lower small horizontal quad

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.045f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.13f, 0.045f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-0.13f, -0.065f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -0.065f, 0.0f);


	glEnd();

	glBegin(GL_LINES);               // 'P' round part(outer)

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = radious_P1 * cos((angle * M_PI) / 180.0f) + (0.0f);
		float y = radious_P1 * sin(angle * M_PI / 180.0f) + (0.17f);



		if (angle <= 90.0f)
		{
			glColor3f(1.0f, 1.0f, 0.0f);
			glVertex3f(0.0f, 0.17f, 0.0f);


			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(x, y, 0.0f);
		}

		if (angle >= 270.0f)
		{
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.17f, 0.0f);


			glColor3f(1.0f, 1.0f, 0.0f);
			glVertex3f(x, y, 0.0f);
		}





	}

	glEnd();

	glBegin(GL_LINES);               // 'P' round part

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = radious_P2 * cos((angle * M_PI) / 180.0f) + (0.0f);
		float y = radious_P2 * sin(angle * M_PI / 180.0f) + (0.17f);



		if (angle <= 90.0f || angle >= 270.0f)
		{
			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.17f, 0.0f);


			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex3f(x, y, 0.0f);
		}





	}

	glEnd();
}

void Letter_R(void)
{


	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	//glTranslatef(2.0f, 0.0f, -3.0f);
	//glScalef(0.5f, 0.5f, 0.0f);





	glBegin(GL_QUADS);                       // big vertical quad

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.13f, 0.41f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.25f, 0.41f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-0.25f, -0.454f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-0.13f, -0.454f, 0.0f);


	glEnd();

	glBegin(GL_QUADS);                     // upper small horizontal quad

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.41f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.13f, 0.41f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-0.13f, 0.3f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.3f, 0.0f);


	glEnd();

	glBegin(GL_QUADS);                              // lower small horizontal quad

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.045f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.13f, 0.045f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-0.13f, -0.065f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -0.065f, 0.0f);


	glEnd();

	glBegin(GL_LINES);               // 'R' round part

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = radious_P1 * cos((angle * M_PI) / 180.0f) + (0.0f);
		float y = radious_P1 * sin(angle * M_PI / 180.0f) + (0.17f);



		if (angle <= 90.0f)
		{
			glColor3f(1.0f, 1.0f, 0.0f);
			glVertex3f(0.0f, 0.17f, 0.0f);


			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(x, y, 0.0f);
		}

		if (angle >= 270.0f)
		{
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.17f, 0.0f);


			glColor3f(1.0f, 1.0f, 0.0f);
			glVertex3f(x, y, 0.0f);
		}



	}
	glEnd();



	glBegin(GL_LINES);               // 'R' round part

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = radious_P2 * cos((angle * M_PI) / 180.0f) + (0.0f);
		float y = radious_P2 * sin(angle * M_PI / 180.0f) + (0.17f);



		if (angle <= 90.0f || angle >= 270.0f)
		{
			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.17f, 0.0f);


			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex3f(x, y, 0.0f);
		}





	}

	glEnd();

	glBegin(GL_QUADS);                              // R slant line

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, -0.065f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.1f, -0.065f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.1f, -0.454f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.2f, -0.454f, 0.0f);


	glEnd();




}

void Letter_A(void)
{
	glBegin(GL_POLYGON);
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(0.22f, 1.0f, 0.0f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-0.15f, 1.0f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);

		glColor3f(1.0f, 0.0f, 0.0f);
		
		glVertex3f(-0.6f, -1.0f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		
		glVertex3f(-0.36f, -0.36f, 0.0f);

		glColor3f(1.0f, 0.0f, 0.0f);
		
		glVertex3f(0.4f, -0.36f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		
		glVertex3f(0.62f, -1.0f, 0.0f);




	}

	glEnd();

	glBegin(GL_QUADS);
	{
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.62f, -1.0f, 0.0f);

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.4f, -0.36f, 0.0f);

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-0.36f, -0.36f, 0.0f);

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-0.6f, -1.0f, 0.0f);
	}

	glEnd();

	glBegin(GL_POLYGON);
	{
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.3f, -0.05f, 0.0f);

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.07f, 0.61f, 0.0f);

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.61f, 0.0f);

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-0.25f, -0.05f, 0.0f);

	}

	glEnd();
}

void Letter_T(void)
{
	glBegin(GL_QUADS);
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.15f, -0.7f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(0.15f, 0.45f, 0.0f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-0.15f, 0.45f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-0.15f, -0.7f, 0.0f);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-0.64f, 0.42f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(0.63f, 0.42f, 0.0f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.63f, 0.71f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-0.64f, 0.71f, 0.0f);
	}
	glEnd();
}

void Letter_H(void)
{

	glBegin(GL_QUADS);
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.28f, 0.1f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-0.28f, 0.1f, 0.0f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-0.28f, -0.06f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(0.28f, -0.06f, 0.0f);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-0.27f, 0.49f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-0.55f, 0.49f, 0.0f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-0.55f, -0.49f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-0.27f, -0.49f, 0.0f);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.27f, 0.49f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(0.55f, 0.49f, 0.0f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.55f, -0.49f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(0.27f, -0.49f, 0.0f);
	}
	glEnd();

	
}

void Letter_M(void)
{

	glBegin(GL_QUADS);
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.59f, 0.14f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(0.58f, 0.84f, 0.0f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.02f, -0.12f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(0.02f, -0.8f, 0.0f);





	}
	glEnd();

	glBegin(GL_QUADS);
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-0.55f, 0.84f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-0.52f, 0.15f, 0.0f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.02f, -0.8f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(0.02f, -0.12f, 0.0f);

	}
	glEnd();

	glBegin(GL_QUADS);
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-0.52f, 0.84f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-0.85f, 0.84f, 0.0f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-0.85f, -0.8f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-0.52f, -0.8f, 0.0f);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.87f, 0.84f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(0.56f, 0.84f, 0.0f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.56f, -0.8f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(0.87f, -0.8f, 0.0f);
	}
	glEnd();

	




}

void Letter_E(void)
{

	glBegin(GL_QUADS);                       
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-0.38f, 0.78f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-0.58f, 0.78f, 0.0f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-0.58f, -0.78f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-0.38f, -0.78f, 0.0f);
	}
	glEnd();
	glBegin(GL_QUADS);                        //E horizontal quad
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.54f, 0.78f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-0.38f, 0.78f, 0.0f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-0.38f, 0.59f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(0.54f, 0.59f, 0.0f);
	}
	glEnd();

	glBegin(GL_QUADS);                         //E horizontal quad
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.54f, 0.12f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-0.38f, 0.12f, 0.0f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-0.38f, -0.06f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(0.54f, -0.06f, 0.0f);
	}
	glEnd();

	glBegin(GL_QUADS);                       // E horizontal quad
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.54f, -0.6f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-0.38f, -0.6f, 0.0f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-0.38f, -0.78f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(0.54f, -0.78f, 0.0f);
	}
	glEnd();

}

void Letter_S(void)
{
	glBegin(GL_QUADS);            

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.3f, 0.4f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.3f, 0.4f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-0.3f, 0.3f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.3f, 0.3f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-0.2f, 0.4f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.3f, 0.4f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-0.3f, -0.2f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.2f, -0.2f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.3f, -0.1f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.3f, -0.1f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-0.3f, -0.2f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.3f, -0.2f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.3f, -0.1f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.3f, -0.1f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-0.3f, -0.2f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.3f, -0.2f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.3f, -0.1f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.2f, -0.1f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.2f, -0.7f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.3f, -0.7f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.3f, -0.6f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.3f, -0.6f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-0.3f, -0.7f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.3f, -0.7f, 0.0f);

	glEnd();


}







