// Windows header files
// Common headers 
#include<windows.h> //For all WIN32 API's
#include<stdio.h>   //for file I/O
#include<stdlib.h>  //for exit()

#include "OGL.h" // swatachi header file
#include "kernel.h"

#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<sstream>


// OpenGL header Files
#include<gl/glew.h>   //this must be before gl/gl.h header file
#include<gl/GL.h> 
#include"vmath.h"

#include<cuda_gl_interop.h>
#include<cuda_runtime.h>

#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
#include"helper_timer.h"

using namespace vmath;
using namespace std;

//#include<GL/glu.h>  //not allowed in PP  (There is no GLU utility in PP)

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Link With OpenGL Library
#pragma comment(lib,"glew32.lib")
#pragma comment(lib, "OpenGL32.lib")

#pragma comment(lib,"cudart.lib")
#pragma comment(lib,"winmm.lib")

// Global Function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//OpenGL Related Global Variables
HDC ghdc = NULL;
HGLRC  ghrc = NULL;      

FILE* gpFile = NULL;

HWND ghwnd = NULL;        // global window handel 
BOOL gbActive = FALSE;    //means by default mazi window active nhi
DWORD dwStyle = 0;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) }; //wpPrev = windowplacementPrevious
BOOL gbFullScreen = FALSE;


GLuint shaderProgramObject = 0;

// shader program for quad texture

GLuint shaderProgramObject_Quad = 0;


GLuint vao_CUDA = 0;
GLuint vbo_Position = 0;
GLuint vbo_Normals = 0;
GLuint vbo_Texcoord = 0;

// quad position for texture 

GLuint vaoQuad = 0;

GLuint vboQuadPosition = 0;

GLuint vboQuadTexCoord = 0;


enum
{
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_NORMAL,
	AMC_ATTRIBUTE_TEXCOORD
};



//GLuint uMVPMatrix;

mat4 perspectiveProjectionMatrix;   //using 4*4 matrix.
//mat4 - represents 4*4 matrix.   
//mat4 is not c++ datattype, it is defined in vmath.h.

// Uniforms

GLuint modelMatrixUniform = 0;
GLuint viewMatrixUniform = 0;
GLuint projectionMatrixUniform = 0;

GLuint lightDiffuseUniform = 0;
GLuint lightAmbientUniform = 0;
GLuint lightSpecularUniform = 0;
GLuint lightPositionUniform = 0;

GLuint materialDiffuseUniform = 0;
GLuint materialAmbientUniform = 0;
GLuint materialSpecularUniform = 0;
GLuint materialShininessUniform = 0;

GLuint blendFactorUniform = 0;
GLuint keyPressUniform = 0;

GLuint textureUniform = 0;
GLuint toggleTextureUniform = 0;

//uniforms for texture quad shader program object

GLuint mvpMatrixUniform_Quad = 0;
GLuint textureSamplerUniform_Quad = 0;

//texture variable
GLuint TextureSmiley = 0;

mat4 perspectiveProjectionMatrix_Quad;


BOOL bLightingEnable = FALSE;

BOOL bAnimationEnable = FALSE;

//texture variable

GLuint texture_Monkey = 0;


// Light Variables

GLfloat lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //white diffuse light
GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightPosition[] = { 3.0f, 3.0f, 3.0f, 1.0f }; //




GLfloat materialAmbient[] = { 0.2f, 0.0113f, 0.01145f, 1.0f };
GLfloat materialDiffuse[] = { 1.0f, 0.85f,  0.7f, 1.0f };
GLfloat materialSpecular[] = { 0.9f, 0.9f, 0.9f, 1.0f };
GLfloat materialShininess = 128.0f;

// vectors to store vertex positions, normals

vector <GLfloat> vertices;
vector <GLfloat> normals;
vector <GLfloat> texCoords;

//vectors to store face indices

vector <GLint> vertexIndices;
vector <GLint> texCoordIndices;
vector <GLint> normalIndices;

//vectors to store position buffer texture buffer and normal buffer

vector <GLfloat> positionBuffer;
vector <GLfloat> normalBuffer;
vector<GLfloat> textureBuffer;

GLfloat alphaValue = 0.0f;

//rotation angle variable

GLfloat rotation = 0.0f;

//wireframe variable

BOOL wireFrame = FALSE;
BOOL texture = FALSE;

BOOL dPress = FALSE;
BOOL aPress = FALSE;

BOOL bResult;

BOOL bRotation = FALSE;

GLfloat zTranslate = -4.5f;

int wheelDelta;

// Set the explosion factor (adjust this value as needed)

//GLfloat explodeFactor[3] = { 0.0012f, 0.0012f, 0.001f };
 
GLfloat explodeFactor[3] = { 0.00052f, 0.00052f, 0.0005f };



// global function declarations

void updatePositionBuffer_CPU(void);

BOOL enableExplosion = FALSE;

//CUDA - OpenGL interop related global variables

GLuint vbo_GPU;

BOOL bOnGPU = FALSE;
BOOL bOnCPU = TRUE;

struct cudaGraphicsResource* cuda_graphics_resource = NULL;

cudaError_t cudaResult;

// pointer for allocating array of floats for explosion factor

float* pExplosionFactor_Device = NULL;

// number of triangles

int numTriangles;

// Texture Switch variables

BOOL CPUTexture = FALSE;
BOOL GPUTexture = FALSE;
BOOL enableTexture = FALSE;

GLuint gpuTexture = 0;

