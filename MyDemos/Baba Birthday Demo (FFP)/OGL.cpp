// Windows header files
// Common headers 
#include<windows.h> //For all WIN32 API's
#include<stdio.h>   //for file I/O
#include<stdlib.h>  //for exit()
#include<algorithm>
#include<random>
#include<iostream>
#include<stdarg.h>

using namespace std;

#include "OGL.h" // swatachi header file

// OpenGL header Files
#include<gl/GL.h> 

#include<GL/glu.h>

#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
#include"helper_timer.h"


#define _USE_MATH_DEFINES
#include<math.h>

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define HORIZONTAL_QUADS 87
#define VERTICAL_QUADS 87

// Link With OpenGL Library
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib") //file me OpenGL
#pragma comment(lib,"winmm.lib")

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

GLfloat C_Angle = 0.0f;

//texture variables

GLuint TEXTURE_BABA = 0;

//Vertex coordinates arrays

GLfloat rightTopX[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };
GLfloat rightTopY[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };

GLfloat leftTopX[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };
GLfloat leftTopY[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };

GLfloat leftBottomX[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };
GLfloat leftBottomY[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };

GLfloat rightBottomX[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };
GLfloat rightBottomY[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };

GLint numOfRows = VERTICAL_QUADS;
GLint numOfColumns = HORIZONTAL_QUADS;
GLint totalQuads = HORIZONTAL_QUADS * VERTICAL_QUADS;

//Texture Coordinates arrays

GLfloat tRightTopX[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };
GLfloat tRightTopY[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };

GLfloat tLeftTopX[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };
GLfloat tLeftTopY[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };

GLfloat tLeftBottomX[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };
GLfloat tLeftBottomY[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };

GLfloat tRightBottomX[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };
GLfloat tRightBottomY[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };

vector <int> visibilityNum;

GLint indexUpdate = 0;


GLint randomQuadNumber = 0;

//font variables

GLuint base;

GLYPHMETRICSFLOAT gmf[256];

//blend variables

GLfloat blendLetters = 0;

//timer initialization

StopWatchInterface* sceneTimer = NULL;
float elapsedTime = 0.0f;



// Entry-Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) 
{
	// Function declarations
	int initialize(void);
	void uninitialize(void);
	float display(void);
	void update(void);

	//Local Variable Declarations
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("Prathamesh Window");
	int iResult = 0;
	BOOL bDone = FALSE; //SDK cha BOOL ahe

	TCHAR caption[256] = { 0 };

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

	PlaySound(MAKEINTRESOURCE(MySound), NULL, SND_RESOURCE | SND_ASYNC);

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
					elapsedTime = display();
					sprintf(caption, " Prathamesh Paropkari                   Elapsed Time : %f", elapsedTime);
					SetWindowText(hwnd, caption);

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
	BOOL loadGLTexture(GLuint *, TCHAR[]);
	BOOL LoadGLPNGTexture(GLuint*, const char*);
	GLvoid BuildFont(GLvoid);

	// variable Declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormateIndex = 0;
	BOOL bResult = 0;

	GLint rowNum = 0;
	GLint columnNum = 0;

	GLint quadIndex = 0;

	GLint numOfIterationX = 1;
	GLint numOfIterationY = 1;
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Blue Color

	//BMP loading

	/*bResult = loadGLTexture(&TEXTURE_BABA, MAKEINTRESOURCE(BABAPHOTO));

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of Baba Image texture failed !! Exiting now\n");
		return(-6);
	}*/

	//png loading

	bResult = LoadGLPNGTexture(&TEXTURE_BABA, "BabaPhoto2.png");


	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of baba texture failed !! Exiting now\n");
		return(-7);
	}

	//nested for loops to calcuate vertex and tecture coordinates based on macro values entered above

	for (rowNum = 0; rowNum < numOfRows; rowNum++)
	{
		numOfIterationX = 1;

		for (columnNum = 0; columnNum < numOfColumns; columnNum++)
		{
			//for right top Vertex coordinates

			rightTopX[quadIndex] = ((2.0f) / (numOfColumns)) * numOfIterationX;
			rightTopY[quadIndex] = ((2.0f) / (numOfRows)) * numOfIterationY;

			//for right top Texture coordinates

			tRightTopX[quadIndex] = ((1.0f) / (numOfColumns)) * numOfIterationX;
			tRightTopY[quadIndex] = ((1.0f) / (numOfRows)) * numOfIterationY;

			//for left top Vertex coordinates

			leftTopX[quadIndex] = rightTopX[quadIndex] - ((2.0f) / (numOfColumns));
			leftTopY[quadIndex] = rightTopY[quadIndex];

			//for left top Texture coordinates

			tLeftTopX[quadIndex] = tRightTopX[quadIndex] - ((1.0f) / (numOfColumns));
			tLeftTopY[quadIndex] = tRightTopY[quadIndex];

			//for left bottom vertex coordinate

			leftBottomX[quadIndex] = leftTopX[quadIndex];
			leftBottomY[quadIndex] = leftTopY[quadIndex] - ((2.0f) / (numOfRows));

			//for left bottom texture coordinates

			tLeftBottomX[quadIndex] = tLeftTopX[quadIndex];
			tLeftBottomY[quadIndex] = tLeftTopY[quadIndex] - ((1.0f) / (numOfRows));

			//for right bottom vertex coordinates

			rightBottomX[quadIndex] = rightTopX[quadIndex];
			rightBottomY[quadIndex] = rightTopY[quadIndex] - ((2.0f) / (numOfRows));

			//for right bottom Texture coordinates

			tRightBottomX[quadIndex] = tRightTopX[quadIndex];
			tRightBottomY[quadIndex]= tRightTopY[quadIndex] - ((1.0f) / (numOfRows));

			//necessory variable updates

			numOfIterationX++;
			quadIndex++;
		}

		numOfIterationY++;

	}

	//create a vector to store visibility control numbers sequentially

	for (int i = 0; i < totalQuads; i++)
	{
		visibilityNum.push_back(i);
		fprintf(gpFile, "%d\n", visibilityNum[i]);
	}


	//seed the random number generator with the current time (Seconds passed since the unix epoch)

	default_random_engine rng(time(NULL));

	

	//shuffle the array of visibility control variables using shuffe function

	shuffle(visibilityNum.begin(), visibilityNum.end(), rng);



	fprintf(gpFile, "Vector Values After Shuffeling : \n\n");

	//print shuffed vector into log file

	for (int i = 0; i < totalQuads; i++)
	{
		
		fprintf(gpFile, "%d\n", visibilityNum[i]);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glEnable(GL_LIGHT0);

	glEnable(GL_LIGHTING);

	glEnable(GL_COLOR_MATERIAL);

	BuildFont();

	// create and start timer
	sdkCreateTimer(&sceneTimer);
	sdkStartTimer(&sceneTimer);
	

	glEnable(GL_TEXTURE_2D);
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

	gluPerspective(45.0f, ((GLfloat)width / (GLfloat)height), 0.1f, 100.0f);


}

