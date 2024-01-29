// Windows header files
// Common headers 
#include<windows.h> //For all WIN32 API's
#include<stdio.h>   //for file I/O
#include<stdlib.h>  //for exit()

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

//GLfloat C_Angle = 0.0f;

//light variables

BOOL bLight = FALSE;

GLfloat gfLightAmbient[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat gfLightDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat gfLightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat gfLightPosition[] = { 0.0f,10.0f,-6.0f,1.0f };

GLfloat gflightSpotLightDirection[] = { 0.0f,-1.0f,0.0f,1.0f };

GLfloat materialAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat materialDiffuse[] = { 1.0f,0.0f,0.0f,1.0f };
GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialShininess[] = { 120.0f };

const GLfloat angleCutoff = 10.0f;
const GLfloat angleExponent = 2.0f;


GLUquadric *quadric = NULL;

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

	if(iResult != 0)
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
		while(bDone == FALSE)
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
LRESULT CALLBACK WndProc(HWND hwnd,UINT imsg,WPARAM wParam,LPARAM lParam)

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

	case 'L':
	case 'l':
		if (bLight == FALSE)
		{
			glEnable(GL_LIGHTING);
			bLight = TRUE;
		}

		else
		{
			glDisable(GL_LIGHTING);
			bLight = FALSE;
		}

		break;

	case 'W':
	case 'w':

		if (gfLightPosition[2] >= -15.0f)
		{
			gfLightPosition[2] -= 0.2f;
		}
			
		
		break;
	case 'S':
	case 's':

		if (gfLightPosition[2] <= -5.0f)
		{
			gfLightPosition[2] += 0.2f;
		}
			
		break;
	case 'A':
	case 'a':

		if (gfLightPosition[0] >= -4.0f)
		{
			gfLightPosition[0] -= 0.2f;
		}
		
		break;
	case'D':
	case'd':

		if (gfLightPosition[0] <= 4.0f)
		{
			gfLightPosition[0] = gfLightPosition[0] + 0.2f;
		}
			
		break;
	case 'R':
	case 'r':

		gfLightPosition[0] = 0.0f;
		gfLightPosition[2] = -6.0f;
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
	pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; // Syntax ky sangto :- 1)window vr draw kr 2)  3)
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits   = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits  = 8;
	pfd.cAlphaBits = 8;
	

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


	//light related initialization

	glLightfv(GL_LIGHT1, GL_AMBIENT, gfLightAmbient); // enable first light out of 8 lights of OpenGL, give it ambient property as my array diclared globally
	glLightfv(GL_LIGHT1, GL_DIFFUSE, gfLightDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, gfLightSpecular);
	glLightfv(GL_LIGHT1, GL_POSITION, gfLightPosition);

	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, gflightSpotLightDirection);

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, angleCutoff);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, angleExponent);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5f);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0f);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0f);

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);


	glEnable(GL_LIGHT1); //you enable light here. but if you dont provide normals of the object in display function you are not gonna see any any light on the screen
	
	//initialize quadric

	quadric = gluNewQuadric();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	resize(WIN_WIDTH, WIN_HEIGHT);

	//code

	return(0);
}

void resize(int width, int height)
{
	// code
	if(height <= 0)
		height = 1;

	
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	gluPerspective(45.0f,((GLfloat)width / (GLfloat)height),0.1f,100.0f);


}

