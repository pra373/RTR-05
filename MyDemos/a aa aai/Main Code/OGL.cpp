// Windows header files
// Common headers 
#include<windows.h> //For all WIN32 API's
#include<stdio.h>   //for file I/O
#include<stdlib.h>  //for exit()

#include "OGL.h" // swatachi header file

// OpenGL header Files
#include<gl/GL.h> 

#include<GL/glu.h>
//#include <GL/glew.h> 

//PNG loading header file
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

//timer header file

#include"helper_timer.h"



// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

//Scene transition macros

#define SCENE_ONE 4   //intro scene
#define SCENE_TWO 14  //chart
#define SCENE_THREE 16 //a aa aai
#define SCENE_FOUR 18  // maka
#define SCENE_FIVE 26  //mama
#define SCENE_SIX 35   //lotus
#define SCENE_SEVEN 42 //duck
#define SCENE_EIGHT 50 //gaadi 1
#define SCENE_NINE 54  //gaadi 2
#define SCENE_TEN 58   //baal
#define SCENE_ELEVAN 63 //credits
#define SCENE_TWELVE 67



// Link With OpenGL Library
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib") //file me OpenGL
#pragma comment(lib,"winmm.lib") //for play sound

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

//texture variables

GLuint intro_Texture = 0;
GLuint varnamala_Texture = 0;
GLuint aai_Texture = 0;
GLuint a_Texture = 0;
GLuint aa_Texture = 0;
GLuint maka_Texture = 0;
GLuint ma_Texture = 0;

GLuint mama1_Texture = 0;   //left top quad
GLuint mama2_Texture = 0;   //right bottom
GLuint mama3_Texture = 0;    //left bottom
GLuint mama4_Texture = 0;    //right top

GLuint lotus_Texture = 0;
GLuint ka_Texture = 0;

GLuint duck_Texture = 0;
GLuint ba_Texture = 0;

GLuint train_Texture = 0;
GLuint ga_Texture = 0;



GLuint train_Texture_2 = 0;
GLuint ga_Texture_2 = 0;

GLuint child1_Texture = 0;  
GLuint child2_Texture = 0; 

GLfloat xBoyImage = -1.5f;
GLfloat xGirlImage = 1.5f;

GLuint cr_Texture = 0;

GLuint thankyou_Texture = 0;






//Alphabet chart related variables

GLfloat C_Angle = 0.0f;

//quad size change variables

GLfloat xLeftBottom = 0.0f;
GLfloat yLeftBottom = 0.0f;

GLfloat xRightBottom = 0.0f;
GLfloat yRightBottom = 0.0f;

GLfloat xRightTop = 0.0f;
GLfloat yRightTop = 0.0f;

GLfloat xLeftTop = 0.0f;
GLfloat yLeftTop = 0.0f;

//quad rotation variables

GLfloat quadAngle = 0.0f;

//timer initialization

StopWatchInterface* sceneTimer = NULL;
float elapsedTime = 0.0f;

//Scene control variables

BOOL sceneOne = FALSE;
BOOL sceneTwo = FALSE;
BOOL sceneThree = FALSE;
BOOL sceneFour = FALSE;
BOOL sceneFive = FALSE;
BOOL sceneSix = FALSE;
BOOL sceneSeven = FALSE;
BOOL sceneEight = FALSE;
BOOL sceneNine = FALSE;
BOOL sceneTen = FALSE;
BOOL sceneElevan = FALSE;
BOOL sceneTwelve = FALSE;

//Animation variables

GLfloat yFirstQuad = 1.2f;
GLfloat ySecondQuad = 1.2f;
GLfloat yFirstQuadMaka = 1.2f;
GLfloat ySecondQuadMaka = 1.2f;

GLfloat xTopLeftQuad = -1.1f;
GLfloat xBottomRightQuad = 1.1f;
GLfloat xTopRightQuad = 1.4f;
GLfloat xBottomLeftQuad = -1.4f;

GLfloat yFirstQuadLotus = 1.2f;
GLfloat ySecondQuadLotus = 1.2f;