float display(void)
{
	

	//function declarations
	GLvoid glPrint(const char* fmt, ...);


	//local variable declaration
	
	//code
	// 3rd Change GL_DEPTH_BUFFER_BIT
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	elapsedTime = sdkGetTimerValue(&sceneTimer);

	elapsedTime = elapsedTime / 1000;  //convert time in seconds

	if (elapsedTime > 1.6f)
	{
		glTranslatef(-1.0f, -1.0f, -4.5f);
		glScalef(1.0f, 1.3f, 1.0f);

		glDisable(GL_LIGHTING);

		glBindTexture(GL_TEXTURE_2D, TEXTURE_BABA);


		/*for (int i = 0; i <= (totalQuads - 1); i++)*/
		for (int i = 0; i < indexUpdate; i++)
		{


			//glBegin(GL_QUADS);

			////glTexCoord2f(1.0f, 1.0f); 
			//glColor3f(1.0f, 1.0f, 1.0f);
			//glVertex3f(rightTopX[i], rightTopY[i], 0.0f);  //right top

			////glTexCoord2f(0.0f, 1.0f);
			//glColor3f(0.5f, 0.0f, 0.0f);
			//glVertex3f(leftTopX[i], leftTopY[i], 0.0f);  //left top

			////glTexCoord2f(0.0f, 0.0f);
			//glColor3f(1.0f, 1.0f, 1.0f);
			//glVertex3f(leftBottomX[i], leftBottomY[i], 0.0f);  //left bottom

			////glTexCoord2f(1.0f, 0.0f);
			//glColor3f(0.5f, 0.0f, 0.0f);
			//glVertex3f(rightBottomX[i], rightBottomY[i], 0.0f);  //right bottom



			//glEnd();

			randomQuadNumber = visibilityNum[i];

			glBegin(GL_QUADS);

			glTexCoord2f(tRightTopX[randomQuadNumber], tRightTopY[randomQuadNumber]);
			glVertex3f(rightTopX[randomQuadNumber], rightTopY[randomQuadNumber], 0.0f);  //right top


			glTexCoord2f(tLeftTopX[randomQuadNumber], tLeftTopY[randomQuadNumber]);
			glVertex3f(leftTopX[randomQuadNumber], leftTopY[randomQuadNumber], 0.0f);  //left top


			glTexCoord2f(tLeftBottomX[randomQuadNumber], tLeftBottomY[randomQuadNumber]);
			glVertex3f(leftBottomX[randomQuadNumber], leftBottomY[randomQuadNumber], 0.0f);  //left bottom


			glTexCoord2f(tRightBottomX[randomQuadNumber], tRightBottomY[randomQuadNumber]);
			glVertex3f(rightBottomX[randomQuadNumber], rightBottomY[randomQuadNumber], 0.0f);  //right bottom



			glEnd();

			//glBegin(GL_QUADS);

			//glTexCoord2f(tRightTopX[i], tRightTopY[i]);
			//glVertex3f(rightTopX[i], rightTopY[i], 0.0f);  //right top


			//glTexCoord2f(tLeftTopX[i], tLeftTopY[i]);
			//glVertex3f(leftTopX[i], leftTopY[i], 0.0f);  //left top


			//glTexCoord2f(tLeftBottomX[i], tLeftBottomY[i]);
			//glVertex3f(leftBottomX[i], leftBottomY[i], 0.0f);  //left bottom


			//glTexCoord2f(tRightBottomX[i], tRightBottomY[i]);
			//glVertex3f(rightBottomX[i], rightBottomY[i], 0.0f);  //right bottom



			//glEnd();



		}

		glBindTexture(GL_TEXTURE_2D, 0);

		glEnable(GL_LIGHTING);

		//rgb(226, 66, 86)
		//glColor3f((226.0f/255.0f), (66.0f/255.0f), (86.0f/255.0f));
		glPushMatrix();
		{
			glScalef(0.5f, 0.5f, 0.5f);
			glTranslatef(2.1f, -1.0f, -1.5f);

			//glColor4f((115.0f / 255.0f), (22.0f / 255.0f), (62.0f / 255.0f), blendLetters);
			glColor4f(1.0f, 0.0f, 0.0f, blendLetters);
			glPrint("Happy Birthday Baba");

			glColor3f(1.0f, 1.0f, 1.0f);

		}

		glPopMatrix();


		

	}

	
	SwapBuffers(ghdc);

	return(elapsedTime);
	
	
}

