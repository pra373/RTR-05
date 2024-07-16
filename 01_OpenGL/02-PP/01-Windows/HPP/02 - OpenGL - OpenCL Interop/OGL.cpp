// Windows header files
// Common headers 
#include<windows.h> //win32 IO
#include<stdio.h> //for file IO
#include<stdlib.h>  //for exit

#include "OGL.h" // swatachi header file

// OpenGL header Files
#include<gl/glew.h> // This must be before gl/GL.h
#include<gl/GL.h> 

#include "vmath.h"
using namespace vmath;

// OpenCL InterOperability related header file
#include <CL/opencl.h>

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Link With OpenGL Library
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "OpenGL32.lib")

#define CL_TARGET_OPENCL_VERSION 200

// OpenCL related library
#pragma comment(lib, "opencl.lib")


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

// OpenGL related variables
GLuint shaderProgramObject = 0;
enum
{
	AMC_ATTRIBUTE_POSITION = 0
};

GLuint vao = 0;

GLuint vbo_position = 0;

GLuint mvpMatrixUniform = 0;

mat4 perspectiveProjectionMatrix; // mat4 data type is in vmath.h 

// OpenCL Inter Oprability Related Global Variables

#define MESH_ARRAY_SIZE 1024 * 1024 * 4

int mesh_width = 1024;
int mesh_height = 1024;

float position[1024][1024][4];  // 3D array

GLuint vbo_GPU;

BOOL bOnGPU = FALSE;

float animationTime = 0.0f;

cl_device_id oclDeviceId;

cl_context oclContext = NULL;

cl_command_queue oclCommandQueue = NULL;

cl_program oclProgram = NULL;

cl_kernel oclKernel = NULL;

cl_mem ocl_graphics_resource = NULL;