GLfloat yFirstQuadDuck = 1.2f;
GLfloat ySecondQuadDuck = 1.2f;

GLfloat yFirstQuadTrain2 = 1.2f;
GLfloat ySecondQuadTrain2 = 1.2f;

GLfloat yFirstQuadTrain = 1.2f;
GLfloat ySecondQuadTrain = 1.2f;
GLfloat xtrain = 0.0f;

GLfloat zChart = -15.0f;






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

	PlaySound(TEXT("Final.wav"), NULL, SND_FILENAME | SND_ASYNC);

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
	BOOL loadGLTexture(GLuint *, TCHAR[]);

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

	bResult = LoadGLPNGTexture(&intro_Texture, "Intro.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of Intro image failed !! Exiting now\n");
		return(-6);
	}

	bResult = bResult = LoadGLPNGTexture(&varnamala_Texture, "Varnamala.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of varnamala texture failed !! Exiting now\n");
		return(-6);
	}

	bResult = LoadGLPNGTexture(&aai_Texture, "aai.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of aai image texture failed !! Exiting now\n");
		return(-6);
	}

	bResult = LoadGLPNGTexture(&a_Texture, "Letter_a.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of letter a image texture failed !! Exiting now\n");
		return(-7);
	}

	bResult = LoadGLPNGTexture(&aa_Texture, "Letter_aa.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of letter aa image texture failed !! Exiting now\n");
		return(-7);
	}

	bResult = LoadGLPNGTexture(&maka_Texture, "CornField.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of Corn field image failed !! Exiting now\n");
		return(-6);
	}

	bResult = LoadGLPNGTexture(&ma_Texture, "Letter_ma.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of letter ma image failed !! Exiting now\n");
		return(-7);
	}

	bResult = LoadGLPNGTexture(&mama1_Texture, "MamaI1.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of mama image failed !! Exiting now\n");
		return(-6);
	}

	bResult = LoadGLPNGTexture(&mama2_Texture, "MamaI2.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of mama image failed !! Exiting now\n");
		return(-7);
	}

	bResult = LoadGLPNGTexture(&mama3_Texture, "MamaI3.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of letter mama image texture failed !! Exiting now\n");
		return(-7);
	}

	bResult = LoadGLPNGTexture(&mama4_Texture, "MamaI4.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of letter mama image texture failed !! Exiting now\n");
		return(-7);
	}

	bResult = LoadGLPNGTexture(&lotus_Texture, "Lotus2.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of Lotus image failed !! Exiting now\n");
		return(-6);
	}

	bResult = LoadGLPNGTexture(&ka_Texture, "Letter_ka.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of letter ma image failed !! Exiting now\n");
		return(-7);
	}

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

	bResult = LoadGLPNGTexture(&train_Texture, "Train.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of train image failed !! Exiting now\n");
		return(-6);
	}

	bResult = LoadGLPNGTexture(&ga_Texture, "Letter_ga.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of letter ga image failed !! Exiting now\n");
		return(-7);
	}

	bResult = LoadGLPNGTexture(&train_Texture_2, "Train_Scene.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of train image failed !! Exiting now\n");
		return(-6);
	}

	bResult = LoadGLPNGTexture(&ga_Texture_2, "Letter_ga.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of letter ga image failed !! Exiting now\n");
		return(-7);
	}

	bResult = LoadGLPNGTexture(&child1_Texture, "Child1.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of Child1 image failed !! Exiting now\n");
		return(-6);
	}

	bResult = LoadGLPNGTexture(&child2_Texture, "Child2.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of child2 image failed !! Exiting now\n");
		return(-7);
	}

	bResult = LoadGLPNGTexture(&cr_Texture, "CreditRole.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of credit role image failed !! Exiting now\n");
		return(-6);
	}

	bResult = LoadGLPNGTexture(&thankyou_Texture, "ThankYou.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of Thank You image failed !! Exiting now\n");
		return(-7);
	}





	

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
		if (height <= 0)
			height = 1;


		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glViewport(0, 0, (GLsizei)width, (GLsizei)height);

		gluPerspective(45.0f, ((GLfloat)width / (GLfloat)height), 0.1f, 100.0f);

	
	

}

