//Windows header files

#include<windows.h>            //For declaration of 3.5 lakh API
#include<stdio.h>              // for file I/O
#include<stdlib.h>             // for exit()

#include "OGL.h"            // Header file in same directory

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

	//local variable declarations
	

	GLfloat numRows = 5;
	GLfloat numColumns = 5;

	GLfloat i = 0;
	GLfloat j = 0;

	GLfloat xCoordinate = -0.5f;
	GLfloat yCoordinate = -1.0f;

	GLint fullScreenwidth;
	GLint fullScreenHeight;

	GLint squareWidth = WIN_WIDTH / 3;
	GLint squareHeight = WIN_HEIGHT / 2;

	GLint countLines = 0;

	

	//code
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glTranslatef(0.0f, 0.0f, -3.0f);

	if (gbFullScreen == FALSE)
	{
		glViewport(0.0, squareHeight, (GLsizei)squareWidth, (GLsizei)squareHeight);
	}

	if (gbFullScreen == TRUE)
	{
		RECT rect;
		GetClientRect(ghwnd, &rect);
		fullScreenwidth = rect.right - rect.left;
		fullScreenHeight = rect.bottom - rect.top;

		squareWidth = fullScreenwidth / 3;
		squareHeight = fullScreenHeight / 2;

		glViewport(0.0, squareHeight, (GLsizei)squareWidth, (GLsizei)squareHeight);
	}

	

	
	glPointSize(4.0f);
	
	glBegin(GL_POINTS);

	

	glColor3f(1.0f, 1.0f, 1.0f);

	
	
	for (i = 0; i < numRows; i++)
	{
		

		for (j = 0; j < numColumns; j++)
		{
			glVertex3f(xCoordinate, yCoordinate, 0.0f);

			xCoordinate += 0.5f;
		}

		yCoordinate += 0.25f;

		xCoordinate = -0.5f;
	}

	

	glEnd();

	glPointSize(1.0f);


	//first row third figure

	if (gbFullScreen == FALSE)
	{
		//glViewport(2*squareWidth, (WIN_HEIGHT / 2), (GLsizei)(WIN_WIDTH / 3), (GLsizei)(WIN_HEIGHT / 2));

		glViewport(2 * squareWidth, squareHeight, (GLsizei)squareWidth, (GLsizei)squareHeight);
	}

	if (gbFullScreen == TRUE)
	{
		RECT rect;
		GetClientRect(ghwnd, &rect);
		fullScreenwidth = rect.right - rect.left;
		fullScreenHeight = rect.bottom - rect.top;

		squareWidth = fullScreenwidth / 3;
		squareHeight = fullScreenHeight / 2;

		glViewport(2* squareWidth, squareHeight, (GLsizei)(squareWidth), (GLsizei)(squareHeight));
	}

	

	//re initialize local variables

	numColumns = 4;
	numRows = 4;
	//left bottom

	GLfloat leftBottomX = -1.0f;
	GLfloat leftBottomY = -1.0f;

	//right bottom

	GLfloat rightBottomX = -0.5f;
	GLfloat rightBottomY = -1.0f;

	//right top

	GLfloat rightTopX = -0.5f;
	GLfloat rightTopY = -0.5f;

	//left top

	GLfloat leftTopX = -1.0f;
	GLfloat leftTopY = -0.5f;


	glBegin(GL_LINES);

	glColor3f(1.0f, 1.0f, 1.0f);

	glPushMatrix();
	{
		glTranslatef(0.0f, -0.1f, 0.0f);
	}
	glPopMatrix();




	for (i = 0; i < numRows-1; i++)
	{


		for (j = 0; j < numColumns; j++)
		{
			//bottom line

			glVertex3f(leftBottomX, leftBottomY, 0.0f);
			glVertex3f(rightBottomX, rightBottomY, 0.0f);

			//right line

			glVertex3f(rightBottomX, rightBottomY, 0.0f);
			glVertex3f(rightTopX, rightTopY, 0.0f);

			//top line

			glVertex3f(rightTopX, rightTopY, 0.0f);
			glVertex3f(leftTopX, leftTopY, 0.0f);

			//left line

			glVertex3f(leftTopX, leftTopY, 0.0f);
			glVertex3f(leftBottomX, leftBottomY, 0.0f);

			leftBottomX = leftBottomX + 0.5f;

			rightBottomX = rightBottomX + 0.5f;

			rightTopX = rightTopX + 0.5f;

			leftTopX = leftTopX + 0.5f;
		}

		//reset X

		leftBottomX = -1.0f;
		
		rightBottomX = -0.5f;
		
		rightTopX = -0.5f;
	
		leftTopX = -1.0f;
		

		leftBottomY = leftBottomY + 0.25f;

		rightBottomY = rightBottomY + 0.25f;

		rightTopY = rightTopY + 0.25f;

		leftTopY = leftTopY + 0.25f;
	}

	glEnd();

	//fourth shape 

	if (gbFullScreen == FALSE)
	{
		//glViewport(2*squareWidth, (WIN_HEIGHT / 2), (GLsizei)(WIN_WIDTH / 3), (GLsizei)(WIN_HEIGHT / 2));

		glViewport(0.0, 0.0, (GLsizei)squareWidth, (GLsizei)squareHeight);
	}

	if (gbFullScreen == TRUE)
	{
		RECT rect;
		GetClientRect(ghwnd, &rect);
		fullScreenwidth = rect.right - rect.left;
		fullScreenHeight = rect.bottom - rect.top;

		squareWidth = fullScreenwidth / 3;
		squareHeight = fullScreenHeight / 2;

		glViewport(0.0, 0.0, (GLsizei)(squareWidth), (GLsizei)(squareHeight));
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glPushMatrix();
	{
		glTranslatef(0.45f, 0.45f, 0.0f);
	

	
		 leftTopX = -1.0f;
		 leftTopY = -0.75f;

		 leftBottomX = -1.0f;
		 leftBottomY = -1.0f;

		 rightTopX = -0.5f;
		 rightTopY = -0.75f;

		 rightBottomX = -0.5f;
		 rightBottomY = -1.0f;
	
	
		 for (i = 0; i < 4; i++)
		 {
			 for (j = 0; j < 4; j++)
			 {
				 glBegin(GL_QUADS);

				 glVertex3f(rightTopX, rightTopY, 0.0f);       //right top
				 glVertex3f(leftTopX, leftTopY, 0.0f);         //left top
				 glVertex3f(leftBottomX, leftBottomY, 0.0f);   // left bottom
				 glVertex3f(rightBottomX, rightBottomY, 0.0f); //right bottom

				 glEnd();

				 glBegin(GL_LINES);

				 glVertex3f(leftBottomX, leftBottomY, 0.0f);   // left bottom

				 glVertex3f(rightTopX, rightTopY, 0.0f);       //right top

				 glEnd();

				 leftTopX = rightTopX;

				 leftBottomX = rightBottomX;

				 rightBottomX = leftBottomX + 0.5f;

				 rightTopX = leftTopX + 0.5f;

				 glEnd();

			 

			 }

			 //reset X coordinate

			 leftTopX = -1.0f;
			 leftBottomX = -1.0f;
			 rightTopX = -0.5f;
			 rightBottomX = -0.5f;


			 //increase Y

			 leftTopY += 0.25f;

			 leftBottomY += 0.25f;

			 rightBottomY += 0.25f;

			 rightTopY += 0.25f;
		 }
	 
	
	}
	glPopMatrix();

	 //second shape

	if (gbFullScreen == FALSE)
	{
		//glViewport(2*squareWidth, (WIN_HEIGHT / 2), (GLsizei)(WIN_WIDTH / 3), (GLsizei)(WIN_HEIGHT / 2));

		glViewport(squareWidth, squareHeight, (GLsizei)squareWidth, (GLsizei)squareHeight);
	}

	if (gbFullScreen == TRUE)
	{
		RECT rect;
		GetClientRect(ghwnd, &rect);
		fullScreenwidth = rect.right - rect.left;
		fullScreenHeight = rect.bottom - rect.top;

		squareWidth = fullScreenwidth / 3;
		squareHeight = fullScreenHeight / 2;

		glViewport(squareWidth, squareHeight, (GLsizei)(squareWidth), (GLsizei)(squareHeight));
	}
	
	leftTopX = -1.0f;
	leftTopY = -0.75f;

	leftBottomX = -1.0f;
	leftBottomY = -1.0f;

	rightTopX = -0.5f;
	rightTopY = -0.75f;

	/*rightBottomX = -0.5f;
	rightBottomY = -1.0f;*/

	glPushMatrix();
	{
		glTranslatef(0.2f, 0.0f, 0.0f);

		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				glBegin(GL_TRIANGLES);

				glVertex3f(rightTopX, rightTopY, 0.0f);       //right top
				glVertex3f(leftTopX, leftTopY, 0.0f);         //left top
				glVertex3f(leftBottomX, leftBottomY, 0.0f);   // left bottom
				//glVertex3f(rightBottomX, rightBottomY, 0.0f); //right bottom

				glEnd();

				leftTopX += 0.5f;

				leftBottomX += 0.5f;

				rightTopX += 0.5f;

			}

			//reset X Coordinate

			leftTopX = -1.0f;


			leftBottomX = -1.0f;


			rightTopX = -0.5f;



			//Increase Y Coordinate;
			leftTopY += 0.25;

			leftBottomY += 0.25f;

			rightTopY += 0.25f;
		}

	}

	glPopMatrix();

	//shape 5


	if (gbFullScreen == FALSE)
	{
		//glViewport(2*squareWidth, (WIN_HEIGHT / 2), (GLsizei)(WIN_WIDTH / 3), (GLsizei)(WIN_HEIGHT / 2));

		glViewport(squareWidth, (squareHeight * 0), (GLsizei)squareWidth, (GLsizei)squareHeight);
	}

	if (gbFullScreen == TRUE)
	{
		RECT rect;
		GetClientRect(ghwnd, &rect);
		fullScreenwidth = rect.right - rect.left;
		fullScreenHeight = rect.bottom - rect.top;

		squareWidth = fullScreenwidth / 3;
		squareHeight = fullScreenHeight / 2;

		glViewport(squareWidth, (squareHeight * 0), (GLsizei)(squareWidth), (GLsizei)(squareHeight));
	}

	glBegin(GL_QUADS);


	glVertex3f(1.0f, 0.65f, 0.0f);       //right top
	glVertex3f(-1.0f, 0.65f, 0.0f);         //left top
	glVertex3f(-1.0f, -0.65f, 0.0f);   // left bottom
	glVertex3f(1.0f, -0.65f, 0.0f); //right bottom

	glEnd();

	

	glBegin(GL_LINES);


	glVertex3f(-1.0f, 0.65f, 0.0f);         //left top
	glVertex3f(-0.5f, -0.65f, 0.0f);

	glVertex3f(-1.0f, 0.65f, 0.0f);         //left top
	glVertex3f(-0.0f, -0.65f, 0.0f);

	glVertex3f(-1.0f, 0.65f, 0.0f);         //left top
	glVertex3f(0.5f, -0.65f, 0.0f);

	glVertex3f(-1.0f, 0.65f, 0.0f);         //left top
	glVertex3f(1.0f, -0.65f, 0.0f);

	glVertex3f(-1.0f, 0.65f, 0.0f);         //left top
	glVertex3f(1.0f, -0.325f, 0.0f);

	glVertex3f(-1.0f, 0.65f, 0.0f);         //left top
	glVertex3f(1.0f, 0.0f, 0.0f);

	glVertex3f(-1.0f, 0.65f, 0.0f);         //left top
	glVertex3f(1.0f, 0.325f, 0.0f);



	glEnd();

	//sixth shape

	if (gbFullScreen == FALSE)
	{
		//glViewport(2*squareWidth, (WIN_HEIGHT / 2), (GLsizei)(WIN_WIDTH / 3), (GLsizei)(WIN_HEIGHT / 2));

		glViewport(squareWidth * 2, (squareHeight * 0), (GLsizei)squareWidth, (GLsizei)squareHeight);
	}

	if (gbFullScreen == TRUE)
	{
		RECT rect;
		GetClientRect(ghwnd, &rect);
		fullScreenwidth = rect.right - rect.left;
		fullScreenHeight = rect.bottom - rect.top;

		squareWidth = fullScreenwidth / 3;
		squareHeight = fullScreenHeight / 2;

		glViewport(squareWidth * 2, (squareHeight * 0), (GLsizei)(squareWidth), (GLsizei)(squareHeight));
	}

	//glBegin(GL_QUADS);


	//glVertex3f(1.0f, 0.65f, 0.0f);       //right top
	//glVertex3f(-1.0f, 0.65f, 0.0f);         //left top
	//glVertex3f(-1.0f, -0.65f, 0.0f);   // left bottom
	//glVertex3f(1.0f, -0.65f, 0.0f); //right bottom

	//glEnd();

	leftTopX = -1.0f;
	leftTopY = -0.216;

	leftBottomX = -1.0f;
	leftBottomY = -0.65f;

	rightTopX = -0.333;
	rightTopY = -0.216;

	rightBottomX = -0.333;
	rightBottomY = -0.65f;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			if (j == 0)
			{
				glColor3f(1.0f, 0.0f, 0.0f);
			}

			else if (j == 1)
			{
				glColor3f(0.0f, 1.0f, 0.0f);
			}

			else
			{
				glColor3f(0.0f, 0.0f, 1.0f);

			}
			glBegin(GL_QUADS);


			glVertex3f(rightTopX, rightTopY, 0.0f);       //right top
			glVertex3f(leftTopX, leftTopY, 0.0f);         //left top
			glVertex3f(leftBottomX, leftBottomY, 0.0f);   // left bottom
			glVertex3f(rightBottomX, rightBottomY, 0.0f); //right bottom

			glEnd();

			glColor3f(1.0f, 1.0f, 1.0f);

			glBegin(GL_LINES);

			glVertex3f(rightTopX, rightTopY, 0.0f);       //right top
			glVertex3f(leftTopX, leftTopY, 0.0f);         //left top

			glVertex3f(leftTopX, leftTopY, 0.0f);         //left top
			glVertex3f(leftBottomX, leftBottomY, 0.0f);   // left bottom

			glVertex3f(leftBottomX, leftBottomY, 0.0f);   // left bottom
			glVertex3f(rightBottomX, rightBottomY, 0.0f); //right bottom

			glVertex3f(rightBottomX, rightBottomY, 0.0f); //right bottom
			glVertex3f(rightTopX, rightTopY, 0.0f);       //right top

			glEnd();


			// increment X

			leftTopX += 0.666f;


			leftBottomX += 0.666f;


			rightTopX += 0.666f;


			rightBottomX += 0.666f;



		}

		//reset X

		leftTopX = -1.0f;
		

		leftBottomX = -1.0f;
		

		rightTopX = -0.333;
		

		rightBottomX = -0.333;
		

		//increment Y

		leftTopY += 0.433;

		
		leftBottomY += 0.433;

		
		rightTopY += 0.433;

		
		rightBottomY += 0.433;
	}

	


	




	

	


	

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







