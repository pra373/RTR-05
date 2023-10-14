// Windows header files
// Common headers 
#include<windows.h> //For all WIN32 API's
#include<stdio.h>   //for file I/O
#include<stdlib.h>  //for exit()

#define _USE_MATH_DEFINES
#include<math.h>

#include "OGL.h" // swatachi header file

// OpenGL header Files
#include<gl/GL.h> 

#include<GL/glu.h>

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Link With OpenGL Library
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib") //file me OpenGL

// Global Function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global Variable Declarations
//OpenGL Related Global Variables
HDC ghdc = NULL;
HGLRC  ghrc = NULL;       // handle to graphic rendering contex

FILE *gpFile = NULL;

HWND ghwnd = NULL;        // global window handel 
BOOL gbActive = FALSE;    //means by default mazi window active nhi
DWORD dwStyle = 0;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) }; //wpPrev = windowplacementPrevious
BOOL gbFullScreen = FALSE;
GLfloat P_angle = 0.0f;

// Entry-Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// Function declarations
	int initialize(void);
	void uninitialize(void);
	void display(void);
	void update(void);

	//Local Variable Declarations
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("Prathamesh Window");
	int iResult = 0;
	BOOL bDone = FALSE; //SDK cha BOOL ahe

	// step : centering window sathi
	//int w_height = 600; //window height
	//int w_width = 800;  // window length or width
	int top_left_X = GetSystemMetrics(SM_CXSCREEN);   // (SM_CXSCREEN); <- its a macro... So SM_CX value gives width
	int top_left_Y = GetSystemMetrics(SM_CYSCREEN);   // (SM_CYSCREEN); <- its a macro... So SM_CY value gives height
	int y;
	int x;
	x = (top_left_X / 2) - WIN_WIDTH / 2;
	y = (top_left_Y / 2) - WIN_HEIGHT / 2;

	//Code
	gpFile = fopen("Log.txt", "w"); //small w mahatwacha
	if (gpFile == NULL)
	{
		MessageBox(NULL, TEXT("Log File Cannot Be Opened"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	fprintf(gpFile, "Program Started Successfully...\n");

	//WNDCLASSEX Class Initialization 
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW || CS_VREDRAW || CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	//wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	//wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	//Register WNDCLASSEX 
	RegisterClassEx(&wndclass);

	//CreateWindow
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		TEXT("Prathamesh Laxmikant Paropkari"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,


		// Passing Calculated 4 Values in CreateWindow()
		x,            // x - user window top left x coordinate
		y,            // y - user window top left y coordinate
		WIN_WIDTH,     // width of window
		WIN_HEIGHT,    // height of window
		NULL,
		NULL,
		hInstance,
		NULL
	);


	ghwnd = hwnd;


	// initialization
	iResult = initialize();

	if (iResult != 0)
	{
		MessageBox(hwnd, TEXT("Initialization Failed"), TEXT("Error"), MB_OK | MB_ICONERROR);

		// unitialize();
		// exit (0);
		DestroyWindow(hwnd);
	}

	// Show The Window
	ShowWindow(hwnd, iCmdShow);

	// Set Foreground Window
	SetForegroundWindow(hwnd);

	SetFocus(hwnd);


	//Game Loop
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
			//code
			if (gbActive == TRUE)
			{
				// Render
				display();

				// Update
				update();
			}
		}
	}


	// Uninitialization
	uninitialize();

	return((int)msg.wParam);
} //WinMain ends here


// Call Back Function
LRESULT CALLBACK WndProc(HWND hwnd,
	UINT imsg,
	WPARAM wParam,
	LPARAM lParam)

