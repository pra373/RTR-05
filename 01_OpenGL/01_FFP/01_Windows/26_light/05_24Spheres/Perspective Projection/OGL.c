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

GLfloat lightAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
//GLfloat lightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightPosition[] = { 0.0f,0.0f,0.0f,1.0f };



//GLfloat materialAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
//GLfloat materialDiffuse[] = { 0.5f,0.2f,0.7f,1.0f };
//GLfloat materialSpecular[] = { 0.7f, 0.7f, 0.7f, 1.0f };
//GLfloat materialShininess[] = { 128.0f };

GLUquadric *quadric = NULL;

GLfloat angleForXRotation = 0.0f;
GLfloat angleForYRotation = 0.0f;
GLfloat angleForZRotation = 0.0f;

GLuint keyPressed = 0;

GLfloat WindowWidth;
GLfloat WindowHeight;

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

	case 'X':
	case 'x':
		keyPressed = 1;
		angleForXRotation = 0.0f;  //reset rotation
		break;

	case 'Y':
	case 'y':
		keyPressed = 2;
		angleForYRotation = 0.0f; //reset rotation
		break;

	case 'Z':
	case 'z':
		keyPressed = 3;
		angleForZRotation = 0.0f; //reset rotation
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
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f); //Dark grey Color


	//light related initialization

	//glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient); // enable first light out of 8 lights of OpenGL, give it ambient property as my array diclared globally
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	//glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);  //decide material propertise
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	//glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);

	//glEnable(GL_LIGHT0); //you enable light here. but if you dont provide normals of the object in display function you are not gonna see any any light on the screen
	//
	//initialize quadric

	quadric = gluNewQuadric();
	

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

	glEnable(GL_LIGHT0);
	resize(WIN_WIDTH, WIN_HEIGHT);  //warmup resize

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//code

	return(0);
}

void resize(int width, int height)
{
	// code
	if(height <= 0)
		height = 1;

	WindowHeight = height;
	WindowWidth = width;


	
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	
	gluPerspective(45.0f, ((GLfloat)width / (GLfloat)height), 0.1f, 100.0f);


}