void display(void)
{

	//function Declaration
	
	void introScene(void);
	void alphabetChartScene(void);
	void aaiScene(void);
	void makaScene(void);
	void mamaScene(void);
	void lotusScene(void);
	void duckScene(void);
	void trainScene1(void);
	void trainScene2(void);
	void baalScene(void);
	void endCredit(void);
	void thankYou(void);

	//code
	// 3rd Change GL_DEPTH_BUFFER_BIT
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	elapsedTime = sdkGetTimerValue(&sceneTimer);

	elapsedTime = elapsedTime / 1000;  //convert time in seconds

	if (elapsedTime < SCENE_ONE)
	{
		sceneOne = TRUE;
		introScene();
	}

	else if(elapsedTime >= SCENE_ONE && elapsedTime < SCENE_TWO)
	{
		sceneOne = FALSE;
		sceneTwo = TRUE;

		alphabetChartScene();
	}

	else if(elapsedTime >= SCENE_TWO && elapsedTime < SCENE_THREE)
	{
		sceneOne = FALSE;
		sceneTwo = FALSE;
		sceneThree = TRUE;

		aaiScene();
	}

	else if(elapsedTime >= SCENE_THREE && elapsedTime <SCENE_FOUR)
	{
		sceneOne = FALSE;
		sceneTwo = FALSE;
		sceneThree = FALSE;
		sceneFour = TRUE;

		makaScene();
	}

	else if (elapsedTime >= SCENE_FOUR && elapsedTime < SCENE_FIVE)
	{
		sceneOne = FALSE;
		sceneTwo = FALSE;
		sceneThree = FALSE;
		sceneFour = FALSE;
		sceneFive = TRUE;

		mamaScene();

	}

	else if (elapsedTime >= SCENE_FIVE && elapsedTime < SCENE_SIX)
	{
		sceneOne = FALSE;
		sceneTwo = FALSE;
		sceneThree = FALSE;
		sceneFour = FALSE;
		sceneFive = FALSE;
		sceneSix = TRUE;

		lotusScene();
	}

	else if (elapsedTime >= SCENE_SIX && elapsedTime < SCENE_SEVEN)
	{
		sceneOne = FALSE;
		sceneTwo = FALSE;
		sceneThree = FALSE;
		sceneFour = FALSE;
		sceneFive = FALSE;
		sceneSix = FALSE;
		sceneSeven = TRUE;

		duckScene();
	}

	else if (elapsedTime >= SCENE_SEVEN && elapsedTime < SCENE_EIGHT)
	{

		sceneOne = FALSE;
		sceneTwo = FALSE;
		sceneThree = FALSE;
		sceneFour = FALSE;
		sceneFive = FALSE;
		sceneSix = FALSE;
		sceneSeven = FALSE;
		sceneEight = TRUE;

		trainScene1();

	}

	else if (elapsedTime >= SCENE_EIGHT && elapsedTime < SCENE_NINE)
	{
		sceneOne = FALSE;
		sceneTwo = FALSE;
		sceneThree = FALSE;
		sceneFour = FALSE;
		sceneFive = FALSE;
		sceneSix = FALSE;
		sceneSeven = FALSE;
		sceneEight = FALSE;
		sceneNine = TRUE;

		trainScene2();
	}

	else if (elapsedTime >= SCENE_NINE && elapsedTime < SCENE_TEN)
	{
		sceneOne = FALSE;
		sceneTwo = FALSE;
		sceneThree = FALSE;
		sceneFour = FALSE;
		sceneFive = FALSE;
		sceneSix = FALSE;
		sceneSeven = FALSE;
		sceneEight = FALSE;
		sceneNine = FALSE;
		sceneTen = TRUE;

		baalScene();
	}

	else if (elapsedTime >= SCENE_TEN && elapsedTime < SCENE_ELEVAN)
	{
		sceneOne = FALSE;
		sceneTwo = FALSE;
		sceneThree = FALSE;
		sceneFour = FALSE;
		sceneFive = FALSE;
		sceneSix = FALSE;
		sceneSeven = FALSE;
		sceneEight = FALSE;
		sceneNine = FALSE;
		sceneTen = FALSE;
		sceneElevan = TRUE;

		endCredit();

	}

	/*else if (elapsedTime >= SCENE_ELEVAN && elapsedTime < SCENE_TWELVE)
	{
		sceneOne = FALSE;
		sceneTwo = FALSE;
		sceneThree = FALSE;
		sceneFour = FALSE;
		sceneFive = FALSE;
		sceneSix = FALSE;
		sceneSeven = FALSE;
		sceneEight = FALSE;
		sceneNine = FALSE;
		sceneTen = FALSE;
		sceneElevan = FALSE;
		sceneTwelve = TRUE;

		thankYou(); 

		}*/


	

	
	SwapBuffers(ghdc);

}