cl_int oclResult;


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
	TCHAR szAppName[] = TEXT("PLPWindow");
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
							TEXT("Prathamesh Paropkari   :   OpenCL Live Code 2 May 2024"),
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
	case 'C':
	case 'c':
		bOnGPU = FALSE;
		break;
	case 'G':
	case 'g':
		bOnGPU = TRUE;
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

	// code

	// Function Declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormateIndex = 0;
    
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	// Initialization of PIXELFORMATDESCRIPTOR
	pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; // Syntax ky sangto :- 1)window vr draw kr 
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

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		fprintf(gpFile, "glewInit() Failed To Initialize GLEW\n");
		return(-6);
	}

	// Call printGLInfo
	printGLInfo();


	// check OpenCL support and if cuda support select defaulted OpenCL device..

	// variables declarations
	cl_platform_id oclPlatformId;

	cl_uint dev_count;
	 
	cl_device_id* oclDeviceIds = NULL;

	// Get Default OpenCL Platform Id

	oclResult = clGetPlatformIDs(1, &oclPlatformId, NULL);

	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clGetPlatformIDs() Function Failed\n");

		uninitialize();
		exit(0);
	}

	// Get OpenCL Device Count

	oclResult = clGetDeviceIDs(oclPlatformId, CL_DEVICE_TYPE_GPU, 0, NULL, &dev_count);

	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clGetDeviceIDs() Function Failed To Get Device Count\n");

		uninitialize();
		exit(0);
	}
	else if (dev_count == 0)
	{
		fprintf(gpFile, "There are no OpenCL Supported Device\n");

		uninitialize();
		exit(0);
	}
	else
	{
		oclDeviceIds = (cl_device_id*)malloc(sizeof(cl_device_id) * dev_count);

		if (oclDeviceIds == NULL)
		{
			fprintf(gpFile, "Memory allocation failure for OpenCl\n");

			uninitialize();
			exit(0);
		}


		// Fill The OpenCL Device IDs Array 

		oclResult = clGetDeviceIDs(oclPlatformId, CL_DEVICE_TYPE_GPU, dev_count, oclDeviceIds, NULL);

		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFile, "clGetDeviceIds() Function Failed To Fill OpenCL DeviceIDs Array\n");

			uninitialize();
			exit(0);
		}

		// Set 0th Device as OpenCL

		oclDeviceId = oclDeviceIds[0];

		/*free(oclDeviceId)
		{
			oclDeviceId = 0;
		}*/

		// Declare OpenCL Context Properties Array

		cl_context_properties oclContextProperties[] =
		{
			CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
			CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
			CL_CONTEXT_PLATFORM, (cl_context_properties)oclPlatformId,
			0

		};


		// Create OpenCL Context

		oclContext = clCreateContext(oclContextProperties, 1, &oclDeviceId, NULL, NULL, &oclResult);

		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFile, "clCreateContext() Function Failed\n");

			uninitialize();
			exit(0);
		}

		// Create OpenCL Command Queue

		oclCommandQueue = clCreateCommandQueue(oclContext, oclDeviceId, 0, &oclResult);

		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFile, "clCreateCommandQueue() Function Failed\n");

			uninitialize();
			exit(0);
		}


		// Declare OpenCL Kernel Code

		const char* oclKernelSourceCode =
			"__kernel void sineWaveKernel1(__global float4 * pos, int width, int height, float time)"\
				"{"\
				"int x = get_global_id(0);"\
				"int y = get_global_id(1); "\
				"float u = (float)x / (float)width;"\
				"float v = (float)y / (float)height;"\
				"u = u * 2.0f - 1.0f;"\
				"v = v * 2.0f - 1.0f;"\
				"float frequency = 4.0f;"\
				"float w = sin(u * frequency + time) * cos(v * frequency + time) * 0.5f;"\
				"pos[y * width + x] = (float4)(u, w, v, 1.0f);"\
				"}";


		// From above kernel source code create OpenCL Program

		oclProgram = clCreateProgramWithSource(oclContext, 1, (const char**)&oclKernelSourceCode, NULL, &oclResult);

		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFile, "clCreateProgramWithSource() Function Failed\n");

			uninitialize();
			exit(0);
		}

		// Build Above OpenCL Program

		oclResult = clBuildProgram(oclProgram, 0, NULL, "-cl-fast-relaxed-math", NULL, NULL);

		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFile, "clBuildProgram() Function Failed\n");

			// Get Program Build Error

			cl_int oclBuildResult;

			char szBuffer[1024];

			oclBuildResult = clGetProgramBuildInfo(oclProgram, oclDeviceId, CL_PROGRAM_BUILD_LOG, sizeof(szBuffer), szBuffer, NULL);

			if (oclBuildResult != CL_SUCCESS)
			{
				fprintf(gpFile, "clGetProgramBuildInfo() Function Failed\n");

			}
			else
			{
				fprintf(gpFile, "OpenCL Program Build Error :  %s\n", szBuffer);
			}


			uninitialize();
			exit(0);
		}


		// Now Create OpenCL Kernel

		oclKernel = clCreateKernel(oclProgram, "sineWaveKernel1", NULL);
		
			if (oclResult != CL_SUCCESS)
			{
				fprintf(gpFile, "clCreateKernel() Function Failed\n");

				uninitialize();
				exit(0);
			}
	}



	// Vertex Sheader
	const GLchar* vertexShaderSourceCode =
		"#version 460 core" \
		"\n" \
		"in vec4 aPosition;" \
		"uniform mat4 uMVPMatrix;" \
		"void main(void)" \
		"{" \
		"gl_Position= uMVPMatrix*aPosition;" \
		"}";

	// Step 2 -> 
	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	// Step 3
	glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);

	// Step 4
	glCompileShader(vertexShaderObject);

	// Step 5
	GLint status = 0;
	GLint infoLogLength = 0;
	GLchar *szInfoLog = NULL;

	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);
  
	if (status == GL_FALSE)
	{
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar*) malloc(infoLogLength);

			if (szInfoLog != NULL)
			{
				glGetShaderInfoLog(vertexShaderObject, infoLogLength, NULL, szInfoLog);

				fprintf(gpFile, "vertexShader Compilation Error Log : %s\n", szInfoLog);

				free(szInfoLog);
				szInfoLog = NULL;
				//uninitialize();
			}
		}

		uninitialize();

	}


	// Fragment Shader
	const GLchar* fragmentShaderSourceCode =
		"#version 460 core" \
		"\n" \
		"out vec4 FragColor;" \
		"void main(void)" \
		"{" \
		"FragColor=vec4(1.0, 0.5, 0.0, 1.0);" \
		"}";


	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShaderObject, 1, (const GLchar**) &fragmentShaderSourceCode, NULL);

	glCompileShader(fragmentShaderObject);

    status = 0;
	infoLogLength = 0;
	szInfoLog = 0;

	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar*)malloc(infoLogLength);

			if (szInfoLog != NULL)
			{
				glGetShaderInfoLog(fragmentShaderObject, infoLogLength, NULL, szInfoLog);

				fprintf(gpFile, "Fragment Shader Compilation Error Log : %s\n", szInfoLog);

				free(szInfoLog);
				szInfoLog = NULL;
				//uninitialize();
			}
		}
		uninitialize();
	}

	// Shader Program
	shaderProgramObject = glCreateProgram();

	// 
	glAttachShader(shaderProgramObject, vertexShaderObject);

	glAttachShader(shaderProgramObject, fragmentShaderObject);

	glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_POSITION, "aPosition");

	glLinkProgram(shaderProgramObject);


	status = 0;
	infoLogLength = 0;
	szInfoLog = 0;

	glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);

	if (status = GL_FALSE)
	{
		glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar*)malloc(infoLogLength);

			if (szInfoLog != NULL)
			{
				glGetProgramInfoLog(shaderProgramObject, infoLogLength, NULL, szInfoLog);

				fprintf(gpFile, "Shader Program Linking Error Log : %s\n", szInfoLog);

				free(szInfoLog);
				szInfoLog = NULL;
				//uninitialize();
			}
		}

		uninitialize();
	}

	// Get Shader Uniform Locations
	mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "uMVPMatrix");

	for (int i = 0; i < mesh_width; i++)
	{
		for (int j = 0; j < mesh_height; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				position[i][j][k] = 0.0f;
			}
		}
	}


	// vao
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);


	// VBO for CPU
	glGenBuffers(1, &vbo_position);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_position);

	glBufferData(GL_ARRAY_BUFFER, MESH_ARRAY_SIZE * sizeof(float), NULL, GL_DYNAMIC_DRAW);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// VBO for GPU *********************************

	glGenBuffers(1, &vbo_GPU);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_GPU);

	glBufferData(GL_ARRAY_BUFFER, MESH_ARRAY_SIZE * sizeof(float), NULL, GL_DYNAMIC_DRAW);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind with Vao
	glBindVertexArray(0);


	// Perpare OpenCL Graphics Resource For Inter Operability 

	ocl_graphics_resource = clCreateFromGLBuffer(oclContext, CL_MEM_WRITE_ONLY, vbo_GPU, &oclResult);

	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clCreateFromGLBuffer() Function Failed\n");

		uninitialize();
		exit(0);
	}



	// For Depth
	// 2nd change for depth or Enabling depth
	glClearDepth(1.0f); // Compulsory
	glEnable(GL_DEPTH_TEST); // 8 test paike ek(depth) test enable kr
	glDepthFunc(GL_LEQUAL); 
	
	// Set The ClearColor Of Window 
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	
	// initialize orthoGraphicProjectionMatrix
	perspectiveProjectionMatrix = vmath::mat4::identity();

	resize(WIN_WIDTH, WIN_HEIGHT); // warmup resize

	//code

	return(0);
}

