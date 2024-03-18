// Windows header files
// Common headers 
#include<windows.h> //win32 IO
#include<stdio.h> //for file IO
#include<stdlib.h>  //for exit

#include "OGL.h" // swatachi header file

// OpenGL header Files
#include<gl/glew.h> // This must be before gl/GL.h
#include<gl/GL.h> 
#include"vmath.h"

//timer header file
 
#include"helper_timer.h"

//math.h header file 

//#define _USE_MATH_DEFINES
//#include<math.h>

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Link With OpenGL Library
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "OpenGL32.lib")


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

// shader related variables

GLuint vertexShader = 0;
GLuint fragmentShader = 0;
GLuint programObject = 0;

//vertex array object (VAO)

GLuint vao = 0;

//timer initialization

StopWatchInterface* sceneTimer = NULL;
float elapsedTime = 0.0f;

//timer control boolean variable

BOOL isTimerRunning = FALSE;

//caption to sprintf

TCHAR caption[256] = { 0 };

using namespace vmath;

// Entry-Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) 
{
	// Function declarations
	int initialize(void);
	void uninitialize(void);
	float display(HDC);
	void update(void);

	//Local Variable Declarations
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("Prathamesh Paropkari Window");
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
	/*gpFile = fopen("Log.txt", "w"); //small w mahatwacha
	if (gpFile == NULL)
	{
		MessageBox(NULL, TEXT("Log File Cannot Be Opened"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	fprintf(gpFile, "Program Started Successfully...\n\n");*/

	if (fopen_s(&gpFile, "Log.txt", "w")!= 0) // secure version of fopen()
	{
		MessageBox(NULL, TEXT("Log File Cannot Be Opened"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	fprintf(gpFile, "Program Started Successfully...\n\n");

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
							TEXT(" Prathamesh Laxmikant Paropkari : \t  moving Triangle"),
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
					elapsedTime = display(ghdc);
					sprintf(caption, "Prathamesh Laxmikant Paropkari. \t  moving Triangle program (Programable pipeline)            Elapsed Time : %f", elapsedTime);
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
	void printGLInfo(void);
	void resize(int, int);
	void uninitialize(void);

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

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		fprintf(gpFile, "glewInit() Failed To Initialize GLEW\n");
		return(-6);
	}

	// Call printGLInfo
	//printGLInfo();

	//vertex Shader source code

	static const GLchar* VertexShaderSourceCode =  
		"#version 460 core\n"
		"                 \n"
		"layout (location = 0) in vec4 offset;              \n"
		"layout (location = 1) in vec4 color;              \n"
		"                 \n"
		"out VS_OUT       \n"
		"{                \n"
		"   vec4 color;              \n"
		"} vs_out;              \n"
		"                 \n"		
		"void main(void)  \n"
		"{                \n"
		"  vs_out.color = color;             \n"           //assign ouput variable with input variable 'color'
		"  vec4 vertex1 = vec4(-0.2, -0.2, 0.0f, 1.0f); \n"  //position of vertex 1
		"  vec4 vertex2 = vec4(0.2, -0.2, 0.0f, 1.0f); \n"   //position of vertex 2
		"  vec4 vertex3 = vec4(0.0, 0.2, 0.0f, 1.0f); \n"    //position of vertex 3
		"\n"
		" if ( gl_VertexID == 0 ) "
		"{                                      \n"
        "  gl_Position = vertex1 + offset;               \n"
		"}                                      \n"
		"else if ( gl_VertexID == 1)            \n"
		"{                                      \n"
		"   gl_Position = vertex2 + offset;              \n"
		"}                                     \n"
		"else if ( gl_VertexID == 2)            \n"
		"{                                      \n"
		"   gl_Position = vertex3 + offset;              \n"
		" }                                     \n"
	 "}";

	//fragment shader source code

	static const GLchar* fragmentShaderSourceCode = "#version 460 core \n"
													"                 \n"
													"in VS_OUT        \n"
													"{                 \n"
													"    vec4 color;             \n"
													"}fs_in;                \n"
													"out vec4 color;  \n"
													"                 \n"
													"void main(void)  \n"
													"{                \n"
													"  color = fs_in.color; \n"
													"}";
	//create and compile vertex shader

	//creates empty shader object to copy the source code of vertex shader

	vertexShader = glCreateShader(GL_VERTEX_SHADER); 

	if (vertexShader == 0)
	{
		fprintf(gpFile, "glCreateShader() failed !! for vertex shader");
		return(-1);
	}

	//copies the shader source code into empty shader object

	glShaderSource(vertexShader, 1, (const GLchar**)&VertexShaderSourceCode, NULL);

	//compile the shader object

	glCompileShader(vertexShader);

	GLint status = 0;
	GLint infoLogLength = 0;
	GLchar* szInfoLog = NULL;

	//step 5 : Check for vertex shader compilation
	//step 5 (a) :  get compile status in status variable
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

	//step 5 (b) : if error occur i.e status == GL_FALSE,  get length of error message into infoLogLength variable
	if (status == GL_FALSE)
	{

		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			//step 5 (c) : declare a string variable allocate memory equal to above length i.e length of error message
			szInfoLog = (GLchar*)malloc(infoLogLength);


			if (szInfoLog != NULL)
			{
				// step 5 (d) : get the compilation error log info into memory allocated string
				glGetShaderInfoLog(vertexShader, infoLogLength, NULL, szInfoLog);

				//step 5 (e) : print error message into log file
				fprintf(gpFile, "Vertex shader compilation error log : %s\n", szInfoLog);


				//step 5 (f) : free allocated memory
				free(szInfoLog);
				szInfoLog = NULL;


				//step 5 (g) : uninitialize and exit.


			}

		}

		uninitialize();
	}


	//create and compile fragment shader

	//creates empty shader object to copy the source code of vertex shader

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	if (fragmentShader == 0)
	{
		fprintf(gpFile, "glCreateShader() failed !! for fragment shader");
		return(-1);
	}

	//copies the shader source code into empty shader object

	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

	//compile the shader object

	glCompileShader(fragmentShader);

	//create program object , attach shader objects to it and link it

	programObject = glCreateProgram();
	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);
	glLinkProgram(programObject);

	// do linking error check fpr shader program

	status = 0;
	infoLogLength = 0;
	szInfoLog = NULL;

	glGetProgramiv(programObject, GL_LINK_STATUS, &status);

	if (status == GL_FALSE)
	{
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar*)malloc(infoLogLength);


			if (szInfoLog != NULL)
			{
				glGetProgramInfoLog(programObject, infoLogLength, NULL, szInfoLog);
				fprintf(gpFile, "shader program linking error log : %s\n", szInfoLog);
				free(szInfoLog);
				szInfoLog = NULL;


			}



		}

		uninitialize();
	}



	//create a  vertex shader object (VAO). it represents vertex fetch stage in pipline

	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);



	//delete the shaders as program object has the code now in binary format

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);





		
	

	// 2nd change for depth or Enabling depth
	glClearDepth(1.0f); // Compulsory
	glEnable(GL_DEPTH_TEST); // 8 test paike ek(depth) test enable kr
	glDepthFunc(GL_LEQUAL); 
	

	// Set The ClearColor Of Window To Blue
	// Clear OpenGl Stars
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Blue Color
	resize(WIN_WIDTH, WIN_HEIGHT);

	//code

	return(0);
}