void update(void)
{
	// Alphabet chart related update code

	//code

	if (sceneTwo == TRUE)
	{
		/*if (xLeftBottom > -1.0f && yLeftBottom > -1.0f)
		{
			xLeftBottom = xLeftBottom - 0.0001f;
			yLeftBottom = yLeftBottom - 0.0001f;
		}

		if (xRightBottom < 1.0f && yRightBottom > -1.0f)
		{
			xRightBottom = xRightBottom + 0.0001f;
			yRightBottom = yRightBottom - 0.0001f;
		}

		if (xRightTop < 1.0f && yRightTop < 1.0f)
		{
			xRightTop = xRightTop + 0.0001f;
			yRightTop = yRightTop + 0.0001f;
		}

		if (xLeftTop > -1.0f && yLeftTop < 1.0f)
		{
			xLeftTop = xLeftTop - 0.0001f;
			yLeftTop = yLeftTop + 0.0001f;
		}*/

		if (quadAngle < 360.0f)
		{
			quadAngle = quadAngle + 0.3f;
		}

		if (zChart < -3.0f)
		{
			zChart = zChart + 0.01;
		}


	}

	//aai scene animation

	if (sceneThree == TRUE)
	{
		if (yFirstQuad > 0.5f)
		{
			yFirstQuad = yFirstQuad - 0.005f;
		}

		if (yFirstQuad <= 0.5f && ySecondQuad > 0.49f)
		{
			ySecondQuad = ySecondQuad - 0.005f;
		}
	}

	//maka scene animation

	if (sceneFour == TRUE)
	{
		if (yFirstQuadMaka > 0.5f)
		{
			yFirstQuadMaka = yFirstQuadMaka - 0.0073f;
		}

		if (yFirstQuadMaka <= 0.5f && ySecondQuadMaka > 0.49f)
		{
			ySecondQuadMaka = ySecondQuadMaka - 0.0073f;
		}
	}

	//mama scene

	if (sceneFive == TRUE)
	{
		if (xTopLeftQuad < 0.1f)
		{
			xTopLeftQuad = xTopLeftQuad + 0.003f;
		}

		if (xBottomRightQuad > -0.1f)
		{
			xBottomRightQuad = xBottomRightQuad - 0.003f;
		}

		if (xTopRightQuad > -0.1f && xTopLeftQuad >= 0.1)
		{
			xTopRightQuad = xTopRightQuad - 0.003f;
		}

		if (xBottomLeftQuad < 0.1f && xTopLeftQuad >= 0.1)
		{
			xBottomLeftQuad = xBottomLeftQuad + 0.003f;
		}


	}

	//lotus

	if (sceneSix == TRUE)
	{
		if (yFirstQuadLotus > 0.5f)
		{
			yFirstQuadLotus = yFirstQuadLotus - 0.0015f;
		}

		if (yFirstQuadLotus <= 0.5f && ySecondQuadLotus > 0.49f)
		{
			ySecondQuadLotus = ySecondQuadLotus - 0.0015f;
		}

	}

	//duck

	if (sceneSeven == TRUE)
	{
		if (yFirstQuadDuck > 0.5f)
		{
			yFirstQuadDuck = yFirstQuadDuck - 0.0015f;
		}

		if (yFirstQuadDuck <= 0.5f && ySecondQuadDuck > 0.49f)
		{
			ySecondQuadDuck = ySecondQuadDuck - 0.0015f;
		}
	}

	//train 1

	if (sceneEight == TRUE)
	{
		if (yFirstQuadTrain > 0.5f)
		{
			yFirstQuadTrain = yFirstQuadTrain - 0.0018f;
		}

		if (yFirstQuadTrain <= 0.5f && ySecondQuadTrain > 0.49f)
		{
			ySecondQuadTrain = ySecondQuadTrain - 0.0018f;
		}

		if (ySecondQuadTrain <= 0.49f && xtrain > -4.1f)
		{
			xtrain = xtrain - 0.0065f;
		}

	}

	//Train 2

	if (sceneNine == TRUE)
	{
		//code

		if (yFirstQuadTrain2 > 0.5f)
		{
			yFirstQuadTrain2 = yFirstQuadTrain2 - 0.003f;
		}

		if (yFirstQuadTrain2 <= 0.5f && ySecondQuadTrain2 > 0.49f)
		{
			ySecondQuadTrain2 = ySecondQuadTrain2 - 0.003f;
		}
	}

	//baal scene

	if (sceneTen == TRUE)
	{
		if (xBoyImage < 0.1f)
		{
			xBoyImage = xBoyImage + 0.008;

		}

		if (xBoyImage >= 0.1f && xGirlImage > -0.1f)
		{
			xGirlImage = xGirlImage - 0.008;
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

	if (intro_Texture)
	{
		glDeleteTextures(1, &intro_Texture);
		intro_Texture = 0;
	}

	//Alphabet chart related uninitialize code

	if (varnamala_Texture)
	{
		glDeleteTextures(1, &varnamala_Texture);
		varnamala_Texture = 0;
	}

	//Close The Log File
	if (gpFile)
	{
		fprintf(gpFile, "Program Ended Sucessfully...\n");
		fclose(gpFile);
		gpFile = NULL;
	}

	if (aai_Texture)
	{
		glDeleteTextures(1, &aai_Texture);
		aai_Texture = 0;
	}

	if (a_Texture)
	{
		glDeleteTextures(1, &a_Texture);
		a_Texture = 0;
	}

	if (aa_Texture)
	{
		glDeleteTextures(1, &aa_Texture);
		aa_Texture = 0;
	}

	if (mama1_Texture)
	{
		glDeleteTextures(1, &mama1_Texture);
		mama1_Texture = 0;
	}

	if (mama2_Texture)
	{
		glDeleteTextures(1, &mama2_Texture);
		mama2_Texture = 0;
	}

	if (thankyou_Texture)
	{
		glDeleteTextures(1, &thankyou_Texture);
		thankyou_Texture = 0;
	}

	if (mama3_Texture)
	{
		glDeleteTextures(1, &mama3_Texture);
		mama3_Texture = 0;
	}

	if (mama4_Texture)
	{
		glDeleteTextures(1, &mama4_Texture);
		mama4_Texture = 0;
	}

	if (maka_Texture)
	{
		glDeleteTextures(1, &maka_Texture);
		maka_Texture = 0;
	}

	if (ma_Texture)
	{
		glDeleteTextures(1, &ma_Texture);
		ma_Texture = 0;
	}

	if (train_Texture)
	{
		glDeleteTextures(1, &train_Texture);
		train_Texture = 0;
	}

	if (ga_Texture)
	{
		glDeleteTextures(1, &ga_Texture);
		ga_Texture = 0;
	}

	if (lotus_Texture)
	{
		glDeleteTextures(1, &lotus_Texture);
		lotus_Texture = 0;
	}

	if (ka_Texture)
	{
		glDeleteTextures(1, &ka_Texture);
		ka_Texture = 0;
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

	if (train_Texture_2)
	{
		glDeleteTextures(1, &train_Texture_2);
		train_Texture_2 = 0;
	}

	if (ga_Texture_2)
	{
		glDeleteTextures(1, &ga_Texture_2);
		ga_Texture_2 = 0;
	}


	if (train_Texture)
	{
		glDeleteTextures(1, &train_Texture);
		train_Texture = 0;
	}

	if (ga_Texture)
	{
		glDeleteTextures(1, &ga_Texture);
		ga_Texture = 0;
	}

	if (child1_Texture)
	{
		glDeleteTextures(1, &child1_Texture);
		child1_Texture = 0;
	}

	if (child2_Texture)
	{
		glDeleteTextures(1, &child2_Texture);
		child2_Texture = 0;
	}

	if (cr_Texture)
	{
		glDeleteTextures(1, &cr_Texture);
		cr_Texture = 0;
	}








	//Uninitialize timer

	sdkStopTimer(&sceneTimer);
	sdkDeleteTimer(&sceneTimer);
	sceneTimer = NULL;

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

void introScene(void)
{
	glTranslatef(0.0f, 0.0f, -2.0f);
	//glRotatef(C_Angle, 1.0f, 0.0f, 0.0f);              //when you use a single glRotatef() call and give (1.0) to every parameter OpenGL does arbitari axis rotation
	//glRotatef(C_Angle, 0.0f, 1.0f, 0.0f);              // when you use 3 Rotatef() calls i.e one for each axis, the result will be different.
	//glRotatef(C_Angle, 0.0f, 0.0f, 1.0f);


	// 4th Change use z-axis coordinate
	glBindTexture(GL_TEXTURE_2D, intro_Texture);
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
}

void alphabetChartScene(void)
{
	
	//glTranslatef(0.0f, 0.0f, -3.0f);
	glTranslatef(0.0f, 0.0f, zChart);
	

	glRotatef(quadAngle, 0.0f, 0.0f, 1.0f);
	// 4th Change use z-axis coordinate
	glBindTexture(GL_TEXTURE_2D, varnamala_Texture);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);  //left bottom
	//glVertex3f(xLeftBottom, yLeftBottom, 0.0f);  //left bottom

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);  //right bottom
	//glVertex3f(xRightBottom, yRightBottom, 0.0f);  //right bottom

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);   //right top
	//glVertex3f(xRightTop, yRightTop, 0.0f);   //right top

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);  //left top
	//glVertex3f(xLeftTop, yLeftTop, 0.0f);  //left top




	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void aaiScene(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glTranslatef(0.0f, 0.0f, -2.0f);
	//glRotatef(C_Angle, 1.0f, 0.0f, 0.0f);              //when you use a single glRotatef() call and give (1.0) to every parameter OpenGL does arbitari axis rotation
	//glRotatef(C_Angle, 0.0f, 1.0f, 0.0f);              // when you use 3 Rotatef() calls i.e one for each axis, the result will be different.
	//glRotatef(C_Angle, 0.0f, 0.0f, 1.0f);


	// 4th Change use z-axis coordinate

	//aai image drawing starts

	glBindTexture(GL_TEXTURE_2D, aai_Texture);
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

	glBindTexture(GL_TEXTURE_2D, a_Texture);

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


	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();



	//second quad starts

	glPushMatrix();

	//glTranslatef(0.47f, 0.49f, 0.0f);

	glTranslatef(0.47f, ySecondQuad, 0.0f);

	glBindTexture(GL_TEXTURE_2D, aa_Texture);

	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);  //left bottom

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.12f, 0.0f, 0.0f);  //right bottom

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.12f, 0.22f, 0.0f);  //right top

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.22f, 0.0f);  //left top

	glEnd();

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_BLEND);

}