void printGLInfo(void)
{
	// variable declarations
	GLint numExtensions;
	GLint i;

	// code
	fprintf(gpFile, "OpenGL Vendor : %s\n\n", glGetString(GL_VENDOR));

	fprintf(gpFile, "OpenGL Renderer : %s\n\n", glGetString(GL_RENDERER));

	fprintf(gpFile, "OpenGL Version : %s\n\n", glGetString(GL_VERSION));

	fprintf(gpFile, "Graphic Library Shading Language  (GLSL) Version : %s\n\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	fprintf(gpFile, "*********************************************************************************\n");

	// Listing of supported extensions
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

	// Loop
	for (i = 0; i < numExtensions; i++)
	{
		fprintf(gpFile, "%s\n", glGetStringi(GL_EXTENSIONS, i));

	}

	fprintf(gpFile,"*********************************************************************************\n");


}

void resize(int width, int height)
{
	// code
	if(height <= 0)
		height = 1;


	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	// set perspective projection matrix
	perspectiveProjectionMatrix = vmath::perspective(
		45.0f,
		((GLfloat)width / (GLfloat)height),
		0.1f,
		100.0f);
		

}

void display(void)
{

	// Function Declarations
	void launchCPUKernel(int, int, float);
	void uninitialize(void);

	//code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(shaderProgramObject);

	// Transformation
	mat4 modelViewMatrix = mat4::identity();

	mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix; // order of multiplication is very important..

	// push above mvp into vertex shader's mvpUniform
	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	glBindVertexArray(vao);

	if (bOnGPU == TRUE)
	{
		// set kernel arguments

		oclResult = clSetKernelArg(oclKernel, 0, sizeof(cl_mem), (void*)&ocl_graphics_resource);

		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFile, "clSetKernelArg() Function Failed For 0th Argument\n");

			uninitialize();
			exit(0);
		}

		// 1st argument
		oclResult = clSetKernelArg(oclKernel, 1, sizeof(cl_int), &mesh_width);

		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFile, "clSetKernelArg() Function Failed For 1st Argument\n");

			uninitialize();
			exit(0);
		}

		// 2nd argument

		oclResult = clSetKernelArg(oclKernel, 2, sizeof(cl_int), &mesh_height);

		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFile, "clSetKernelArg() Function Failed For 2nd Argument\n");

			uninitialize();
			exit(0);
		}

		// 3rd argument

		oclResult = clSetKernelArg(oclKernel, 3, sizeof(cl_float), &animationTime);

		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFile, "clSetKernelArg() Function Failed For 3rd Argument\n");

			uninitialize();
			exit(0);
		}

		// map OpenCL graphics resource for interoperability

		oclResult = clEnqueueAcquireGLObjects(oclCommandQueue, 1, &ocl_graphics_resource, 0, NULL, NULL);

		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFile, "clEnqueueAcquireGLObjects() Function Failed..\n");

			uninitialize();
			exit(0);
		}

		// Launch OpenCL Kernel

		size_t globalWorkSize[2];

		globalWorkSize[0] = mesh_width;
		globalWorkSize[1] = mesh_height;

		oclResult = clEnqueueNDRangeKernel(oclCommandQueue, oclKernel, 2, NULL, globalWorkSize, NULL, 0, NULL, NULL);

		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFile, "clEnqueueNDRangeKernel() Function Failed..\n");

			uninitialize();
			exit(0);
		}

		// Unmap OpenCL Graphics Resource..

		oclResult = clEnqueueReleaseGLObjects(oclCommandQueue, 1, &ocl_graphics_resource, 0, NULL, NULL);


		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFile, "clEnqueueReleaseGLObjects() Function Failed..\n");

			uninitialize();
			exit(0);
		}

		// finish OpenCL command queue
		clFinish(oclCommandQueue);

		// Re-Bind with the OpenGL Buffer Object

		glBindBuffer(GL_ARRAY_BUFFER, vbo_GPU);

	}
	else
	{
		launchCPUKernel(mesh_width, mesh_height, animationTime);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_position);

		glBufferData(GL_ARRAY_BUFFER, MESH_ARRAY_SIZE * sizeof(float), position, GL_DYNAMIC_DRAW);

	}

	// To Tell OpenCL Where We mapped OpenCL Buffer 

	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	glDrawArrays(GL_POINTS, 0, mesh_width* mesh_height);

	// unbind with vbo
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// unbind with vao
	glBindVertexArray(0);

	glUseProgram(0);
	
	SwapBuffers(ghdc);

}