{
	// function declaration | FUNCTION PROTOTYPE
	void ToggleFullScreen(void);
	void resize(int, int);

	// Code
	switch (imsg)
	{
	case WM_SETFOCUS:
		gbActive = TRUE;
		break;
	case WM_KILLFOCUS:
		gbActive = FALSE;
		break;
	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_ERASEBKGND:
		return(0);
	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
		case VK_ESCAPE:   //VK_ESCAPE has 27 ascii value
			DestroyWindow(hwnd);
			break;
		}
		break;
	case WM_CHAR:
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
	{
		DestroyWindow(hwnd);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return(DefWindowProc(hwnd, imsg, wParam, lParam));
}

void ToggleFullScreen(void)
{
	// Local-Variable Declarations
	MONITORINFO mi = { sizeof(MONITORINFO) };

	// Code
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

// User defined function defination
int initialize(void)
{
	//function declarations
	void resize(int, int);
	// Function Declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormateIndex = 0;

	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	// Initialization of PIXELFORMATDESCRIPTOR
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; // Syntax ky sangto :- 1)window vr draw kr 2)  3)
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	//resize(WIN_WIDTH, WIN_HEIGHT);

	// For Depth
	pfd.cDepthBits = 32; //1st change for depth

	//Get The DC
	ghdc = GetDC(ghwnd);
	if (ghdc == NULL)
	{
		fprintf(gpFile, "GetDC Failed\n");
		return(-1);
	}

	iPixelFormateIndex = ChoosePixelFormat(ghdc, &pfd);
	if (iPixelFormateIndex == 0) //if PixelFormateIndex is zero if toh fail zala tr
	{
		fprintf(gpFile, "ChoosePixelFormat Failed\n");
		return(-2);
	}

	// Set obtained Pixel formate 
	if (SetPixelFormat(ghdc, iPixelFormateIndex, &pfd) == FALSE)
	{
		fprintf(gpFile, "SetPixelFormat Failed\n");
		return(-3);
	}

	// Step 4 Create OpenGl Context from Device Context
	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
	{
		fprintf(gpFile, "wglCreateContext Failed\n");
		return(-4);
	}

	// Make Rendering Context Current
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		fprintf(gpFile, "wglMakeCurrent Failed\n");
		return(-5);
	}

	// 2nd change for depth or Enabling depth
	glShadeModel(GL_SMOOTH); // (Its beautification line Not compulsory) jeva kheva color deu theva shade smooth thev
	glClearDepth(1.0f); // Compulsory
	glEnable(GL_DEPTH_TEST); // 8 test paike ek(depth) test enable kr
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // optional for beautification


	// Set The ClearColor Of Window To Blue
	// Clear OpenGl Stars
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Blue Color
	resize(WIN_WIDTH, WIN_HEIGHT);

	//code

	return(0);
}

void resize(int width, int height)
{
	// code
	if (height <= 0)
		height = 1;


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	gluPerspective(
		45.0f,
		((GLfloat)width / (GLfloat)height),
		0.1f,
		100.0f);


}

void display(void)
{
	void BaseRectangle(void);
	void BaseTriangle(void);
	void Roof(void);
	void Windows(void);
	void Door(void);
	void SmokeOutlet(void);

	//code
	// 3rd Change GL_DEPTH_BUFFER_BIT
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);
	glRotatef(P_angle, 0.0f, 1.0f, 0.0f);

	BaseRectangle();
	BaseTriangle();
	Roof();
	Windows();
	SmokeOutlet();
	Door();

	SwapBuffers(ghdc);

}

void update(void)
{
	//code


	P_angle = P_angle + 0.02f;

	if (P_angle >= 360.0f)
	{
		P_angle = 0;
	}
}


void uninitialize(void)
{
	// Function Declarations
	void ToggleFullScreen(void);

	//code
	// if application is exiting in fullScreen then ha pudcha code follow kela jael
	if (gbFullScreen == TRUE)
	{
		ToggleFullScreen();
		gbFullScreen = FALSE;
	}

	// Make the HDC as Current Context
	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}

	// Destroy Rendering Context or Delete Rendering Context
	if (ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	// Release HDC 
	if (ghdc)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}


	// DistroyWindow
	if (ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd = NULL;
	}

	//Close The Log File
	if (gpFile)
	{
		fprintf(gpFile, "Program Ended Sucessfully...\n");
		fclose(gpFile);
		gpFile = NULL;
	}

}