//GLfloat dValue = 1.0f;

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
	TCHAR caption[256];

	// step : centering window sathi
	//int w_height = 600; //window height
	//int w_width = 800;  // window length or width
	int top_left_X = GetSystemMetrics(SM_CXSCREEN);   // (SM_CXSCREEN); <- its a macro... So SM_CX value gives width
	int top_left_Y = GetSystemMetrics(SM_CYSCREEN);   // (SM_CYSCREEN); <- its a macro... So SM_CY value gives height
	int y;
	int x;
	x = (top_left_X / 2) - WIN_WIDTH / 2;
	y = (top_left_Y / 2) - WIN_HEIGHT / 2;


	if (fopen_s(&gpFile, "Log.txt", "w") != 0)   //secuare version of fopen()
	{
		MessageBox(NULL, TEXT("Log File Cannot Be Opened"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	fprintf(gpFile, "Program Started Successfully...\n\n\n");



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
		TEXT("Prathamesh Laxmikant Paropkari :                   Explosion Effect With CUDA   "),
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

	PlaySound(MAKEINTRESOURCE(MySound), NULL, SND_RESOURCE | SND_ASYNC);

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
				float elapsedTime = display();
				sprintf(caption, " Prathamesh Laxmikant Paropkari \t\t\t\t\t\t Model Explosion With CUDA \t\t\t\t\t\t Elapsed Time : %f", elapsedTime);
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
LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam)

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

	case WM_MOUSEWHEEL:

		wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);

		if (wheelDelta > 0)
		{
			if (zTranslate >= -14.0f)
			{
				zTranslate -= 0.3f;
			}
		}
		if (wheelDelta < 0)
		{
			if (zTranslate <= 0.7f)
			{
				zTranslate += 0.3;
			}
		}

		// Force window to redraw
		InvalidateRect(ghwnd, NULL, TRUE);
		break;
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

		case 'W':
		case 'w':

			if (wireFrame == FALSE)
			{
				
				wireFrame = TRUE;
			}
			else
			{
				
				wireFrame = FALSE;
			}
			break;

		case 'T':
		case 't':

			if (texture == FALSE)
			{

				texture = TRUE;
			}
			else
			{

				texture = FALSE;
			}
			break;

		case 'd':
		case 'D':

			if (dPress == FALSE)
			{
				aPress = FALSE;
				dPress = TRUE;
			}
			
			break;

		case 'a':
		case 'A':

			if (aPress == FALSE)
			{
				aPress = TRUE;
				dPress = FALSE;
			}

			break;

		case 'r':
		case 'R':

			if (bRotation == FALSE)
			{
				
				bRotation = TRUE;
			}
			else
			{
				bRotation = FALSE;
			}

			break;
		
		case 'L':
		case 'l':
			if (bLightingEnable == FALSE)
			{

				bLightingEnable = TRUE;
			}
			else
			{

				bLightingEnable = FALSE;
			}
			break;

		case 'E':
		case 'e':
			if (enableExplosion == FALSE)
			{

				enableExplosion = TRUE;
			}
			else
			{

				enableExplosion = FALSE;
			}
			break;

		case 'G':
		case 'g':
			if (bOnGPU == FALSE)
			{

				bOnGPU = TRUE;
				bOnCPU = FALSE;
			}

			GPUTexture = TRUE;
			CPUTexture = FALSE;
			break;

		case 'C':
		case 'c':
			if (bOnCPU == FALSE)
			{

				bOnCPU = TRUE;
				bOnGPU = FALSE;
			}

			GPUTexture = FALSE;
			CPUTexture = TRUE;
			break;

		case 'Q':
		case 'q':

			if (enableTexture == FALSE)
			{
				enableTexture = TRUE;
			}
			else
			{
				enableTexture = FALSE;
			}
		
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
		ShowCursor(TRUE);
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
	// local variables

	int dev_Count;
	


	//function declarations
	void printGLInfo(void);
	void uninitialize(void);
	void resize(int, int);
	void loadOBJFile(void);
	void printVertices(void);
	BOOL LoadGLPNGTexture(GLuint * texture, const char* image);

	//check for CUDA Support If supported select default CUDA device

	cudaResult = cudaGetDeviceCount(&dev_Count);   // check how many number of CUDA GPUs are present in the system

	if (cudaResult != cudaSuccess)
	{
		fprintf(gpFile, "cudaGetDeviceCount() failed \n");
		uninitialize();
		exit(0);
	}

	else if (dev_Count == 0)
	{
		fprintf(gpFile, "No  CUDA supported device \n");
		uninitialize();
		exit(0);
	}
	else
	{

		fprintf(gpFile, "\n\n0th device found\n\n");
		cudaSetDevice(0);  // set 0th device among all the available devices. As I have only one it  will be 0th for me
	}





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

	//initialize GLEW used for PP

	if (glewInit() != GLEW_OK)
	{
		fprintf(gpFile, "glewInit() failed , failed to initialize glew !!\n");
		return(-6);
	}

	//print GL info

	//printGLInfo();

	//load OBJ file

	 loadOBJFile();

	 //printVertices();

	 glEnable(GL_BLEND);
	 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	 

	//Steps to follow in initialize function for PP

	// step 1 : write vertex shader code

	//vertex shader

	 const GLchar* vertexShaderSourceCode =
		 "#version 460 core\n"              // (version of GLSL * 100) core means you are using core profile i.e PP
		 "\n"
		 "in vec4 aPosition;\n"
		 "in vec3 aNormal;\n"
		 "in vec2 aTexCoord;\n"
		 "uniform mat4 uModelMatrix;\n"
		 "uniform mat4 uViewMatrix;\n"
		 "uniform mat4 uProjectionMatrix;\n"
		 "uniform vec3 ulightAmbient;\n"
		 "uniform vec3 ulightDiffuse;\n"
		 "uniform vec3 ulightSpecular;\n"
		 "uniform vec4 ulightPosition;\n"
		 "uniform vec3 uMaterialAmbient;\n"
		 "uniform vec3 uMaterialDiffuse;\n"
		 "uniform vec3 uMaterialSpecular;\n"
		 "uniform float uMaterialShininess;\n"
		 "uniform float ublendFactor;\n"
		 "out vec3 otransformedNormals;\n"
		 "out vec3 olightDirection;\n"
		 "out vec3 oviewerVector;\n"
		 "out float vblendFactor;\n"
		 "out vec2 oTexCoord;"
		 "void main(void)\n"
		 "{\n"
		 "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;\n"  //v
		 "otransformedNormals = (mat3(uViewMatrix * uModelMatrix) * aNormal);\n" // v
		 "olightDirection = (vec3 (ulightPosition - eyeCoordinates));\n" //v
		 "oviewerVector = (- eyeCoordinates.xyz);\n"
		 "gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;\n"
		 "vblendFactor = ublendFactor;\n"
		 "oTexCoord = aTexCoord;\n"
		 "}";

	
	//step 2: Create vertex shader object

	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	//step 3 : Give vertex shader code to OpenGL
	glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);

	//Step 4 : Let OpenGL engine Compile the vertex shader code
	glCompileShader(vertexShaderObject);

	GLint status = 0;
	GLint infoLogLength = 0;
	GLchar* szInfoLog = NULL;

	//step 5 : Check for vertex shader compilation
	//step 5 (a) :  get compile status in status variable
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);

	//step 5 (b) : if error occur i.e status == GL_FALSE,  get length of error message into infoLogLength variable
	if (status == GL_FALSE)
	{

		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			//step 5 (c) : declare a string variable allocate memory equal to above length i.e length of error message
			szInfoLog = (GLchar*)malloc(infoLogLength);


			if (szInfoLog != NULL)
			{
				// step 5 (d) : get the compilation error log info into memory allocated string
				glGetShaderInfoLog(vertexShaderObject, infoLogLength, NULL, szInfoLog);

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

	//follow all steps of vertex shader for fragment shader

	//step 6  : Write the source code for fragment shader   

	//fragment shader

	const GLchar* fragmentShaderSourceCode =
		"#version 460 core\n"
		"\n"
		"in vec3 otransformedNormals;\n"
		"in vec3 olightDirection;\n"
		"in vec3 oviewerVector;\n"
		"in float vblendFactor;\n"
		"in vec2 oTexCoord;\n"
		"uniform vec3 ulightAmbient;\n"
		"uniform vec3 ulightDiffuse;\n"
		"uniform vec3 ulightSpecular;\n"
		"uniform vec3 uMaterialAmbient;\n"
		"uniform vec3 uMaterialDiffuse;\n"
		"uniform vec3 uMaterialSpecular;\n"
		"uniform float uMaterialShininess;\n"
		"uniform sampler2D uTexture;\n"  // Texture sampler
		"uniform bool useTexture\n;"     // Toggle texture usage
		"vec4 FragColor;\n"
		"out vec4 blendedColor;\n"
		"void main(void)\n"
		"{\n"
		"vec3 PhongADSLight;\n" // local variable
		"vec3 normalizedTransformedNormals = normalize(otransformedNormals);\n"
		"vec3 normalizedLightDirection = normalize(olightDirection);\n"
		"vec3 normalizedViwerVector = normalize(oviewerVector);\n"
		"vec3 ambientLight = ulightAmbient * uMaterialAmbient;\n"
		"vec3 diffuseLight = ulightDiffuse * uMaterialDiffuse * max(dot(normalizedLightDirection, normalizedTransformedNormals),0.0);\n"
		"vec3 reflectionVector = reflect(-normalizedLightDirection, normalizedTransformedNormals);\n"
		"vec3 specularLight = ulightSpecular * uMaterialSpecular * pow(max(dot(reflectionVector, normalizedViwerVector), 0.0),uMaterialShininess);\n"
		"PhongADSLight = ambientLight + diffuseLight + specularLight;\n "
		"FragColor =  vec4 (PhongADSLight, 1.0);\n"
		"if(useTexture)"
		"{\n"
		"     FragColor = texture(uTexture,oTexCoord);\n "
		"}\n"
		"blendedColor = FragColor * vblendFactor;\n"
		"}";

	

	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

	glCompileShader(fragmentShaderObject);

	status = 0;
	infoLogLength = 0;
	szInfoLog = NULL;

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
				fprintf(gpFile, "Fragment shader compilation error log : %s\n", szInfoLog);
				free(szInfoLog);
				szInfoLog = NULL;


			}



		}

		uninitialize();
	}

	//linking process for shader program.

	// shader program

	//step 7 : create shader program object

	shaderProgramObject = glCreateProgram();

	//step 8 : Attach both shaders into this program            also know as prelinking binding  should be done before linking and after attachment
	glAttachShader(shaderProgramObject, vertexShaderObject);
	glAttachShader(shaderProgramObject, fragmentShaderObject);

	//step 9: bind attribute location to the shader program object

	glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_POSITION, "aPosition");

	glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_NORMAL, "aNormal");

	glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_TEXCOORD, "aTexCoord");

	



	//step 10: Link the shader program

	glLinkProgram(shaderProgramObject);

	//step 11: do linking error check fpr shader program

	status = 0;
	infoLogLength = 0;
	szInfoLog = NULL;

	glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);

	if (status == GL_FALSE)
	{
		glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar*)malloc(infoLogLength);


			if (szInfoLog != NULL)
			{
				glGetProgramInfoLog(shaderProgramObject, infoLogLength, NULL, szInfoLog);
				fprintf(gpFile, "shader program linking error log : %s\n", szInfoLog);
				free(szInfoLog);
				szInfoLog = NULL;


			}



		}

		uninitialize();
	}


	// Get shader uniform location

	modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "uModelMatrix");



	viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "uViewMatrix");

	projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "uProjectionMatrix");


	lightAmbientUniform = glGetUniformLocation(shaderProgramObject, "ulightAmbient");


	lightDiffuseUniform = glGetUniformLocation(shaderProgramObject, "ulightDiffuse");

	lightSpecularUniform = glGetUniformLocation(shaderProgramObject, "ulightSpecular");


	lightPositionUniform = glGetUniformLocation(shaderProgramObject, "ulightPosition");

	materialAmbientUniform = glGetUniformLocation(shaderProgramObject, "uMaterialAmbient");

	materialDiffuseUniform = glGetUniformLocation(shaderProgramObject, "uMaterialDiffuse");


	materialSpecularUniform = glGetUniformLocation(shaderProgramObject, "uMaterialSpecular");

	materialShininessUniform = glGetUniformLocation(shaderProgramObject, "uMaterialShininess");

	blendFactorUniform = glGetUniformLocation(shaderProgramObject, "ublendFactor");

	textureUniform = glGetUniformLocation(shaderProgramObject, "uTexture");

	toggleTextureUniform = glGetUniformLocation(shaderProgramObject, "useTexture");

	//keyPressUniform = glGetUniformLocation(shaderProgramObject, "uKeyPress");


	////step 13: Create vertex array object

	// vao
	glGenVertexArrays(1, &vao_CUDA);
	glBindVertexArray(vao_CUDA);

	// position vbo for CPU based calculations
	glGenBuffers(1, &vbo_Position);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_Position);

	glBufferData(GL_ARRAY_BUFFER, positionBuffer.size() * sizeof(float), NULL, GL_DYNAMIC_DRAW);

	//glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// normal vbo
	glGenBuffers(1, &vbo_Normals);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Normals);
	glBufferData(GL_ARRAY_BUFFER, normalBuffer.size() * sizeof(float), normalBuffer.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// VBO for Texcoord
	
	glGenBuffers(1, &vbo_Texcoord);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_Texcoord);

	glBufferData(GL_ARRAY_BUFFER, textureBuffer.size()*sizeof(float), textureBuffer.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//step 18 : unbind with VAO
	glBindVertexArray(0);

	

/********************************** Shader Program for Quad Texture ********************************/



	//vertex shader

	const GLchar* vertexShaderSourceCode_Quad =
		"#version 460 core\n" 
		"\n"
		"in vec4 aPosition;\n"
		"in vec2 aTexCoord;\n"
		"out vec2 oTexCoord;\n"
		"uniform mat4 uMVPMatrix;\n"          //u for uniform datatype
		"void main(void)\n"
		"{\n"
		"gl_Position=uMVPMatrix*aPosition;\n"
		"oTexCoord = aTexCoord;\n"
		"}";

	//step 2: Create vertex shader object

	GLuint vertexShaderObject_Quad = glCreateShader(GL_VERTEX_SHADER);

	//step 3 : Give vertex shader code to OpenGL
	glShaderSource(vertexShaderObject_Quad, 1, (const GLchar**)&vertexShaderSourceCode_Quad, NULL);

	//Step 4 : Let OpenGL engine Compile the vertex shader code
	glCompileShader(vertexShaderObject_Quad);

	status = 0;
	infoLogLength = 0;
	szInfoLog = NULL;

	//step 5 : Check for vertex shader compilation
	
	glGetShaderiv(vertexShaderObject_Quad, GL_COMPILE_STATUS, &status);

	//step 5 (b) : if error occur i.e status == GL_FALSE,  get length of error message into infoLogLength variable
	if (status == GL_FALSE)
	{

		glGetShaderiv(vertexShaderObject_Quad, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			//step 5 (c) : declare a string variable allocate memory equal to above length i.e length of error message
			szInfoLog = (GLchar*)malloc(infoLogLength);


			if (szInfoLog != NULL)
			{
				// step 5 (d) : get the compilation error log info into memory allocated string
				glGetShaderInfoLog(vertexShaderObject_Quad, infoLogLength, NULL, szInfoLog);

				//step 5 (e) : print error message into log file
				fprintf(gpFile, "Vertex shader Quad compilation error log : %s\n", szInfoLog);


				//step 5 (f) : free allocated memory
				free(szInfoLog);
				szInfoLog = NULL;


				//step 5 (g) : uninitialize and exit.


			}

		}

		uninitialize();
	}

	//fragment shader

	const GLchar* fragmentShaderSourceCode_Quad =
		"#version 460 core\n"
		"\n"
		"in vec2 oTexCoord;\n"
		"uniform sampler2D uTextureSampler;\n"  
		"out vec4 FragColor;\n"
		"void main(void)\n"
		"{\n"
		"FragColor = texture(uTextureSampler, oTexCoord);\n"
		//"FragColor = vec4(1.0,1.0,1.0,1.0);\n"
		"}";

	GLuint fragmentShaderObject_Quad = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShaderObject_Quad, 1, (const GLchar**)&fragmentShaderSourceCode_Quad, NULL);

	glCompileShader(fragmentShaderObject_Quad);

	status = 0;
	infoLogLength = 0;
	szInfoLog = NULL;

	glGetShaderiv(fragmentShaderObject_Quad, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{

		glGetShaderiv(fragmentShaderObject_Quad, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar*)malloc(infoLogLength);


			if (szInfoLog != NULL)
			{
				glGetShaderInfoLog(fragmentShaderObject_Quad, infoLogLength, NULL, szInfoLog);
				fprintf(gpFile, "Fragment shader quad compilation error log : %s\n", szInfoLog);
				free(szInfoLog);
				szInfoLog = NULL;


			}



		}

		uninitialize();
	}

	//linking process for shader program.

	// shader program

	//step 7 : create shader program object

	shaderProgramObject_Quad = glCreateProgram();

	glAttachShader(shaderProgramObject_Quad, vertexShaderObject_Quad);
	glAttachShader(shaderProgramObject_Quad, fragmentShaderObject_Quad);

	//step 9: bind attribute location to the shader program object

	glBindAttribLocation(shaderProgramObject_Quad, AMC_ATTRIBUTE_POSITION, "aPosition");
	glBindAttribLocation(shaderProgramObject_Quad, AMC_ATTRIBUTE_TEXCOORD, "aTexCoord");



	//step 10: Link the shader program

	glLinkProgram(shaderProgramObject_Quad);

	//step 11: do linking error check fpr shader program

	status = 0;
	infoLogLength = 0;
	szInfoLog = NULL;

	glGetProgramiv(shaderProgramObject_Quad, GL_LINK_STATUS, &status);

	if (status == GL_FALSE)
	{
		glGetProgramiv(shaderProgramObject_Quad, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar*)malloc(infoLogLength);


			if (szInfoLog != NULL)
			{
				glGetProgramInfoLog(shaderProgramObject_Quad, infoLogLength, NULL, szInfoLog);
				fprintf(gpFile, "shader program quad linking error log : %s\n", szInfoLog);
				free(szInfoLog);
				szInfoLog = NULL;


			}



		}

		uninitialize();
	}

	//get Shader uniform Locations

	mvpMatrixUniform_Quad = glGetUniformLocation(shaderProgramObject_Quad, "uMVPMatrix"); 

	textureSamplerUniform_Quad = glGetUniformLocation(shaderProgramObject_Quad, "uTextureSampler");

	if (textureSamplerUniform_Quad == -1)
	{
		fprintf(gpFile, "Loading of quad Texture Uniform variable failed !! Exiting now\n");
		return(-7);
	}

	const GLfloat quadPosition[] =
	{
		1.5f,1.0f,0.0f,   // right top
		-1.0f,1.0f,0.0f,   // left top
		-1.0f,-1.0f,0.0f, // left bottom
		1.5f,-1.0f,0.0f   // right bottom

	};

	const GLfloat quadTexCoord[] =
	{
		1.0f,1.0f,
		 0.0f,1.0f,
		 0.0f,0.0f,
		 1.0f,0.0f

	};



	// VAO for quad 

	glGenVertexArrays(1, &vaoQuad);
	glBindVertexArray(vaoQuad);

	// vbo for quad position

	glGenBuffers(1, &vboQuadPosition);

	glBindBuffer(GL_ARRAY_BUFFER, vboQuadPosition);

	glBufferData(GL_ARRAY_BUFFER, sizeof(quadPosition), quadPosition, GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vbo for quad position

	glGenBuffers(1, &vboQuadTexCoord);

	glBindBuffer(GL_ARRAY_BUFFER, vboQuadTexCoord);

	glBufferData(GL_ARRAY_BUFFER, sizeof(quadTexCoord), quadTexCoord, GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// unbind vao for quad
	glBindVertexArray(vaoQuad);


	//Register OpenGL Buffer to CUDA Graphics Resource for interoperability  

	cudaResult = cudaGraphicsGLRegisterBuffer(&cuda_graphics_resource, vbo_Position, cudaGraphicsRegisterFlagsWriteDiscard);

	if (cudaResult != cudaSuccess)
	{
		fprintf(gpFile, "cudaGraphicsGLRegisterBuffer() failed: %s\n", cudaGetErrorString(cudaResult));
		uninitialize();
		exit(0);
	}

	else
	{
		fprintf(gpFile, "cudaGraphicsGLRegisterBuffer() SUCCESS \n");
	}

	// allocate memory for explosion factor array on GPU

	cudaResult = cudaMalloc((void**)&pExplosionFactor_Device, sizeof(float) * 3);

	if (cudaResult != cudaSuccess)
	{
		fprintf(gpFile, "cudaMalloc() failed \n");
		uninitialize();
		exit(0);
	}

	// copy explotion factor array which is present on host to device memory

	cudaResult = cudaMemcpy(pExplosionFactor_Device, explodeFactor, sizeof(float) * 3, cudaMemcpyHostToDevice);

	if (cudaResult != cudaSuccess)
	{
		fprintf(gpFile, "cudaMemcpy() failed \n");
		uninitialize();
		exit(0);
	}

	// calculate total number of triangles

	numTriangles = positionBuffer.size() / (3 * 3);

	// unbind vao
	glBindVertexArray(0);

	// image koading call

	bResult = LoadGLPNGTexture(&texture_Monkey, "color18.png");


	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of Rabbit Texture failed !! Exiting now\n");
		return(-1);
	}

	bResult = LoadGLPNGTexture(&TextureSmiley, "CPURB.png");


	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of ONCPU texture failed !! Exiting now\n");
		return(-1);
	}

	bResult = LoadGLPNGTexture(&gpuTexture, "GPURB.png");


	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of ONGPU texture failed !! Exiting now\n");
		return(-1);
	}





	// 2nd change for depth or Enabling depth

	glClearDepth(1.0f); // Compulsory
	glEnable(GL_DEPTH_TEST); // 8 test paike ek(depth) test enable kr
	glDepthFunc(GL_LEQUAL);



	// Set The ClearColor Of Window 
	// Clear OpenGl Stars
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	perspectiveProjectionMatrix = vmath::mat4::identity();

	resize(WIN_WIDTH, WIN_HEIGHT);

	int numElements = positionBuffer.size();

	fprintf(gpFile, "\n\nsizeof positionBuffer is %zu\n\n", numElements * sizeof(float));

	// create and start timer
	sdkCreateTimer(&sceneTimer);
	sdkStartTimer(&sceneTimer);

	//code

	return(0);
}