void makaScene(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glTranslatef(0.0f, 0.0f, -2.0f);
	//glRotatef(C_Angle, 1.0f, 0.0f, 0.0f);              //when you use a single glRotatef() call and give (1.0) to every parameter OpenGL does arbitari axis rotation
	//glRotatef(C_Angle, 0.0f, 1.0f, 0.0f);              // when you use 3 Rotatef() calls i.e one for each axis, the result will be different.
	//glRotatef(C_Angle, 0.0f, 0.0f, 1.0f);


	// 4th Change use z-axis coordinate

	//corn field image drawing starts

	glBindTexture(GL_TEXTURE_2D, maka_Texture);
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

	glTranslatef(0.3f, yFirstQuadMaka, 0.0f);

	glBindTexture(GL_TEXTURE_2D, ma_Texture);

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

	glTranslatef(0.43f, ySecondQuadMaka, 0.0f);

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

}

void mamaScene(void)
{
	glBindTexture(GL_TEXTURE_2D, mama1_Texture);
	glPushMatrix();

	{
		//glTranslatef(0.1f, -0.1f, -2.0f);
		glTranslatef(xTopLeftQuad, -0.1f, -2.0f);



		//upper left

		glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-0.8f, 0.2f, 0.0f);  //left bottom


		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-0.2f, 0.2f, 0.0f);  //right bottom


		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-0.2f, 0.8f, 0.0f);   //right top


		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-0.8f, 0.8f, 0.0f);  //left top

		glEnd();

	}
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);

	//bottom left quad

	glBindTexture(GL_TEXTURE_2D, mama3_Texture);

	glPushMatrix();

	{
		//glTranslatef(0.1f, 0.1f, -2.0f);
		glTranslatef(xBottomLeftQuad, 0.1f, -2.0f);


		glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-0.8f, -0.2f, 0.0f);  //left bottom


		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-0.2f, -0.2f, 0.0f);  //right bottom


		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-0.2f, -0.8f, 0.0f);   //right top


		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-0.8f, -0.8f, 0.0f);  //left top

		glEnd();
	}
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);



	//top right quad

	glBindTexture(GL_TEXTURE_2D, mama4_Texture);

	glPushMatrix();

	{
		//glTranslatef(-0.1f, -0.1f, -2.0f);
		glTranslatef(xTopRightQuad, -0.1f, -2.0f);

		glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.8f, 0.2f, 0.0f);  //left bottom


		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.2f, 0.2f, 0.0f);  //right bottom


		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.2f, 0.8f, 0.0f);   //right top


		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.8f, 0.8f, 0.0f);  //left top

		glEnd();

	}

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);


	//bottom right

	glBindTexture(GL_TEXTURE_2D, mama2_Texture);
	glPushMatrix();
	{
		//glTranslatef(-0.1f, 0.1f, -2.0f);

		glTranslatef(xBottomRightQuad, 0.1f, -2.0f);

		glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.8f, -0.2f, 0.0f);  //left bottom


		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.2f, -0.2f, 0.0f);  //right bottom


		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.2f, -0.8f, 0.0f);   //right top


		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.8f, -0.8f, 0.0f);  //left top

		glEnd();

	}

	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();



}

