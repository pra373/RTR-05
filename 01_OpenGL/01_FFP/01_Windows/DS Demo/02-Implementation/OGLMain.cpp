// Windows header files
// Common headers 
#include<windows.h> //For all WIN32 API's
#include<stdio.h>   //for file I/O
#include<stdlib.h>  //for exit()

#include "OGLMain.h" // swatachi header file //#include"../02-Implementation/Scenes/Common.h"   


//timer header file

#include"./External/helper_timer.h"

// OpenGL header Files
#include<gl/GL.h> 

#include<GL/glu.h>

//Header files for individual scenes

#include"./Scenes/Prastawana/Prastawana.h"
#include"./Scenes/Logo/Logo.h"
#include"./Scenes/TitleScene/TitleScene.h"
#include"./Scenes/EndCredits/EndCredits.h"
#include"./Scenes/Setu Bandha Re/Setu_Bandha_Re.h"
#include"./Scenes/Chimni_Scene/Chimni.h"
#include"./Scenes/AnimalsScene/Animals.h"
#include"./Scenes/BridgeScene/Bridge.h"



// MACROS


#define WIN_WIDTH 800
#define WIN_HEIGHT 600



// Link With OpenGL Library
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib") //file me OpenGL
#pragma comment(lib,"winmm.lib") // for PlaySound API

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

//project related variables


 int selected_Scene;
 float ELAPSED_TIME;


//timer

StopWatchInterface* timer = NULL;

//music control variable

BOOL musicEnable = FALSE;