void display(void)
{
	//code
	// 3rd Change GL_DEPTH_BUFFER_BIT

	//variable declarations

	GLfloat materialAmbient[4];
	GLfloat materialDiffuse[4];
	GLfloat materialSpecular[4];

	GLfloat Shininess;

	GLfloat horizontalDistance = WindowWidth / 6;
	GLfloat verticalDistance = WindowHeight / 7;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, (verticalDistance*5), (GLsizei)(WindowWidth/4), (GLsizei)(WindowHeight/4));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//transformation

	glTranslatef(0.0f, 0.0f, -1.0f);
	

	//animation
	
	if (keyPressed == 1)
	{
		glRotatef(angleForXRotation, 1.0f, 0.0f, 0.0f);
		lightPosition[2] = angleForXRotation;
	}

	if (keyPressed == 2)
	{
		glRotatef(angleForYRotation, 0.0f, 1.0f, 0.0f);
		lightPosition[0] = angleForYRotation;
	}

	if (keyPressed == 3)
	{
		glRotatef(angleForZRotation, 0.0f, 0.0f, 1.0f);
		lightPosition[1] = angleForZRotation;
	}

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);


	//First Column precious stones
	
	//(emerald)

	materialAmbient[0] = 0.0215;
	materialAmbient[1] = 0.1745;
	materialAmbient[2] = 0.0215;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.07568;
	materialDiffuse[1] = 0.61424;
	materialDiffuse[2] = 0.07568;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.633;
	materialSpecular[1] = 0.727811;
	materialSpecular[2] = 0.633;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.6 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5.0f);

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//Second Sphere 
	//jade

	materialAmbient[0] = 0.135;
	materialAmbient[1] = 0.2225;
	materialAmbient[2] = 0.1575; 
	materialAmbient[3] = 1.0f;   

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.54;
	materialDiffuse[1] = 0.89; 
	materialDiffuse[2] = 0.63;
	materialDiffuse[3] = 1.0f; 

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.316228;
	materialSpecular[1] = 0.316228;
	materialSpecular[2] = 0.316228;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.1 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport(0, (verticalDistance * 4), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0, -5.0f);

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//first column third sphere
	//obsidian


	materialAmbient[0] = 0.05375;
	materialAmbient[1] = 0.05;
	materialAmbient[2] = 0.06625;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.18275;
	materialDiffuse[1] = 0.17;
	materialDiffuse[2] = 0.22525;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.332741;
	materialSpecular[1] = 0.328634;
	materialSpecular[2] = 0.346435;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.3 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport(0, (verticalDistance * 3), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//first colomn fourth sphere
	//pearl

	materialAmbient[0] = 0.25;
	materialAmbient[1] = 0.20725;
	materialAmbient[2] = 0.20725;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 1.0;
	materialDiffuse[1] = 0.829;
	materialDiffuse[2] = 0.829;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.296648;
	materialSpecular[1] = 0.296648;
	materialSpecular[2] = 0.296648;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.088 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport(0, (verticalDistance * 2), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);

	
	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//first column fifth sphere
	//ruby


	materialAmbient[0] = 0.1745;
	materialAmbient[1] = 0.01175;
	materialAmbient[2] = 0.01175;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.61424;
	materialDiffuse[1] = 0.04136;
	materialDiffuse[2] = 0.04136;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.727811;
	materialSpecular[1] = 0.626959;
	materialSpecular[2] = 0.626959;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.6 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport(0, (verticalDistance), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);

	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//first column sixth sphere
	//turquoise


	materialAmbient[0] = 0.1;
	materialAmbient[1] = 0.18725;
	materialAmbient[2] = 0.1745;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.396;
	materialDiffuse[1] = 0.74151;
	materialDiffuse[2] = 0.69102;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.297254;
	materialSpecular[1] = 0.30829;
	materialSpecular[2] = 0.306678;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.1 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport(0, 0, (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);



	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	// ******************************** end of first column ****************************

	//second column first sphere
	//brass

	materialAmbient[0] = 0.329412;
	materialAmbient[1] = 0.223529;
	materialAmbient[2] = 0.027451;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.780392;
	materialDiffuse[1] = 0.568627;
	materialDiffuse[2] = 0.113725;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.992157;
	materialSpecular[1] = 0.941176;
	materialSpecular[2] = 0.807843;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.21794872 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance), (verticalDistance * 5), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);
	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//second column second sphere
	//bronze

	materialAmbient[0] = 0.2125;
	materialAmbient[1] = 0.1275;
	materialAmbient[2] = 0.054;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.714;
	materialDiffuse[1] = 0.568627;
	materialDiffuse[2] = 0.113725;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.992157;
	materialSpecular[1] = 0.941176;
	materialSpecular[2] = 0.807843;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.21794872 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance), (verticalDistance * 4), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);

	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//secong column third sphere
	//chrome


	materialAmbient[0] = 0.25;
	materialAmbient[1] = 0.25;
	materialAmbient[2] = 0.25;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.4;
	materialDiffuse[1] = 0.4;
	materialDiffuse[2] = 0.4;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.774597;
	materialSpecular[1] = 0.774597;
	materialSpecular[2] = 0.774597;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.6 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance), (verticalDistance * 3), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);
	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//second column fourth sphere
	//copper

	materialAmbient[0] = 0.19125;
	materialAmbient[1] = 0.0735;
	materialAmbient[2] = 0.0225;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.7038;
	materialDiffuse[1] = 0.27048;
	materialDiffuse[2] = 0.0828;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.256777;
	materialSpecular[1] = 0.137622;
	materialSpecular[2] = 0.086014;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.1 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance), (verticalDistance * 2), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);
	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//second column fifth sphere
	//gold


	materialAmbient[0] = 0.24725;
	materialAmbient[1] = 0.1995;
	materialAmbient[2] = 0.0745;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.75164;
	materialDiffuse[1] = 0.60648;
	materialDiffuse[2] = 0.22648;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.628281;
	materialSpecular[1] = 0.555802;
	materialSpecular[2] = 0.366065;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.4 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance), (verticalDistance), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//second column sixth sphere
	//silver


	materialAmbient[0] = 0.19225;
	materialAmbient[1] = 0.19225;
	materialAmbient[2] = 0.19225;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.50754;
	materialDiffuse[1] = 0.50754;
	materialDiffuse[2] = 0.50754;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.508273;
	materialSpecular[1] = 0.508273;
	materialSpecular[2] = 0.508273;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.4 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance), 0, (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);
	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere
	// ******************************* Second Column ends here ***************************
	
	//third column first sphere
	//black


	materialAmbient[0] = 0.0f;
	materialAmbient[1] = 0.0f;
	materialAmbient[2] = 0.0f;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.01;
	materialDiffuse[1] = 0.01;
	materialDiffuse[2] = 0.01;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.5;
	materialSpecular[1] = 0.5;
	materialSpecular[2] = 0.5;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.25 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance*2), (verticalDistance*5), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);
	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//third column second sphere
	//cyan


	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.1;
	materialAmbient[2] = 0.06;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.0;
	materialDiffuse[1] = 0.50980392;
	materialDiffuse[2] = 0.50980392;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.50196078;
	materialSpecular[1] = 0.50196078;
	materialSpecular[2] = 0.50196078;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.25 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance * 2), (verticalDistance * 4), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);
	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere
	
	//third column third sphere
	//green

	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.0;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.1;
	materialDiffuse[1] = 0.35;
	materialDiffuse[2] = 0.1;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.45;
	materialSpecular[1] = 0.55;
	materialSpecular[2] = 0.45;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.25 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance * 2), (verticalDistance * 3), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);
	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//third column fourth sphere
	//red

	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.0;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.5;
	materialDiffuse[1] = 0.0;
	materialDiffuse[2] = 0.0;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.7;
	materialSpecular[1] = 0.6;
	materialSpecular[2] = 0.6;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.25 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance * 2), (verticalDistance * 2), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//third column fifth sphere
	// white


	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.0;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.55;
	materialDiffuse[1] = 0.55;
	materialDiffuse[2] = 0.55;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.70;
	materialSpecular[1] = 0.70;
	materialSpecular[2] = 0.70;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.25 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance * 2), (verticalDistance ), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);
	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//third column sixth sphere
	//yello plastic


	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.0;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.5;
	materialDiffuse[1] = 0.5;
	materialDiffuse[2] = 0.0;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.60;
	materialSpecular[1] = 0.60;
	materialSpecular[2] = 0.50;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.25 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();
	
	glViewport((horizontalDistance * 2), (0), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	// ***************************** thied column ends here ****************

	//fourth column first sphere
	//black


	materialAmbient[0] = 0.02;
	materialAmbient[1] = 0.02;
	materialAmbient[2] = 0.02;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.01;
	materialDiffuse[1] = 0.01;
	materialDiffuse[2] = 0.01;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.4;
	materialSpecular[1] = 0.4;
	materialSpecular[2] = 0.4;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.078125 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance * 3), (verticalDistance*5), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//fourth column second sphere
	//cyan


	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.05;
	materialAmbient[2] = 0.05;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.4;
	materialDiffuse[1] = 0.5;
	materialDiffuse[2] = 0.5;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.04;
	materialSpecular[1] = 0.7;
	materialSpecular[2] = 0.7;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.078125 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance * 3), (verticalDistance * 4), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);
	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//fourth column third sphere
	//green


	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.05;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.4;
	materialDiffuse[1] = 0.5;
	materialDiffuse[2] = 0.4;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.04;
	materialSpecular[1] = 0.7;
	materialSpecular[2] = 0.04;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.078125 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();
	glViewport((horizontalDistance * 3), (verticalDistance * 3), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//fourth column fourth sphere
	//red


	materialAmbient[0] = 0.05;
	materialAmbient[1] = 0.0;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.5;
	materialDiffuse[1] = 0.4;
	materialDiffuse[2] = 0.4;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.7;
	materialSpecular[1] = 0.04;
	materialSpecular[2] = 0.04;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.078125 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance * 3), (verticalDistance * 2), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);
	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//fourth column fifth sphere
	//white


	materialAmbient[0] = 0.05;
	materialAmbient[1] = 0.05;
	materialAmbient[2] = 0.05;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.5;
	materialDiffuse[1] = 0.5;
	materialDiffuse[2] = 0.5;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.7;
	materialSpecular[1] = 0.7;
	materialSpecular[2] = 0.7;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.078125 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance * 3), (verticalDistance), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);
	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//fourth column sixth sphere
	//yellow rubber



	materialAmbient[0] = 0.05;
	materialAmbient[1] = 0.05;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.5;
	materialDiffuse[1] = 0.5;
	materialDiffuse[2] = 0.4;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.7;
	materialSpecular[1] = 0.7;
	materialSpecular[2] = 0.04;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.078125 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance * 3), (0), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);
	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere



















	//gluSphere(quadric, 0.2f, 50, 50);  //gluSphere call creates internally creates all needed normals for you
    
	// 4th Change use z-axis coordinate*/
	
	
	SwapBuffers(ghdc);

}

void update(void)
{
	//code

	if (keyPressed == 1)
	{
		angleForXRotation = angleForXRotation + 0.1f;

		if (angleForXRotation >= 360.0f)
		{
			angleForXRotation = 0.0f;
			
		}
	}

	if (keyPressed == 2)
	{
		angleForYRotation = angleForYRotation + 0.1f;

		if (angleForYRotation >= 360.0f)
		{
			angleForYRotation -= 360.0f;
		}
	}

	if (keyPressed == 3)
	{
		angleForZRotation = angleForZRotation + 0.1f;

		if (angleForZRotation >= 360.0f)
		{
			angleForZRotation -= 360.0f;
		}
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



/*
>cl.exe /c /EHsc window.c    => Compilation command
>rc.exe window.rc 			   => window.res tayar hote ya command ne
>link.exe window.obj window.res user32.lib gdi32.lib /SUBSYSTEM:WINDOWS  => linking command
*/








	