void display(void)
{

	//function Prototype
	
	void DrawQuads(void);
	void PLP_DrawQuad(void);

	//code
	// 3rd Change GL_DEPTH_BUFFER_BIT

	glLightfv(GL_LIGHT1, GL_POSITION, gfLightPosition);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt(0.0f, 1.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	glTranslatef(0.0f, -3.0f, -4.0f);

	DrawQuads();

	
	
	SwapBuffers(ghdc);

}

void update(void)
{
	//code

	/*C_Angle = C_Angle + 0.04f;

	if (C_Angle >= 360.0f)
	{
		C_Angle = 0.0f;
	}*/
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

	if (quadric)
	{
		gluDeleteQuadric(quadric);
		quadric = NULL;
	}
	//Close The Log File
	if (gpFile)
	{
		fprintf(gpFile, "Program Ended Sucessfully...\n");
		fclose(gpFile);
		gpFile = NULL;
	}

}

void DrawQuads(void)
{
	glPushMatrix();
	{
		// first column
		materialDiffuse[0] = 1.0f; // r
		materialDiffuse[1] = 0.0f; // g
		materialDiffuse[2] = 0.0f; // b
		materialDiffuse[3] = 1.0f; // a
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		materialSpecular[0] = 1.0f;
		materialSpecular[1] = 0.0f;
		materialSpecular[2] = 0.0f;
		materialSpecular[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		glTranslatef(-4.0f, 0.0f, 0.0f);

		for (int i = 0; i < 10; i++)
		{
			glTranslatef(0.0f, 0.0f, -2.0f);
			PLP_DrawQuad();
		}
	}
	glPopMatrix();


	glPushMatrix();
	{
		// 2nd column
		materialDiffuse[0] = 1.0f; // r
		materialDiffuse[1] = 0.0f; // g
		materialDiffuse[2] = 0.0f; // b
		materialDiffuse[3] = 1.0f; // a
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		materialSpecular[0] = 1.0f; // r
		materialSpecular[1] = 0.5f; // g
		materialSpecular[2] = 0.0f; // b
		materialSpecular[3] = 1.0f;  //a
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		glTranslatef(-3.0f, 0.0f, 0.0f);
		for (int i = 0; i < 10; i++)
		{
			glTranslatef(0.0f, 0.0f, -2.0f);
			PLP_DrawQuad();
		}
	}
	glPopMatrix();

	// 3rd column
	glPushMatrix();
	{
		// 3rd column
		materialDiffuse[0] = 1.0f; // r
		materialDiffuse[1] = 1.0f; // g
		materialDiffuse[2] = 0.0f; // b
		materialDiffuse[3] = 1.0f; // a
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		materialSpecular[0] = 1.0f; // r
		materialSpecular[1] = 1.0f; // g
		materialSpecular[2] = 0.0f; // b
		materialSpecular[3] = 1.0f;  //a
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		glTranslatef(-1.5f, 0.0f, 0.0f);
		for (int i = 0; i < 10; i++)
		{
			glTranslatef(0.0f, 0.0f, -2.0f);
			PLP_DrawQuad();
		}
	}
	glPopMatrix();

	glPushMatrix();
	{
		// 4th column
		materialDiffuse[0] = 0.0f; // r
		materialDiffuse[1] = 1.0f; // g
		materialDiffuse[2] = 0.0f; // b
		materialDiffuse[3] = 1.0f; // a
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		materialSpecular[0] = 0.0f; // r
		materialSpecular[1] = 1.0f; // g
		materialSpecular[2] = 0.0f; // b
		materialSpecular[3] = 1.0f;  //a
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		glTranslatef(0.0f, 0.0f, 0.0f);

		for (int i = 0; i < 10; i++)
		{
			glTranslatef(0.0f, 0.0f, -2.0f);
			PLP_DrawQuad();
		}
	}
	glPopMatrix();

	glPushMatrix();
	{
		// 5th column
		materialDiffuse[0] = 0.0f; // r
		materialDiffuse[1] = 0.0f; // g
		materialDiffuse[2] = 1.0f; // b
		materialDiffuse[3] = 1.0f; // a
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		materialSpecular[0] = 0.0f; // r
		materialSpecular[1] = 0.0f; // g
		materialSpecular[2] = 1.0f; // b
		materialSpecular[3] = 1.0f;  //a
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		glTranslatef(1.5f, 0.0f, 0.0f);
		for (int i = 0; i < 10; i++)
		{
			glTranslatef(0.0f, 0.0f, -2.0f);
			PLP_DrawQuad();
		}
	}
	glPopMatrix();

	glPushMatrix();
	{
		// 6th column
		materialDiffuse[0] = 0.294f; // r
		materialDiffuse[1] = 0.0f;   // g
		materialDiffuse[2] = 0.509f; // b
		materialDiffuse[3] = 1.0f;   // a
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		materialSpecular[0] = 0.294f; // r
		materialSpecular[1] = 0.0f;   // g
		materialSpecular[2] = 0.509f; // b
		materialSpecular[3] = 1.0f;  //a
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		glTranslatef(3.0f, 0.0f, 0.0f);
		for (int i = 0; i < 10; i++)
		{
			glTranslatef(0.0f, 0.0f, -2.0f);
			PLP_DrawQuad();
		}
	}
	glPopMatrix();

	glPushMatrix();
	{
		// 7th column
		materialDiffuse[0] = 0.937f; // r
		materialDiffuse[1] = 0.509f;   // g
		materialDiffuse[2] = 0.937f; // b
		materialDiffuse[3] = 1.0f;   // a
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		materialSpecular[0] = 0.937f; // r
		materialSpecular[1] = 0.509f;   // g
		materialSpecular[2] = 0.937f; // b
		materialSpecular[3] = 1.0f;  //a
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		glTranslatef(4.5f, 0.0f, 0.0f);
		for (int i = 0; i < 10; i++)
		{
			glTranslatef(0.0f, 0.0f, -2.0f);
			PLP_DrawQuad();
		}
	}
	glPopMatrix();
	



}

void PLP_DrawQuad(void)
{
	glPushMatrix();
	{
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glEnd();
	}
	glPopMatrix();
}












	