// Entry-Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) 
{
	//Function declarations
	int initialize(void);
	void uninitialize(void);
	float display(HDC);
	void update(void);

	//Local Variable Declarations

	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("Prathamesh Window");
	int iResult = 0;
	BOOL bDone = FALSE; 

	//catch time

	float ElapsedTime = 0.0f;

	//caption to sprintf

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
							TEXT("Prathamesh Laxmikant Paropkari.            ELAPSED TIME : "),
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

	//sound API

	//PlaySound(TEXT("./Assets/Audio/SBRS.wav"), NULL, SND_FILENAME | SND_ASYNC);

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
					ElapsedTime = display(ghdc);
					sprintf(caption, " Prathamesh Paropkari                   Elapsed Time : %f", ElapsedTime);
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

	//BOOL loadGLTexture(GLuint *, TCHAR[]);
	// variable declaration
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Black Color

	//Initialize Opening scene 

	

	logoInitialize();
	fprintf(gpFile, "Calling initialize intro");

	
	titleInitialize(ghdc);     //title scene

	initializePrastawanaScene();  //prastawana scene

	initializeWanerSena();    //waner sena scene

	initializeChimini();     //chimni scene

	initializeAnimals();    //animals scene

	initializeBridgeScene(); //bridge scene

	initializeEndCredits(ghdc);

	

	

	

	

	//Timer initialization

	

	sdkCreateTimer(&timer);
	sdkStartTimer(&timer);

	//Initialize Audio

	/*BOOL bRet = initializeAudio();

	if (bRet == FALSE)
	{
		return(-6);
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

	gluPerspective(45.0f, ((GLfloat)width / (GLfloat)height), 0.1f, 100.0f);


}



float display(HDC ghdc)
{

	
	
	ELAPSED_TIME = sdkGetTimerValue(&timer);     //time in miliseconds
	ELAPSED_TIME = ELAPSED_TIME / 1000.0f;       //miliseconds to seconds conversion

	if (!musicEnable)
	{
		if (ELAPSED_TIME >= LOGO_SCENE_END)
		{
			PlaySound(TEXT("./Assets/Audio/SBRS(LM3).wav"), NULL, SND_FILENAME | SND_ASYNC);

			musicEnable = TRUE;
		}
		
	}

	//code
	// 3rd Change GL_DEPTH_BUFFER_BIT
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);          //original translate function

	//glTranslatef(0.0f, 0.0f, -1.0f);

	//Sequence of scenes based on elapsed time and story

	if (ELAPSED_TIME <= LOGO_SCENE_END)
	{
		selected_Scene = LOGO_SCENE;
	}

	else if(ELAPSED_TIME > LOGO_SCENE_END && ELAPSED_TIME <= TITLE_SCENE_END)
	{
		selected_Scene = TITLE_SCENE;
	}

	else if (ELAPSED_TIME > PRASTAWANA_SCENE_START && ELAPSED_TIME <= PRASTAWANA_SCENE_END)
	{
		selected_Scene = PRASTAWANA_SCENE;
	}

	else if (ELAPSED_TIME > WANER_SCENE_START && ELAPSED_TIME <= WANER_SCENE_END)
	{
		selected_Scene = SETU_BANDHA_RE;
	}

	else if (ELAPSED_TIME > ANIMALS_SCENE_START && ELAPSED_TIME <= ANIMALS_SCENE_END)
	{
		selected_Scene = ANIMALS_SCENE;
	}

	else if (ELAPSED_TIME > CHIMNI_SCENE_START && ELAPSED_TIME <= CHIMNI_SCENE_END)
	{
		selected_Scene = CHIMNI_SCENE;
	}

	else if (ELAPSED_TIME > RAMSETU_SCENE_START && ELAPSED_TIME <= RAMSETU_SCENE_END)
	{
		selected_Scene = BRIDGE_SCENE;
	}

	else if (ELAPSED_TIME > END_CREDIT_STARTS && ELAPSED_TIME <= END_CREDIT_END)
	{
		selected_Scene = ENDCREDITS;
	}

	//selected_Scene = SETU_BANDHA_RE;

	//selected_Scene = BRIDGE_SCENE;

	//selected_Scene = CHIMNI_SCENE;

	

	

	switch (selected_Scene)
	{
	case PRASTAWANA_SCENE:
		displayPrastawanaScene();
		break;

	case LOGO_SCENE:
		logoDisplay();
		break;

	case TITLE_SCENE:
		titleDisplay();
		break;

	case ENDCREDITS:
		displayEndCredits();
		break;

	case SETU_BANDHA_RE:
		displayWanerSena();
		break;

	case CHIMNI_SCENE:
		displayChimini();
		break;

	case ANIMALS_SCENE:
		displayAnimals();
		break;

	case BRIDGE_SCENE:
		displayBridgeScene();
		break;


	
	}
	
	


	SwapBuffers(ghdc);
	return(ELAPSED_TIME);

}

void update(void)
{

	if (ELAPSED_TIME <= LOGO_SCENE_END)  //0 to 8 sec
	{
		selected_Scene = LOGO_SCENE;
	}

	else if (ELAPSED_TIME > LOGO_SCENE_END && ELAPSED_TIME <= TITLE_SCENE_END)  //8 to 12 sec
	{
		selected_Scene = TITLE_SCENE;
	}

	else if (ELAPSED_TIME > PRASTAWANA_SCENE_START && ELAPSED_TIME <= PRASTAWANA_SCENE_END) //
	{
		selected_Scene = PRASTAWANA_SCENE;
	}

	else if (ELAPSED_TIME > WANER_SCENE_START && ELAPSED_TIME <= WANER_SCENE_END)
	{
		selected_Scene = SETU_BANDHA_RE;
	}

	else if (ELAPSED_TIME > ANIMALS_SCENE_START && ELAPSED_TIME <= ANIMALS_SCENE_END)
	{
		selected_Scene = ANIMALS_SCENE;
	}

	else if (ELAPSED_TIME > CHIMNI_SCENE_START && ELAPSED_TIME <= CHIMNI_SCENE_END)
	{
		selected_Scene = CHIMNI_SCENE;
	}

	else if (ELAPSED_TIME > RAMSETU_SCENE_START && ELAPSED_TIME <= RAMSETU_SCENE_END)
	{
		selected_Scene = BRIDGE_SCENE;
	}
	else if(ELAPSED_TIME > END_CREDIT_STARTS && ELAPSED_TIME <= END_CREDIT_END)
	{
		selected_Scene = ENDCREDITS;
	}
	

	//selected_Scene = SETU_BANDHA_RE;

	//selected_Scene = CHIMNI_SCENE;

	
	switch (selected_Scene)
	{
	case PRASTAWANA_SCENE:
		updatePrastawanaScene();
		break;
	case LOGO_SCENE:
		logoUpdate();
		break;

	case TITLE_SCENE:
		titleUpdate();
		break;

	case ENDCREDITS:
		updateEndCredits();
		break;

	case SETU_BANDHA_RE:
		updateWanerSena();
		break;

	case CHIMNI_SCENE:
		updateChimini();
		break;

	case ANIMALS_SCENE:
		updateAnimals();
		break;

	case BRIDGE_SCENE:
		updateBridgeScene();
		break;




	
	}

	
}



void uninitialize(void)
{
   // Function Declarations
	void ToggleFullScreen(void);

   //code

	//uninitialize all scenes

	
	uninitializeEndCredits();

	uninitializeBridgeScene();

	uninitializeAnimals();

	uninitializeChimini();

	uninitializeWanerSena();
	
	uninitializePrastawanaScene();

	titleUninitialize();
	
	
	logoUninitialize();

	

	

	//uninitializeAudio();

	

	

	

	

	if (timer)
	{
		sdkStopTimer(&timer);
		sdkDeleteTimer(&timer);

		timer = NULL;
	}

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













	