BOOL loadGLTexture(GLuint *Texture, TCHAR image_Resource_ID[])
{
	//Local variable declarations
	HBITMAP hBitmap = NULL;
	BITMAP bmp;

	//work of OS to load image and extract data from image starts here
	//load the image - step5 in notebook
	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), image_Resource_ID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	if (hBitmap == NULL)
	{
		fprintf(gpFile, "Load image function failed !! Exiting now\n");
		return(FALSE);
	}

	//Get Image Data  - step 6
	GetObject(hBitmap, sizeof(BITMAP), &bmp);

	//work of OS to load image and extract data from image ends here
	// work of OPENGL starts
	//step 7 

	glGenTextures(1, Texture); // generate textures

	//step 8
	glBindTexture(GL_TEXTURE_2D, *Texture);

	//step 9

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//step 10

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


	//step 11

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, (void *)bmp.bmBits);

	//step 12
	glBindTexture(GL_TEXTURE_2D, 0);

	//step 13

	DeleteObject(hBitmap);
	hBitmap = NULL;
	return(TRUE);

}

void update(void)
{
	//code

	if (elapsedTime > 1.6f)
	{
		if (indexUpdate < visibilityNum.size())
		{
			indexUpdate += 1;
		}

		if (indexUpdate >= visibilityNum.size() && blendLetters < 1.0f)
		{
			blendLetters = blendLetters + 0.001f;
		}
	}

	
	
}


void uninitialize(void)
{
   // Function Declarations
	void ToggleFullScreen(void);
	GLvoid KillFont(GLvoid);
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

	if (TEXTURE_BABA)
	{
		glDeleteTextures(1, &TEXTURE_BABA);
		TEXTURE_BABA = 0;
	}

	//Close The Log File
	if (gpFile)
	{
		fprintf(gpFile, "Program Ended Sucessfully...\n");
		fclose(gpFile);
		gpFile = NULL;
	}

	KillFont();

}

BOOL LoadGLPNGTexture(GLuint* texture, const char* image)
{
	//variable declarations

	int iWidth, iHeight, iComponent;
	unsigned char* data = stbi_load(image, &iWidth, &iHeight, &iComponent, 0);

	if (!data)
	{
		fprintf(gpFile, "LoadingPNGTexture failed for %s:", image);
		stbi_image_free(data);
	}

	else
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		glGenTextures(1, texture);
		glBindTexture(GL_TEXTURE_2D, *texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		if (iComponent == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}

		else if (iComponent == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

		glBindTexture(GL_TEXTURE_2D, 0); //unbind texture

		//Delete object

		stbi_image_free(data);
		return(TRUE);
	}

	return FALSE;
}

GLvoid glPrint(const char* fmt, ...)
{
	float length = 0;
	char text[256];
	va_list ap; //for handeling variable arguments

	if (fmt == NULL)
	{
		return;
	}

	va_start(ap, fmt);  //initialize va_list to start the variable argument processing
	vsprintf(text, fmt, ap);  // formatting variable argument according to fmt

	va_end(ap);  //cleans the list

	for (unsigned int loop = 0; loop < (strlen(text)); loop++)
	{
		length += gmf[text[loop]].gmfCellIncX;
	}

	glTranslatef(-length / 2.0f, 0.0f, 0.0f);

	glPushAttrib(GL_LIST_BIT);

	glListBase(base);

	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);

	glPopAttrib();
}


GLvoid KillFont(GLvoid)
{
	glDeleteLists(base, 256);  //delete list
}

GLvoid BuildFont(GLvoid)
{
	HFONT font; // will hold window font ID

	base = glGenLists(256); //storage for 256 

	//create font

	font = CreateFont(-12, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "Comic Sans MS");

	//select font object

	SelectObject(ghdc, font);

	wglUseFontOutlines(ghdc, 0, 255, base, 0.0f, 0.2f, WGL_FONT_POLYGONS, gmf);
}











	