void resize(int width, int height)
{
	// code
	if (height <= 0)
		height = 1;


	//glMatrixMode(GL_PROJECTION); //not allowed in PP 
	//glLoadIdentity();            not allowed in PP 

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	//set Perspective projection matrix

	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	//gluPerspective(45.0f,((GLfloat)width / (GLfloat)height),0.1f,100.0f);  //not allowed in PP 


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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

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


float display(void)
{
	//code
	// 3rd Change GL_DEPTH_BUFFER_BIT
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	elapsedTime = sdkGetTimerValue(&sceneTimer);

	elapsedTime = elapsedTime / 1000;  //convert time in seconds

	//function declarations

	void uninitialize(void);

	//variable declarations

	float* pInteropBuffer= NULL;

	size_t sizeOfBuffer = NULL;


	//PP steps for display function

	//step 1: Use shader program object

	glUseProgram(shaderProgramObject);


	if (wireFrame == TRUE)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	

	//transformations

	mat4 modelViewMatrix = mat4::identity();

	mat4 translationMatrix = mat4::identity();

	mat4 rotationMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, -1.4f, zTranslate);

	//translationMatrix = vmath::translate(0.0f, 10.0f, -10.0f);

	rotationMatrix = vmath::rotate(rotation, 0.0f, 1.0f, 0.0f);

	mat4 modelMatrix = translationMatrix * rotationMatrix;

	mat4 viewMatrix = mat4::identity();

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	glUniform1f(blendFactorUniform, alphaValue);

	
	

	//glUniform1i(keyPressUniform, 1);

	glUniform3fv(lightAmbientUniform, 1, lightAmbient);

	glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

	glUniform3fv(lightSpecularUniform, 1, lightSpecular);

	glUniform4fv(lightPositionUniform, 1, lightPosition);

	glUniform3fv(materialAmbientUniform, 1, materialAmbient);

	glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

	glUniform3fv(materialSpecularUniform, 1, materialSpecular);

	glUniform1f(materialShininessUniform, materialShininess);
	
	/*else
	{
		glUniform1i(keyPressUniform, 0);
	}*/

	

	

	// Set uniform to indicate whether to use texture or material
	if (texture == TRUE) 
	{
		glUniform1i(toggleTextureUniform, GL_TRUE);
	}
	else
	{
		glUniform1i(toggleTextureUniform, GL_FALSE);
	}
	

	if (texture == TRUE)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_Monkey);
		glUniform1i(textureUniform, 0);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glEnable(GL_BLEND);
	// *** bind vao ***
	glBindVertexArray(vao_CUDA);

	if (bOnCPU == TRUE)
	{
		//glBindBuffer(GL_ARRAY_BUFFER, vbo_Position);

		if (enableExplosion == TRUE)
		{
			updatePositionBuffer_CPU();
		}

		glBindBuffer(GL_ARRAY_BUFFER, vbo_Position);
		glBufferData(GL_ARRAY_BUFFER, positionBuffer.size() * sizeof(float), positionBuffer.data(), GL_DYNAMIC_DRAW);

		
		glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);


		

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}
	if (bOnGPU == TRUE)
	{

		//glBindBuffer(GL_ARRAY_BUFFER, 0);
	

		//map cuda graphics resourse for interoperability
		cudaResult = cudaGraphicsMapResources(1, &cuda_graphics_resource, 0);

		if (cudaResult != cudaSuccess)
		{
			fprintf(gpFile, "cudaGraphicsMapResources() failed \n");
			uninitialize();
			exit(0);
		}
		else
		{
			fprintf(gpFile, "cudaGraphicsMapResources() SUCCESS \n");
		}

		// get mapped resource pointer useable in host code

		cudaResult = cudaGraphicsResourceGetMappedPointer((void**)&pInteropBuffer, &sizeOfBuffer, cuda_graphics_resource);

		if (cudaResult != cudaSuccess)
		{
			fprintf(gpFile, "cudaGraphicsResourceGetMappedPointer() failed: %s\n", cudaGetErrorString(cudaResult));
			uninitialize();
			exit(0);
		}
		else
		{
			fprintf(gpFile, "cudaGraphicsResourceGetMappedPointer() SUCCESS\n");
			fprintf(gpFile,"Sizeof mapped buffer is %zu bytes\n", sizeOfBuffer);
		}

		// call the CUDA Kernel and update the same buffer vbo_Position by GPU ( CUDA )

		if (enableExplosion == TRUE)
		{
			launchCUDAKernel(pInteropBuffer, pExplosionFactor_Device, numTriangles);
		}

		//unmap the host usable pointer to read on the host

		cudaResult = cudaGraphicsUnmapResources(1, &cuda_graphics_resource, 0);

		if (cudaResult != cudaSuccess)
		{
			fprintf(gpFile, "cudaGraphicsUnmapResources() failed: %s\n", cudaGetErrorString(cudaResult));
			uninitialize();
			exit(0);
		}

		// Synchronize GPU buffer to CPU buffer (important step)

		glBindBuffer(GL_ARRAY_BUFFER, vbo_Position);
		glGetBufferSubData(GL_ARRAY_BUFFER, 0, positionBuffer.size() * sizeof(float), positionBuffer.data());  // Copy data back to CPU
		glBindBuffer(GL_ARRAY_BUFFER, 0);



		glBindBuffer(GL_ARRAY_BUFFER, vbo_Position);

		
		glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

		

	}


	
	

	
	GLint numVertices = positionBuffer.size() / 3;
	
	glDrawArrays(GL_TRIANGLES, 0, numVertices);

	// *** unbind vao ***
	glBindVertexArray(0);

	//step 5: Unuse shader program object
	glUseProgram(0);


	/***************************************** quad drawing starts *****************************************/
	 
	

	//step 1: Use shader program object

	glUseProgram(shaderProgramObject_Quad);



	//transformations

	mat4 scaleMatrix = mat4::identity();

	scaleMatrix = vmath::scale(0.4f, 0.4f, 0.4f);

	 modelViewMatrix = vmath::translate(-2.1f, 1.0f, -3.5f);

	 modelViewMatrix = modelViewMatrix * scaleMatrix;

	mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;  

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(mvpMatrixUniform_Quad, 1, GL_FALSE, modelViewProjectionMatrix);

	//for texture

	if (enableTexture == TRUE)
	{
		if (CPUTexture == TRUE || GPUTexture == FALSE)
		{
			glActiveTexture(GL_TEXTURE1);

			glBindTexture(GL_TEXTURE_2D, TextureSmiley);

			glUniform1i(textureSamplerUniform_Quad, 1);
		}

		if (GPUTexture == TRUE)
		{
			glActiveTexture(GL_TEXTURE1);

			glBindTexture(GL_TEXTURE_2D, gpuTexture);

			glUniform1i(textureSamplerUniform_Quad, 1);
		}
		
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glBindVertexArray(vaoQuad);

	//step 3: Draw the geometry

	glDrawArrays(GL_QUADS, 0, 4);


	//step 4: unBind with (VAO)

	glBindVertexArray(0);

	//step 5: Unuse shader program object
	glUseProgram(0);


	SwapBuffers(ghdc);

	return(elapsedTime);

}