void BaseRectangle(void)
{
	glBegin(GL_QUADS);

	glColor3f(1.0f, 1.0f, 0.8f);                 // base rectangle front face

	glVertex3f(0.5f, 0.0f, 0.0f);     //right top
	glVertex3f(-0.5f, 0.0f, 0.0f);    //left top
	glVertex3f(-0.5f, -0.6f, 0.0f);   //left bottom
	glVertex3f(0.5f, -0.6f, 0.0f);    //right bottom

	//base rectangle right face

	
	glVertex3f(0.5f, 0.0f, -1.0f);   //right top                 
	glVertex3f(0.5f, 0.0f, 0.0f);    //left top
	glVertex3f(0.5f, -0.6f, 0.0f);   //left bottom
	glVertex3f(0.5f, -0.6f, -1.0f);  //right bottom 

	//base rectangle back face

	glVertex3f(-0.5f, 0.0f, -1.0f);    //right top
	glVertex3f(0.5f, 0.0f, -1.0f);     //left top
	glVertex3f(0.5f, -0.6f, -1.0f);     //left bottom
	glVertex3f(-0.5f, -0.6f, -1.0f);    //right bottom

	//base rectangle left face
	glVertex3f(-0.5f, 0.0f, 0.0f);    //right top
	glVertex3f(-0.5f, 0.0f, -1.0f);   //left top
	glVertex3f(-0.5f, -0.6f, -1.0f);  //left bottom
	glVertex3f(-0.5f, -0.6f, 0.0f);   //left bottom





	glEnd();
}

void BaseTriangle(void)
{
	//triangle for roof front face
	glBegin(GL_TRIANGLES);

	glColor3f(1.0f, 1.0f, 0.8f);

	glVertex3f(0.0f, 0.5f, 0.0f);


	glVertex3f(-0.5f, 0.0f, 0.0f);


	glVertex3f(0.5f, 0.0f, 0.0f);

	glEnd();

	//triangle for roof back face
	glBegin(GL_TRIANGLES);
	glVertex3f(0.0f, 0.5f, -1.0f);
	glVertex3f(-0.5f, 0.0f, -1.0f);
	glVertex3f(0.5f, 0.0f, -1.0f);
	glEnd();

	//right part of triangle

	glBegin(GL_QUADS);
	glVertex3f(0.0f, 0.5f, 0.0f);    //left top
	glVertex3f(0.5f, 0.0f, 0.0f);    //left bottom
	glVertex3f(0.5f, 0.0f, -1.0f);   //right bottom
	glVertex3f(0.0f, 0.5f, -1.0f);   //right top

	//left part of triangle
	glVertex3f(0.0f, 0.5f, 0.0f);    //right top
	glVertex3f(0.0f, 0.5f, -1.0f);   //left top
	glVertex3f(-0.5f, 0.0f, -1.0f);  //left bottom
	glVertex3f(-0.5f, 0.0f, 0.0f);   //right bottom



	glEnd();

}

void Roof(void)
{
	glBegin(GL_QUADS);                            // left roof front face 

	glColor3f(0.3f, 0.2f, 0.0f);
	glVertex3f(0.0f, 0.5f, 0.0f);    //right bottom


	glVertex3f(0.0f, 0.6f, 0.0f);   //right top


	glVertex3f(-0.7f, -0.1f, 0.0f);  //left-left


	glVertex3f(-0.6f, -0.1f, 0.0f); // left - right

	//left roof 3d part upper roof
	
	glVertex3f(0.0f, 0.6f, 0.0f);    //right top
	glVertex3f(0.0f, 0.6f, -1.06f);   //left top
	glVertex3f(-0.7f, -0.1f, -1.06f); //left bottom
	glVertex3f(-0.7f, -0.1f, 0.0f);  //right bottom

	//left roof 3d part lower roof
	glVertex3f(0.0f, 0.5f, 0.0f);    //right top
	glVertex3f(0.0f, 0.5f, -1.06f);   //left top
	glVertex3f(-0.6f, -0.1f, -1.06f); //left bottom
	glVertex3f(-0.6f, -0.1f, 0.0f);  //right bottom

	//left roof 3d part back quad

	glVertex3f(0.0f, 0.5f, -1.06f);    //right bottom


	glVertex3f(0.0f, 0.6f, -1.06f);   //right top


	glVertex3f(-0.7f, -0.1f, -1.06f);  //left-left


	glVertex3f(-0.6f, -0.1f, -1.06f); // left - right









	glEnd();

	glBegin(GL_QUADS);                          // right roof 3D front quad

	
	glVertex3f(0.0f, 0.5f, 0.0f);   //left bottom

	
	glVertex3f(0.6f, -0.1f, 0.0f);  //right-left

	
	glVertex3f(0.7f, -0.1f, 0.0f);  // right-right

	
	glVertex3f(0.0f, 0.6f, 0.0f);   //left top

	//right roof 3d part upper roof

	glVertex3f(0.0f, 0.6f, -1.06f);   //right top
	glVertex3f(0.0f, 0.6f, 0.0f);     //left top
	glVertex3f(0.7f, -0.1f, 0.0f);    //left bottom
	glVertex3f(0.7f, -0.1f, -1.06f);  //right bottom

	//right roof 3d part lower roof

	glVertex3f(0.0f, 0.5f, -1.06f);   //right top
	glVertex3f(0.0f, 0.5f, 0.0f);     //left top
	glVertex3f(0.6f, -0.1f, 0.0f);    //left bottom
	glVertex3f(0.6f, -0.1f, -1.06f);  //right bottom

	//right roof 3D back quad
	glVertex3f(0.0f, 0.5f, -1.06f);   //left bottom


	glVertex3f(0.6f, -0.1f, -1.06f);  //right-left


	glVertex3f(0.7f, -0.1f, -1.06f);  // right-right


	glVertex3f(0.0f, 0.6f, -1.06f);   //left top

	glEnd();

}