void lotusScene(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glTranslatef(0.0f, 0.0f, -2.0f);
	


	// 4th Change use z-axis coordinate

	//corn field image drawing starts

	glBindTexture(GL_TEXTURE_2D, lotus_Texture);
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

	glTranslatef(0.3f, yFirstQuadLotus, 0.0f);

	glBindTexture(GL_TEXTURE_2D, ka_Texture);

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

	glTranslatef(0.43f, ySecondQuadLotus, 0.0f);

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

}

void duckScene(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glTranslatef(0.0f, 0.0f, -2.0f);
	

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

	glTranslatef(0.3f, yFirstQuadDuck, 0.0f);

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

	glTranslatef(0.43f, ySecondQuadDuck, 0.0f);

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

}

void trainScene1(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTranslatef(0.0f, 0.0f, -2.0f);

	glPushMatrix();
	glTranslatef(xtrain, -0.2f, -1.5f);
	//glRotatef(C_Angle, 1.0f, 0.0f, 0.0f);              //when you use a single glRotatef() call and give (1.0) to every parameter OpenGL does arbitari axis rotation
	//glRotatef(C_Angle, 0.0f, 1.0f, 0.0f);              // when you use 3 Rotatef() calls i.e one for each axis, the result will be different.
	//glRotatef(C_Angle, 0.0f, 0.0f, 1.0f);


	// 4th Change use z-axis coordinate

	//corn field image drawing starts

	glBindTexture(GL_TEXTURE_2D, train_Texture);
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

	glPopMatrix();

	//first quad starts

	glPushMatrix();

	//glTranslatef(0.3f, 0.5f, 0.0f);

	glTranslatef(0.3f, yFirstQuadTrain, 0.0f);

	glBindTexture(GL_TEXTURE_2D, ga_Texture);

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

	glTranslatef(0.43f, ySecondQuadTrain, 0.0f);

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

}