void update(void)
{

	if (dPress == FALSE && aPress == FALSE)
	{
		if (alphaValue < 1.0f)
		{
			alphaValue += 0.004f;
		}
		
	}

	if (dPress == TRUE)
	{
		if (alphaValue > 0.0f)
		{
			alphaValue -= 0.005;
		}
		
	}

	if (aPress == TRUE)
	{
		if (alphaValue < 1.0f)
		{
			alphaValue += 0.01;
		}
	}

	
	if (bRotation == TRUE)
	{
		if (rotation < 360.0f)
		{
			rotation += 0.25f;
		}
		else
		{
			rotation = -360.0f;
		}
	}

	


	

	

	


}


void uninitialize(void)
{
	// Function Declarations
	void ToggleFullScreen(void);

	//code

	 //steps for uninitialize function in PP

	if (shaderProgramObject)
	{
		//step 1 : use shader program object

		glUseProgram(shaderProgramObject);

		GLint numShaders = 0;

		//step 2 : get number of attached shaders
		glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &numShaders);



		if (numShaders > 0)
		{
			//step 3 : Create and allocate memory of shader objects according to number of attached shaders
			GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint));

			if (pShaders != NULL)
			{
				//step 4 : Get shader object into the allocated array.

				glGetAttachedShaders(shaderProgramObject, numShaders, NULL, pShaders);

				//step 5 : Detach and delete shader objects iteratively

				for (GLint i = 0; i < numShaders; i++)
				{
					glDetachShader(shaderProgramObject, pShaders[i]);
					glDeleteShader(pShaders[i]);
					pShaders[i] = 0;
				}
				//step 6 : Free the memory allocated for array of shader objects

				free(pShaders);
				pShaders = NULL;
			}
		}
		//step 7 : Unuse shader program objects

		glUseProgram(0);

		//step 8 : Delete shader program objects

		glDeleteProgram(shaderProgramObject);
		shaderProgramObject = 0;


	}


	if (shaderProgramObject_Quad)
	{
		//step 1 : use shader program object

		glUseProgram(shaderProgramObject_Quad);

		GLint numShaders = 0;

		//step 2 : get number of attached shaders
		glGetProgramiv(shaderProgramObject_Quad, GL_ATTACHED_SHADERS, &numShaders);



		if (numShaders > 0)
		{
			//step 3 : Create and allocate memory of shader objects according to number of attached shaders
			GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint));

			if (pShaders != NULL)
			{
				//step 4 : Get shader object into the allocated array.

				glGetAttachedShaders(shaderProgramObject_Quad, numShaders, NULL, pShaders);

				//step 5 : Detach and delete shader objects iteratively

				for (GLint i = 0; i < numShaders; i++)
				{
					glDetachShader(shaderProgramObject_Quad, pShaders[i]);
					glDeleteShader(pShaders[i]);
					pShaders[i] = 0;
				}
				//step 6 : Free the memory allocated for array of shader objects

				free(pShaders);
				pShaders = NULL;
			}
		}
		//step 7 : Unuse shader program objects

		glUseProgram(0);

		//step 8 : Delete shader program objects

		glDeleteProgram(shaderProgramObject_Quad);
		shaderProgramObject_Quad = 0;


	}


	//step 9 : delete vbo of color

	//sphere

	// delete vbo for element
	if (vbo_Normals)
	{
		glDeleteBuffers(1, &vbo_Normals);
		vbo_Normals = 0;
	}

	// delete vbo for normal
	if (vbo_Position)
	{
		glDeleteBuffers(1, &vbo_Position);
		vbo_Position = 0;
	}

	// delete vbo for normal
	if (vbo_Texcoord)
	{
		glDeleteBuffers(1, &vbo_Texcoord);
		vbo_Texcoord = 0;
	}


	//step 11 : delete VAO 

	if (vao_CUDA)
	{
		glDeleteVertexArrays(1, &vao_CUDA);
		vao_CUDA = 0;
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

	// relese allocated memory by cudaMalloc()

	if (pExplosionFactor_Device)
	{
		cudaFree(pExplosionFactor_Device);
		pExplosionFactor_Device = NULL;
	}




	//Close The Log File
	if (gpFile)
	{
		fprintf(gpFile, "Program Ended Sucessfully...\n");
		fclose(gpFile);
		gpFile = NULL;
	}

	// DistroyWindow
	if (ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd = NULL;
	}

}