void printGLInfo(void)
{
	// variable declarations
	GLint numExtensions;
	GLint i;

	// code
	fprintf(gpFile, "OpenGL Vendor   : %s\n\n", glGetString(GL_VENDOR));

	fprintf(gpFile, "OpenGL Renderer : %s\n\n", glGetString(GL_RENDERER));

	fprintf(gpFile, "OpenGL Version  : %s\n\n", glGetString(GL_VERSION));

	fprintf(gpFile, "Graphic Library Shading Language  (GLSL) Version : %s\n\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Listing of supported extensions
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

	// Loop
	for (i = 0; i < numExtensions; i++)
	{
		fprintf(gpFile, "%s\n", glGetStringi(GL_EXTENSIONS, i));

	}



}

void resize(int width, int height)
{
	// code
	if(height <= 0)
		height = 1;


	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	

}

float display(HDC ghdc)
{
	//code

	//start timer

	if (isTimerRunning == FALSE)
	{
		// create and start timer
		sdkCreateTimer(&sceneTimer);
		sdkStartTimer(&sceneTimer);

		isTimerRunning = TRUE;
	}

	elapsedTime = sdkGetTimerValue(&sceneTimer);     //time in miliseconds
	elapsedTime = elapsedTime / 1000.0f;       //miliseconds to seconds conversion

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(programObject);

	glPointSize(1.0f);

	GLfloat attribVertex[] = { (float)sin(elapsedTime) * 0.5f,(float)cos(elapsedTime) * 0.6f,0.0f,0.0f };
	GLfloat attribColor[] = { ((float)sin(elapsedTime) + 0.5f),(-(float)sin(elapsedTime) + 0.5f),(-(float)cos(elapsedTime) + 0.5f),0.0f };
	

	//update the value of in offset variable in shader

	glVertexAttrib4fv(0, attribVertex);
	glVertexAttrib4fv(1, attribColor);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	SwapBuffers(ghdc);

	return(elapsedTime);

}

void update(void)
{
	//code
	//only at time of change animation here code will come
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

	if (vao)
	{
		glDeleteVertexArrays(1, &vao);
		vao = 0;
	}

	//Uninitialize timer

	if (sceneTimer)
	{
		sdkStopTimer(&sceneTimer);
		sdkDeleteTimer(&sceneTimer);
		sceneTimer = NULL;
	}

	

}












	

