// Windows header files
// Common headers 
#include<windows.h> //For all WIN32 API's
#include<stdio.h>   //for file I/O
#include<stdlib.h>  //for exit()

#include "OGL.h" // swatachi header file

#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

// OpenGL header Files
#include<gl/GL.h> 

#include<GL/glu.h>
//#include <GL/glew.h> 


// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

//#ifndef GL_CLAMP_TO_EDGE
//#define GL_CLAMP_TO_EDGE 0x812F
//#endif

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

GLfloat C_Angle = 0.0f;




//texture variables

GLuint duck_Texture = 0;
GLuint ba_Texture = 0;
//GLuint aa_Texture = 0;

//Animation variables

GLfloat yFirstQuad = 1.2f;
GLfloat ySecondQuad = 1.2f;


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
	//BOOL loadGLTexture(GLuint *, TCHAR[]);
	BOOL LoadGLPNGTexture(GLuint*, const char*);

	// variable Declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormateIndex = 0;
	BOOL bResult = 0;

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
	glClearColor((33.0f/256.0f), (33.0f / 256.0f), (33.0f / 256.0f), 1.0f); //Blue Color

	bResult = LoadGLPNGTexture(&duck_Texture, "Duck.png");
	
	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of duck image failed !! Exiting now\n");
		return(-6);
	}

	bResult = LoadGLPNGTexture(&ba_Texture, "Letter_ba.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of letter ma image failed !! Exiting now\n");
		return(-7);
	}

	/*bResult = LoadGLPNGTexture(&aa_Texture, "Letter_aa.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of letter aa image texture failed !! Exiting now\n");
		return(-7);
	}*/


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

	

	//gluPerspective(45.0f, ((GLfloat)width / (GLfloat)height), 0.1f, 100.0f);

	gluPerspective(45.0f, (800.0f / (GLfloat)height), 0.1f, 100.0f);


}

void display(void)
{

	
	// 3rd Change GL_DEPTH_BUFFER_BIT
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	
	glTranslatef(0.0f, 0.0f, -2.0f);
	//glRotatef(C_Angle, 1.0f, 0.0f, 0.0f);              //when you use a single glRotatef() call and give (1.0) to every parameter OpenGL does arbitari axis rotation
	//glRotatef(C_Angle, 0.0f, 1.0f, 0.0f);              // when you use 3 Rotatef() calls i.e one for each axis, the result will be different.
	//glRotatef(C_Angle, 0.0f, 0.0f, 1.0f);

	
	// 4th Change use z-axis coordinate

	//corn field image drawing starts

	glBindTexture(GL_TEXTURE_2D, duck_Texture);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);  //left bottom
	

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);  //right bottom
	

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);   //right top
	

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);  //left top

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

	//first quad starts

	glPushMatrix();

	//glTranslatef(0.3f, 0.5f, 0.0f);

	glTranslatef(0.3f, yFirstQuad, 0.0f);

	glBindTexture(GL_TEXTURE_2D, ba_Texture);

	glBegin(GL_QUADS);

	

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);  //left bottom

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.13f, 0.0f, 0.0f);  //right bottom

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.13f, 0.2f, 0.0f);  //right top

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.2f, 0.0f);  //left top

	glEnd();

	
	
	glPopMatrix();

	

	//second quad starts

	glPushMatrix();

	//glTranslatef(0.47f, 0.49f, 0.0f);

	glTranslatef(0.43f, ySecondQuad, 0.0f);

	//glBindTexture(GL_TEXTURE_2D, ma_Texture);

	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);  //left bottom

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.13f, 0.0f, 0.0f);  //right bottom

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.13f, 0.215f, 0.0f);  //right top

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.215f, 0.0f);  //left top

	glEnd();

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);
	
	glDisable(GL_BLEND);

	
	SwapBuffers(ghdc);

}

/*BOOL loadGLTexture(GLuint* Texture, TCHAR image_Resource_ID[])
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
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);



	//step 11

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, (void *)bmp.bmBits);

	//step 12
	glBindTexture(GL_TEXTURE_2D, 0);

	//step 13

	DeleteObject(hBitmap);
	hBitmap = NULL;
	return(TRUE);

}*/

void update(void)
{
	//code

	if (yFirstQuad > 0.5f)
	{
		yFirstQuad = yFirstQuad - 0.00035f;
	}

	if (yFirstQuad <= 0.5f && ySecondQuad > 0.49f)
	{
		ySecondQuad = ySecondQuad - 0.00035f;
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

	if (duck_Texture)
	{
		glDeleteTextures(1, &duck_Texture);
		duck_Texture = 0;
	}

	if (ba_Texture)
	{
		glDeleteTextures(1, &ba_Texture);
		ba_Texture = 0;
	}


	

	

	//Close The Log File
	if (gpFile)
	{
		fprintf(gpFile, "Program Ended Sucessfully...\n");
		fclose(gpFile);
		gpFile = NULL;
	}

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











	