void printGLInfo(void)
{
	//variable declarations

	GLint numExtensions;
	GLint i;

	//code

	fprintf(gpFile, "OpenGL Vendor   : %s\n\n", glGetString(GL_VENDOR));
	fprintf(gpFile, "OpenGL Renderer : %s\n\n", glGetString(GL_RENDERER));
	fprintf(gpFile, "OpenGL Version  : %s\n\n", glGetString(GL_VERSION));
	fprintf(gpFile, "GLSL (Graphic library shading language) Version : %s\n\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	//listing of supported extensions

	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

	for (i = 0; i < numExtensions; i++)
	{
		fprintf(gpFile, "%s\n", glGetStringi(GL_EXTENSIONS, i));
	}



}

void loadOBJFile(void)
{
	ifstream dataIn;

	dataIn.open("bunny2.obj");

	if (!dataIn.is_open())
	{
		fprintf(gpFile, "Failed to load the OBJ file !!!\n\n");
		exit(-56);
	}

	string line;

	while (getline(dataIn, line))
	{
		if (line[0] == 'v' && line[1] == ' ')
		{
			istringstream sso1(line);
			sso1.ignore(2);

			GLfloat vertexX, vertexY, vertexZ;

			if (sso1 >> vertexX >> vertexY >> vertexZ)
			{
				vertices.push_back(vertexX);
				vertices.push_back(vertexY);
				vertices.push_back(vertexZ);
			}
			else
			{
				fprintf(gpFile, "\n\nError extracting vertex coordinates\n\n");
			}
		}
		else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ')
		{
			istringstream sso2(line);
			sso2.ignore(3);

			GLfloat normalX, normalY, normalZ;

			if (sso2 >> normalX >> normalY >> normalZ)
			{
				normals.push_back(normalX);
				normals.push_back(normalY);
				normals.push_back(normalZ);
			}
			else
			{
				fprintf(gpFile, "\n\nError extracting Normals\n\n");
			}
		}
		else if (line[0] == 'v' && line[1] == 't' && line[2] == ' ')
		{
			istringstream sso3(line);
			sso3.ignore(3);

			GLfloat texCoordX, texCoordY;

			if (sso3 >> texCoordX >> texCoordY)
			{
				texCoords.push_back(texCoordX);
				texCoords.push_back(texCoordY);
			}
			else
			{
				fprintf(gpFile, "\n\nError extracting Texture Coordinates\n\n");
			}
		}
		else if (line[0] == 'f' && line[1] == ' ')
		{
			istringstream sso4(line);
			sso4.ignore(2);

			GLint vertexIndex, textureIndex, normalIndex;
			GLchar separator;

			while (sso4 >> vertexIndex >> separator >> textureIndex >> separator >> normalIndex)
			{
				vertexIndices.push_back(vertexIndex);
				texCoordIndices.push_back(textureIndex);
				normalIndices.push_back(normalIndex);

				// Handle multiple faces per line if needed
				if (sso4.peek() == ' ')
					sso4.ignore();
			}
		}
	}

	// Fill position buffer
	int num1 = 0;
	while (num1 < vertexIndices.size())
	{
		int vertexIndex = vertexIndices[num1] - 1;
		int startIndex = vertexIndex * 3;

		GLfloat vertexX = vertices[startIndex];
		GLfloat vertexY = vertices[startIndex + 1];
		GLfloat vertexZ = vertices[startIndex + 2];

		positionBuffer.push_back(vertexX);
		positionBuffer.push_back(vertexY);
		positionBuffer.push_back(vertexZ);

		num1++;
	}

	// Fill normal buffer
	int num2 = 0;
	while (num2 < normalIndices.size())
	{
		int normalIndex = normalIndices[num2] - 1;
		int startIndex = normalIndex * 3;

		GLfloat normalX = normals[startIndex];
		GLfloat normalY = normals[startIndex + 1];
		GLfloat normalZ = normals[startIndex + 2];

		normalBuffer.push_back(normalX);
		normalBuffer.push_back(normalY);
		normalBuffer.push_back(normalZ);

		num2++;
	}

	// Fill texture buffer
	int num3 = 0;
	while (num3 < texCoordIndices.size())
	{
		int texCoordIndex = texCoordIndices[num3] - 1;
		int startIndex = (texCoordIndex * 2);//

		GLfloat texCoordX = texCoords[startIndex];
		GLfloat texCoordY = texCoords[startIndex + 1];

		textureBuffer.push_back(texCoordX);
		textureBuffer.push_back(texCoordY);

		num3++;
	}

	dataIn.close();
}


void printVertices(void)
{
	GLint i = 0;
	GLint j = 0;
	GLint k = 0;
	GLint l = 0;
	GLint m = 0;
	GLint n = 0;

	while (i < vertices.size())
	{
		fprintf(gpFile, "X = %f\n", vertices[i]);

		i++;

		fprintf(gpFile, "Y = %f\n", vertices[i]);

		i++;

		fprintf(gpFile, "Z = %f\n\n\n", vertices[i]);



		i++;

	}

	/*fprintf(gpFile, "\n\n**************************** Printing Normals*****************************************\n\n");

	while (j < normals.size())
	{
		fprintf(gpFile, "X = %f\n", normals[j]);

		j++;

		fprintf(gpFile, "Y = %f\n", normals[j]);

		j++;

		fprintf(gpFile, "Z = %f\n\n\n", normals[j]);



		j++;
	}*/

	fprintf(gpFile, "\n\n**************************** Printing Texture Coordinates*****************************************\n\n");

	while (k < texCoords.size())
	{
		fprintf(gpFile, "X = %f\n", texCoords[k]);

		k++;

		fprintf(gpFile, "Y = %f\n\n", texCoords[k]);

		k++;

	}

	/*fprintf(gpFile, "\n\n**************************** Printing Vertex Indices indices*****************************************\n\n");

	while (l < vertexIndices.size())
	{
		fprintf(gpFile, "V = %d\n", vertexIndices[l]);

		l++;
	}*/


	fprintf(gpFile, "\n\n**************************** Printing texture Indices indices*****************************************\n\n");

	while (m < texCoordIndices.size())
	{
		fprintf(gpFile, "T = %d\n", texCoordIndices[m]);

		m++;
	}

	/*fprintf(gpFile, "\n\n**************************** Printing Normal Indices indices*****************************************\n\n");

	while (n < normalIndices.size())
	{
		fprintf(gpFile, "N = %d\n", normalIndices[n]);

		n++;
	}*/

	/*fprintf(gpFile, "\n\n**************************** Printing Position buffer *****************************************\n\n");

	int num = 0;
	while (num < positionBuffer.size())
	{
		fprintf(gpFile, "vertex %d :  X = %f  y = %f  z = %f\n", num, positionBuffer[num], positionBuffer[num + 1], positionBuffer[num + 2]);

		num++;
	}*/

	/*fprintf(gpFile, "\n\n**************************** Printing normal buffer *****************************************\n\n");

	num = 0;
	while (num < normalBuffer.size())
	{
		fprintf(gpFile, "normal %d :  X = %f  y = %f  z = %f\n", num, normalBuffer[num], normalBuffer[num + 1], normalBuffer[num + 2]);

		num++;
	}*/

	fprintf(gpFile, "\n\n**************************** Printing texture buffer *****************************************\n\n");

	int num = 0;
	while (num < textureBuffer.size())
	{
		fprintf(gpFile, "Tex Coord %d :  X = %f  y = %f\n", num/2, textureBuffer[num], textureBuffer[num + 1]);

		num += 2;
	}

}

void updatePositionBuffer_CPU(void)
{
	for (size_t i = 0; i < positionBuffer.size(); i += 9)
	{
		//fetch vertices of a triangle

		vector<GLfloat> v1 = { positionBuffer[i],positionBuffer[i + 1],positionBuffer[i + 2] };

		vector<GLfloat> v2 = { positionBuffer[i + 3],positionBuffer[i + 4],positionBuffer[i + 5] };

		vector<GLfloat> v3 = { positionBuffer[i + 6],positionBuffer[i + 7],positionBuffer[i + 8] };

		// calculate normal for the above triangle

		vector <GLfloat> ab = { v2[0] - v1[0],v2[1] - v1[1],v2[2] - v1[2] };

		vector <GLfloat> ac = { v3[0] - v1[0],v3[1] - v1[1],v3[2] - v1[2] };

		// calculate normal of the triangle

		vector<GLfloat> normal = 
		{
			ab[1] * ac[2] - ab[2] * ac[1], // X component
			ab[2] * ac[0] - ab[0] * ac[2], // Y component
			ab[0] * ac[1] - ab[1] * ac[0]  // Z component
		};

		// Normalize the normal vector


		GLfloat magnitude = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);

		if (magnitude > 1.0E-6) 
		{
			normal[0] = normal[0]/magnitude;
			normal[1] = normal[1]/magnitude;
			normal[2] = normal[2]/magnitude;
		}

		// set positions

		// vertex one

		positionBuffer[i] = positionBuffer[i] + explodeFactor[0] * normal[0];
		positionBuffer[i +1] = positionBuffer[i + 1] + explodeFactor[1] * normal[1];
		positionBuffer[i + 2] = positionBuffer[i + 2] + explodeFactor[2] * normal[2];

		// vertex two

		positionBuffer[i + 3] = positionBuffer[i + 3] + explodeFactor[0] * normal[0];
		positionBuffer[i + 4] = positionBuffer[i + 4] + explodeFactor[1] * normal[1];
		positionBuffer[i + 5] = positionBuffer[i + 5] + explodeFactor[2] * normal[2];

		// vertex three

		positionBuffer[i + 6] = positionBuffer[i + 6] + explodeFactor[0] * normal[0];
		positionBuffer[i + 7] = positionBuffer[i + 7] + explodeFactor[1] * normal[1];
		positionBuffer[i + 8] = positionBuffer[i + 8] + explodeFactor[2] * normal[2];



		

	}
}