void trainScene2(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTranslatef(0.0f, 0.0f, -2.0f);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -0.5f);
	//glRotatef(C_Angle, 1.0f, 0.0f, 0.0f);              //when you use a single glRotatef() call and give (1.0) to every parameter OpenGL does arbitari axis rotation
	//glRotatef(C_Angle, 0.0f, 1.0f, 0.0f);              // when you use 3 Rotatef() calls i.e one for each axis, the result will be different.
	//glRotatef(C_Angle, 0.0f, 0.0f, 1.0f);


	// 4th Change use z-axis coordinate

	//corn field image drawing starts

	glBindTexture(GL_TEXTURE_2D, train_Texture_2);
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

	glPopMatrix();

	//first quad starts

	glPushMatrix();

	//glTranslatef(0.3f, 0.5f, 0.0f);

	glTranslatef(0.3f, yFirstQuadTrain2, 0.0f);

	glBindTexture(GL_TEXTURE_2D, ga_Texture_2);

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

	glTranslatef(0.43f, ySecondQuadTrain2, 0.0f);

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



}

void baalScene(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glBindTexture(GL_TEXTURE_2D, child1_Texture);
	glPushMatrix();

	{

		//glTranslatef(0.1f, 0.0f, -1.5f);
		glTranslatef(xBoyImage, 0.0f, -1.5f);



		//left quad

		glBegin(GL_QUADS);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-0.2f, 0.4f, 0.0f);  //right top


		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-0.8f, 0.4f, 0.0f);  //left top


		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-0.8f, -0.4f, 0.0f);   //left bottom


		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-0.2f, -0.4f, 0.0f);  //right bottom

		glEnd();

	}
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);

	//bottom left quad

	glBindTexture(GL_TEXTURE_2D, child2_Texture);

	glPushMatrix();

	{

		//glTranslatef(-0.1f, 0.0f, -1.5f);

		glTranslatef(xGirlImage, 0.0f, -1.5f);

		glBegin(GL_QUADS);

		//glTranslatef(0.1f, 0.1f, -2.0f);
		//glTranslatef(0.0f, 0.0f, -2.0f);


		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.2f, 0.4f, 0.0f);    //left top

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.2f, -0.4f, 0.0f);   //left bottom

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.8f, -0.4f, 0.0f);  //right bottom



		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.8f, 0.4f, 0.0f);  //right top

		glEnd();





	}
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);






	glDisable(GL_BLEND);

}

void endCredit(void)
{
	glBindTexture(GL_TEXTURE_2D, cr_Texture);
	glPushMatrix();

	{

		//glTranslatef(0.1f, 0.0f, -1.5f);
		glTranslatef(0.0f, 0.0f, -1.5f);



		//left quad

		glBegin(GL_QUADS);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.5f, 0.5f, 0.0f);  //right top


		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-0.5f, 0.5f, 0.0f);  //left top


		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-0.5f, -0.5f, 0.0f);   //left bottom


		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.5f, -0.5f, 0.0f);  //right bottom

		glEnd();

	}
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);



}

void thankYou(void)
{
	glTranslatef(0.0f, 0.0f, -2.0f);
	//glRotatef(C_Angle, 1.0f, 0.0f, 0.0f);              //when you use a single glRotatef() call and give (1.0) to every parameter OpenGL does arbitari axis rotation
	//glRotatef(C_Angle, 0.0f, 1.0f, 0.0f);              // when you use 3 Rotatef() calls i.e one for each axis, the result will be different.
	//glRotatef(C_Angle, 0.0f, 0.0f, 1.0f);


	// 4th Change use z-axis coordinate
	glBindTexture(GL_TEXTURE_2D, thankyou_Texture);
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
}











	