void Windows(void)
{
	glBegin(GL_QUADS);                // Top window

	glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(0.1f, 0.3f, 0.001f);

	glColor3f(0.6f, 0.8f, 1.0f);
	glVertex3f(-0.1f, 0.3f, 0.001f);

	glColor3f(0.6f, 0.8f, 1.0f);
	glVertex3f(-0.1f, 0.1f, 0.001f);

	glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(0.1f, 0.1f, 0.001f);

	glEnd();

	glBegin(GL_QUADS);                       //left window

	glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(-0.2f, -0.1f, 0.001f);

	glColor3f(0.6f, 0.8f, 1.0f);
	glVertex3f(-0.4f, -0.1f, 0.001f);

	glColor3f(0.6f, 0.8f, 1.0f);
	glVertex3f(-0.4f, -0.3f, 0.001f);

	glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(-0.2f, -0.3f, 0.001f);

	glEnd();

	glBegin(GL_QUADS);                  //right window

	glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(0.4f, -0.1f, 0.001f);

	glColor3f(0.6f, 0.8f, 1.0f);
	glVertex3f(0.2f, -0.1f, 0.001f);

	glColor3f(0.6f, 0.8f, 1.0f);
	glVertex3f(0.2f, -0.3f, 0.001f);

	glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(0.4f, -0.3f, 0.001f);

	glEnd();



	glLineWidth(3.0f);

	glBegin(GL_LINES);                      // vetical line left window

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.3f, -0.1f, 0.001f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.3f, -0.3f, 0.001f);

	glEnd();


	glBegin(GL_LINES);                        // horizontal line left window

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.4f, -0.2f, 0.001f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.2f, -0.2f, 0.001f);

	glEnd();

	glBegin(GL_LINES);                    // vetical line right window

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.3f, -0.1f, 0.001f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.3f, -0.3f, 0.001f);

	glEnd();

	glBegin(GL_LINES);                     // horizontal line right window

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.4f, -0.2f, 0.001f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.2f, -0.2f, 0.001f);

	glEnd();

	glBegin(GL_LINES);                     // vertical line top window

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.3f, 0.001f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.1f, 0.001f);

	glEnd();

	glBegin(GL_LINES);                     // horizontal line top window

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.1f, 0.2f, 0.001f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.1f, 0.2f, 0.001f);

	glEnd();

	glLineWidth(1.0f);


	//base rectangle right face

	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(0.5f, 0.0f, -1.0f);   //right top                 
	//glVertex3f(0.5f, 0.0f, 0.0f);    //left top
	//glVertex3f(0.5f, -0.6f, 0.0f);   //left bottom
	//glVertex3f(0.5f, -0.6f, -1.0f);  //right bottom 

	glBegin(GL_QUADS);               //window right wall

	glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(0.51f, -0.15f, -0.6f);  //right top

	glColor3f(0.6f, 0.8f, 1.0f);
	glVertex3f(0.51f, -0.15f, -0.4f);  //left top

	glColor3f(0.6f, 0.8f, 1.0f);
	glVertex3f(0.51f, -0.35f, -0.4f); //left bottom

	glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(0.51f, -0.35f, -0.6f); // right bottom

	glEnd();

	glBegin(GL_LINES);                     // horizontal line right wall window

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.515f, -0.25f, -0.4f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.515f, -0.25f, -0.6f);

	glEnd();

	glBegin(GL_LINES);                     // vertical line right wall window

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.515f, -0.15f, -0.5f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.515f, -0.35f, -0.5f);

	glEnd();

	glBegin(GL_QUADS);               //window left wall

	glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(-0.51f, -0.15f, -0.6f);  //right top

	glColor3f(0.6f, 0.8f, 1.0f);
	glVertex3f(-0.51f, -0.15f, -0.4f);  //left top

	glColor3f(0.6f, 0.8f, 1.0f);
	glVertex3f(-0.51f, -0.35f, -0.4f); //left bottom

	glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(-0.51f, -0.35f, -0.6f); // right bottom

	glEnd();

	glBegin(GL_LINES);                     // horizontal line left wall window

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.515f, -0.25f, -0.4f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.515f, -0.25f, -0.6f);

	glEnd();

	glBegin(GL_LINES);                     // vertical line left wall window

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.515f, -0.15f, -0.5f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.515f, -0.35f, -0.5f);

	glEnd();

}