void launchCPUKernel(int width, int height, float time)
{
	// code

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				float u = (float)i / (float)width;
				float v = (float)j / (float)height;

				u = u * 2.0f - 1.0f;

				v = v * 2.0f - 1.0f;

				float frequency = 4.0f;

				float w = sinf(u * frequency + time) * cosf(v * frequency + time) * 0.5f;

				if (k == 0)
				{
					position[i][j][k] = u; // will be mapped x


				}

				if (k == 1)
				{
					position[i][j][k] = w; // will be mapped y
				}

				if (k == 2)
				{
					position[i][j][k] = v; // will be mapped z
				}

				if (k == 3)
				{
					position[i][j][k] = 1.0f; // will be mapped with w
				}

			}
		}
	}

}


void update(void)
{
	//code
	//only at time of change animation here code will come
	animationTime = animationTime + 0.01f;
}


void uninitialize(void)
{
   // Function Declarations
	void ToggleFullScreen(void);

   //code
	if (shaderProgramObject)
	{
		glUseProgram(shaderProgramObject);

		GLint numShaders = 0;

		glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &numShaders);

		if (numShaders > 0)
		{
			GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint));

			if (pShaders != NULL)
			{
				glGetAttachedShaders(shaderProgramObject, numShaders, NULL, pShaders);



				for (GLint i = 0; i < numShaders; i++)
				{
					glDetachShader(shaderProgramObject, pShaders[i]);
					glDeleteShader(pShaders[i]);
					pShaders[i] = 0;
				}

				free(pShaders);
				pShaders = NULL;
			}

			
		}
		
		glUseProgram(0);

		glDeleteProgram(shaderProgramObject);

		shaderProgramObject = 0;

    }

	// Delete VBO of GPU

	if (vbo_GPU)
	{
		if (ocl_graphics_resource)
		{
			clReleaseMemObject(ocl_graphics_resource);
			ocl_graphics_resource = NULL;
		}

		glDeleteBuffers(1, &vbo_GPU);
		vbo_GPU = 0;

	}

	// Release OpenCL Kernel

	if (oclKernel)
	{
		clReleaseKernel(oclKernel);
		oclKernel = NULL;
	}

	// Release OpenCL Program

	if (oclProgram)
	{
		clReleaseProgram(oclProgram);
		oclProgram = NULL;
	}

	// Release OpenCL CommandQueue

	if (oclCommandQueue)
	{
		clReleaseCommandQueue(oclCommandQueue);
		oclCommandQueue = NULL;
	}

	// Release OpenCL Context
	if (oclContext)
	{
		clReleaseContext(oclContext);
		oclContext = NULL;
	}


	// Delete VBO of position
	if (vbo_position)
	{
		glDeleteBuffers(1, &vbo_position);
		vbo_position = 0;
	}

	// Delete VBO of position
	if (vao)
	{
		glDeleteVertexArrays(1, &vao);
		vao = 0;
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









	