void SmokeOutlet(void)
{

	// smoke outlet front face
	glBegin(GL_QUADS);

	glColor3f(0.59f, 0.59f, 0.59f);

	glVertex3f(0.4f, 0.4f, -0.2f);  //right top
	glVertex3f(0.3f, 0.4f, -0.2f);   //left top
	glVertex3f(0.3f, 0.3f, -0.2f);  //left bottom
	glVertex3f(0.4f, 0.2f, -0.2f);  // right bottom

	//smoke outlet right face

	glVertex3f(0.4f, 0.4f, -0.3f);  //right top
	glVertex3f(0.4f, 0.4f, -0.2f);  //left top
	glVertex3f(0.4f, 0.2f, -0.2f);  //left bottom
	glVertex3f(0.4f, 0.2f, -0.3f);  //right bottom

	//smoke outlet back face


	glVertex3f(0.4f, 0.4f, -0.3f);  //right top
	glVertex3f(0.3f, 0.4f, -0.3f);   //left top
	glVertex3f(0.3f, 0.3f, -0.3f);  //left bottom
	glVertex3f(0.4f, 0.2f, -0.3f);  // right bottom

	//smoke outlet left face
	glVertex3f(0.3f, 0.4f, -0.2f);  //right top
	glVertex3f(0.3f, 0.4f, -0.3f);  //left top
	glVertex3f(0.3f, 0.3f, -0.3f);  //left bottom
	glVertex3f(0.3f, 0.3f, -0.2f);  //right bottom




	glEnd();

	glLineWidth(5.0f);

	glBegin(GL_LINES);          //line front face

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.4f, 0.4f, -0.2f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.3f, 0.4f, -0.2f);

	glEnd();

	glBegin(GL_LINES);          //line right face

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.4f, 0.4f, -0.2f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.4f, 0.4f, -0.3f);

	glEnd();

	glBegin(GL_LINES);          //line back face

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.4f, 0.4f, -0.3f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.3f, 0.4f, -0.3f);

	glEnd();

	glBegin(GL_LINES);          //line left face

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.3f, 0.4f, -0.2f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.3f, 0.4f, -0.3f);

	glEnd();

	glLineWidth(1.0f);
}

void Door(void)
{
	float radious_door_handle = 0.01f;
	glBegin(GL_QUADS);               //door 

	glColor3f(0.3f, 0.2f, 0.0f);

	glVertex3f(0.1f, -0.2f, 0.0f);

	
	glVertex3f(-0.1f, -0.2f, 0.001f);

	
	glVertex3f(-0.1f, -0.6f, 0.001f);

	
	glVertex3f(0.1f, -0.6f, 0.001f);

	glEnd();

	glBegin(GL_LINES);                          // door handle 



	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = radious_door_handle * cos(angle) + (-0.0725);
		float y = radious_door_handle * sin(angle) + (-0.4);



		// Circle Origin

		glColor3f(1.0f, 0.9f, 0.0f);
		glVertex3f(-0.0725, -0.4, 0.001f);


		glColor3f(1.0f, 0.9f, 0.0f);
		glVertex3f(x, y, 0.001f);


	}

	glEnd();

}













