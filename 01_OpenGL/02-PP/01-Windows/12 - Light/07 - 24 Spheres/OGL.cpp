// Windows header files
// Common headers 
#include<windows.h> //For all WIN32 API's
#include<stdio.h>   //for file I/O
#include<stdlib.h>  //for exit()

#include "OGL.h" // swatachi header file
#include "Sphere.h" // header for sphere


// OpenGL header Files
#include<gl/glew.h>   //this must be before gl/gl.h header file
#include<gl/GL.h> 
#include"vmath.h"



using namespace vmath;

//#include<GL/glu.h>  //not allowed in PP  (There is no GLU utility in PP)

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Link With OpenGL Library
#pragma comment(lib,"glew32.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib,"Sphere.lib")  // for sphere


//#pragma comment(lib, "glu32.lib") //file me OpenGL    //dont need in PP (There is no GLU utility in PP)

// Global Function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global Variable Declarations

//OpenGL Related Global Variables
HDC ghdc = NULL;
HGLRC  ghrc = NULL;       // handle to graphic rendering contex

FILE* gpFile = NULL;

HWND ghwnd = NULL;        // global window handel 
BOOL gbActive = FALSE;    //means by default mazi window active nhi
DWORD dwStyle = 0;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) }; //wpPrev = windowplacementPrevious
BOOL gbFullScreen = FALSE;


GLuint shaderProgramObject = 0;


GLuint gVao = 0;
GLuint gVbo_sphere_position = 0;
GLuint gVbo_sphere_normal = 0;
GLuint gVbo_sphere_element = 0;
GLuint vbo_Texcoord = 0;


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

GLuint mvpMatrixUniform = 0;




//unsigned int  gNumVertices;
unsigned int  gNumSphereElements;


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


GLuint keyPressUniform = 0;

BOOL bLightingEnable = FALSE;

BOOL bAnimationEnable = FALSE;


//GLfloat lightAmbient[] = { 0.1f,0.1f,0.1f,1.0f };
//GLfloat lightDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
//GLfloat lightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
//GLfloat lightPosition[] = { 100.0f,100.0f,100.0f,1.0f };

GLfloat lightAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightPosition[] = { 0.0f,0.0f,0.0f,1.0f };

//GLfloat materialAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
//GLfloat materialDiffuse[] = { 0.5f,0.2f,0.7f,1.0f };
//GLfloat materialSpecular[] = { 0.7f, 0.7f, 0.7f, 1.0f };
//GLfloat materialShininess = 128.0f;


//light rotation variables

GLfloat angleForXRotation = 0.0f;
GLfloat angleForYRotation = 0.0f;
GLfloat angleForZRotation = 0.0f;

GLfloat WindowWidth;
GLfloat WindowHeight;

int keyPressed = 0;

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
		TEXT("Prathamesh Laxmikant Paropkari :   24 Spheres "),
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

				if (bLightingEnable == TRUE)
				{
					// Update
					update();
				}


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
			if (bLightingEnable == FALSE)
			{
				bLightingEnable = TRUE;
			}
			else
			{
				bLightingEnable = FALSE;
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
	//Local variable declaration for sphere
	float sphere_position[1146];
	float sphere_normals[1146];
	float sphere_texcoord[764];
	unsigned short sphere_elements[2280];


	//function declarations
	void printGLInfo(void);
	void uninitialize(void);
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

	//initialize GLEW used for PP

	if (glewInit() != GLEW_OK)
	{
		fprintf(gpFile, "glewInit() failed , failed to initialize glew !!\n");
		return(-6);
	}

	//print GL info

	printGLInfo();

	//Steps to follow in initialize function for PP

	// step 1 : write vertex shader code

	//vertex shader

	const GLchar* vertexShaderSourceCode =
		"#version 460 core\n"              // (version of GLSL * 100) core means you are using core profile i.e PP
		"\n"
		"in vec4 aPosition;\n"
		"in vec3 aNormal;\n"
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
		"uniform int uKeyPress;\n"
		"out vec3 otransformedNormals;\n"
		"out vec3 olightDirection;\n"
		"out vec3 oviewerVector;\n"
		"void main(void)\n"
		"{\n"
		"if(uKeyPress == 1)\n"
		"{\n"
		"vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;\n"
		"otransformedNormals = (mat3(uViewMatrix * uModelMatrix) * aNormal);\n"
		"olightDirection = (vec3 (ulightPosition - eyeCoordinates));\n"
		"oviewerVector = (- eyeCoordinates.xyz);\n"
		"}\n"
		"else\n"
		"{\n"
		"otransformedNormals = vec3(0.0, 0.0, 0.0);\n"
		"olightDirection = vec3(0.0, 0.0, 0.0);\n"
		"oviewerVector = vec3(0.0, 0.0, 0.0);\n"
		"}\n"
		"gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;\n"
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
		"uniform vec3 ulightAmbient;\n"
		"uniform vec3 ulightDiffuse;\n"
		"uniform vec3 ulightSpecular;\n"
		"uniform vec3 uMaterialAmbient;\n"
		"uniform vec3 uMaterialDiffuse;\n"
		"uniform vec3 uMaterialSpecular;\n"
		"uniform float uMaterialShininess;\n"
		"uniform int uKeyPress;\n"
		"out vec4 FragColor;\n"
		"void main(void)\n"
		"{\n"
		"vec3 PhongADSLight;\n" // local variable
		"if(uKeyPress == 1)\n"
		"{\n"
		"vec3 normalizedTransformedNormals = normalize(otransformedNormals);\n"
		"vec3 normalizedLightDirection = normalize(olightDirection);\n"
		"vec3 normalizedViwerVector = normalize(oviewerVector);\n"
		"vec3 ambientLight = ulightAmbient * uMaterialAmbient;\n"
		"vec3 diffuseLight = ulightDiffuse * uMaterialDiffuse * max(dot(normalizedLightDirection, normalizedTransformedNormals),0.0);\n"
		"vec3 reflectionVector = reflect(-normalizedLightDirection, normalizedTransformedNormals);\n"
		"vec3 specularLight = ulightSpecular * uMaterialSpecular * pow(max(dot(reflectionVector, normalizedViwerVector), 0.0),uMaterialShininess);\n"
		"PhongADSLight = ambientLight + diffuseLight + specularLight;\n "
		"}\n"
		"else\n"
		"{\n"
	    "PhongADSLight = vec3(1.0, 1.0, 1.0);\n"
		"}\n"
		"FragColor =  vec4 (PhongADSLight, 1.0);\n"
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

	keyPressUniform = glGetUniformLocation(shaderProgramObject, "uKeyPress");




	getSphereVertexData(sphere_position, sphere_normals, sphere_texcoord, sphere_elements);

	// gNumVertices = getNumberOfSphereVertices();

	gNumSphereElements = getNumberOfSphereElements();




	////step 13: Create vertex array object

	// vao
	glGenVertexArrays(1, &gVao);
	glBindVertexArray(gVao);

	// position vbo
	glGenBuffers(1, &gVbo_sphere_position);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_sphere_position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_position), sphere_position, GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// normal vbo
	glGenBuffers(1, &gVbo_sphere_normal);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_sphere_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// VBO for Texcoord
	glGenBuffers(1, &vbo_Texcoord);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_Texcoord);

	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_texcoord), sphere_texcoord, GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//step 18 : unbind with VAO
	glBindVertexArray(0);



	// element vbo
	glGenBuffers(1, &gVbo_sphere_element);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// unbind vao
	glBindVertexArray(0);



	// 2nd change for depth or Enabling depth

	glClearDepth(1.0f); // Compulsory
	glEnable(GL_DEPTH_TEST); // 8 test paike ek(depth) test enable kr
	glDepthFunc(GL_LEQUAL);



	// Set The ClearColor Of Window 
	// Clear OpenGl Stars
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	perspectiveProjectionMatrix = vmath::mat4::identity();

	resize(WIN_WIDTH, WIN_HEIGHT);



	//code

	return(0);
}

void resize(int width, int height)
{
	// code
	if (height <= 0)
		height = 1;


	WindowHeight = height;
	WindowWidth = width;

	//glMatrixMode(GL_PROJECTION); //not allowed in PP 
	//glLoadIdentity();            not allowed in PP 

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	//set Perspective projection matrix

	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	//gluPerspective(45.0f,((GLfloat)width / (GLfloat)height),0.1f,100.0f);  //not allowed in PP 


}

void display(void)
{

	//variable declaration
	
	GLfloat horizontalDistance = WindowWidth / 6;
	GLfloat verticalDistance = WindowHeight / 7;

	GLfloat materialAmbient[4];
	GLfloat materialDiffuse[4];
	GLfloat materialSpecular[4];

	GLfloat Shininess;


	
	//code
	// 3rd Change GL_DEPTH_BUFFER_BIT
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	


	//PP steps for display function

	//step 1: Use shader program object

	glUseProgram(shaderProgramObject);




	//Sphere

	//set the view Port

	

	//First Column precious stones

	//(emerald)

	materialAmbient[0] = 0.0215;
	materialAmbient[1] = 0.1745;
	materialAmbient[2] = 0.0215;
	materialAmbient[3] = 1.0f;

	

	materialDiffuse[0] = 0.07568;
	materialDiffuse[1] = 0.61424;
	materialDiffuse[2] = 0.07568;
	materialDiffuse[3] = 1.0f;

	

	materialSpecular[0] = 0.633;
	materialSpecular[1] = 0.727811;
	materialSpecular[2] = 0.633;
	materialSpecular[3] = 1.0f;

	

	GLfloat materialShininess = 0.6f * 128.0f;



	//transformations

	mat4 modelViewMatrix = mat4::identity();

	mat4 translationMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

	mat4 modelMatrix = translationMatrix;

	mat4 viewMatrix = mat4::identity();

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLightingEnable == TRUE)
	{
		glUniform1i(keyPressUniform, 1);

		glUniform3fv(lightAmbientUniform, 1, lightAmbient);

		glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

		glUniform3fv(lightSpecularUniform, 1, lightSpecular);

		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);

		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

		glUniform3fv(materialSpecularUniform, 1, materialSpecular);

		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}



	// *** bind vao ***
	glBindVertexArray(gVao);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);



	//first column first sphere

	glViewport(0, (verticalDistance * 5), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glDrawElements(GL_TRIANGLES, gNumSphereElements, GL_UNSIGNED_SHORT, 0); // 3rd parameter should be that


	//first column second sphere

	glViewport(0, (verticalDistance * 4), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));


	materialAmbient[0] = 0.135;
	materialAmbient[1] = 0.2225;
	materialAmbient[2] = 0.1575;
	materialAmbient[3] = 1.0f;

	

	materialDiffuse[0] = 0.54;
	materialDiffuse[1] = 0.89;
	materialDiffuse[2] = 0.63;
	materialDiffuse[3] = 1.0f;



	materialSpecular[0] = 0.316228;
	materialSpecular[1] = 0.316228;
	materialSpecular[2] = 0.316228;
	materialSpecular[3] = 1.0f;

	

	materialShininess = 0.1 * 128;

	//transformations

	 modelViewMatrix = mat4::identity();

	 translationMatrix = mat4::identity();

	 translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

	 modelMatrix = translationMatrix;

	 viewMatrix = mat4::identity();

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLightingEnable == TRUE)
	{
		glUniform1i(keyPressUniform, 1);

		glUniform3fv(lightAmbientUniform, 1, lightAmbient);

		glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

		glUniform3fv(lightSpecularUniform, 1, lightSpecular);

		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);

		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

		glUniform3fv(materialSpecularUniform, 1, materialSpecular);

		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}


	glDrawElements(GL_TRIANGLES, gNumSphereElements, GL_UNSIGNED_SHORT, 0); // 3rd parameter should be that

	//first column Third sphere

	glViewport(0, (verticalDistance * 3), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));


	materialAmbient[0] = 0.05375;
	materialAmbient[1] = 0.05;
	materialAmbient[2] = 0.06625;
	materialAmbient[3] = 1.0f;

	

	materialDiffuse[0] = 0.18275;
	materialDiffuse[1] = 0.17;
	materialDiffuse[2] = 0.22525;
	materialDiffuse[3] = 1.0f;

	

	materialSpecular[0] = 0.332741;
	materialSpecular[1] = 0.328634;
	materialSpecular[2] = 0.346435;
	materialSpecular[3] = 1.0f;

	

	materialShininess = 0.3 * 128;
	//transformations

	modelViewMatrix = mat4::identity();

	translationMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

	modelMatrix = translationMatrix;

	viewMatrix = mat4::identity();

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLightingEnable == TRUE)
	{
		glUniform1i(keyPressUniform, 1);

		glUniform3fv(lightAmbientUniform, 1, lightAmbient);

		glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

		glUniform3fv(lightSpecularUniform, 1, lightSpecular);

		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);

		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

		glUniform3fv(materialSpecularUniform, 1, materialSpecular);

		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}


	glDrawElements(GL_TRIANGLES, gNumSphereElements, GL_UNSIGNED_SHORT, 0); // 3rd parameter should be that

	//first column Fourth sphere

	glViewport(0, (verticalDistance * 2), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));


	materialAmbient[0] = 0.25;
	materialAmbient[1] = 0.20725;
	materialAmbient[2] = 0.20725;
	materialAmbient[3] = 1.0f;

	

	materialDiffuse[0] = 1.0;
	materialDiffuse[1] = 0.829;
	materialDiffuse[2] = 0.829;
	materialDiffuse[3] = 1.0f;

	

	materialSpecular[0] = 0.296648;
	materialSpecular[1] = 0.296648;
	materialSpecular[2] = 0.296648;
	materialSpecular[3] = 1.0f;



	materialShininess = 0.088 * 128;

	//transformations

	modelViewMatrix = mat4::identity();

	translationMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

	modelMatrix = translationMatrix;

	viewMatrix = mat4::identity();

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLightingEnable == TRUE)
	{
		glUniform1i(keyPressUniform, 1);

		glUniform3fv(lightAmbientUniform, 1, lightAmbient);

		glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

		glUniform3fv(lightSpecularUniform, 1, lightSpecular);

		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);

		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

		glUniform3fv(materialSpecularUniform, 1, materialSpecular);

		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}


	glDrawElements(GL_TRIANGLES, gNumSphereElements, GL_UNSIGNED_SHORT, 0); // 3rd parameter should be that

	//first column Fifth sphere

	glViewport(0, (verticalDistance * 1), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));


	materialAmbient[0] = 0.1745;
	materialAmbient[1] = 0.01175;
	materialAmbient[2] = 0.01175;
	materialAmbient[3] = 1.0f;


	materialDiffuse[0] = 0.61424;
	materialDiffuse[1] = 0.04136;
	materialDiffuse[2] = 0.04136;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.727811;
	materialSpecular[1] = 0.626959;
	materialSpecular[2] = 0.626959;
	materialSpecular[3] = 1.0f;


	materialShininess = 0.6 * 128;

	//transformations

	modelViewMatrix = mat4::identity();

	translationMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

	modelMatrix = translationMatrix;

	viewMatrix = mat4::identity();

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLightingEnable == TRUE)
	{
		glUniform1i(keyPressUniform, 1);

		glUniform3fv(lightAmbientUniform, 1, lightAmbient);

		glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

		glUniform3fv(lightSpecularUniform, 1, lightSpecular);

		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);

		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

		glUniform3fv(materialSpecularUniform, 1, materialSpecular);

		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}


	glDrawElements(GL_TRIANGLES, gNumSphereElements, GL_UNSIGNED_SHORT, 0); // 3rd parameter should be that

	//first column Sixth sphere

	glViewport(0, 0, (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));


	materialAmbient[0] = 0.1;
	materialAmbient[1] = 0.18725;
	materialAmbient[2] = 0.1745;
	materialAmbient[3] = 1.0f;


	materialDiffuse[0] = 0.396;
	materialDiffuse[1] = 0.74151;
	materialDiffuse[2] = 0.69102;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.297254;
	materialSpecular[1] = 0.30829;
	materialSpecular[2] = 0.306678;
	materialSpecular[3] = 1.0f;

	materialShininess = 0.1 * 128;

	//transformations

	modelViewMatrix = mat4::identity();

	translationMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

	modelMatrix = translationMatrix;

	viewMatrix = mat4::identity();

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLightingEnable == TRUE)
	{
		glUniform1i(keyPressUniform, 1);

		glUniform3fv(lightAmbientUniform, 1, lightAmbient);

		glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

		glUniform3fv(lightSpecularUniform, 1, lightSpecular);

		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);

		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

		glUniform3fv(materialSpecularUniform, 1, materialSpecular);

		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}


	glDrawElements(GL_TRIANGLES, gNumSphereElements, GL_UNSIGNED_SHORT, 0); // 3rd parameter should be that

	//second column first sphere

	glViewport((horizontalDistance), (verticalDistance * 5), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	materialAmbient[0] = 0.329412;
	materialAmbient[1] = 0.223529;
	materialAmbient[2] = 0.027451;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.780392;
	materialDiffuse[1] = 0.568627;
	materialDiffuse[2] = 0.113725;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.992157;
	materialSpecular[1] = 0.941176;
	materialSpecular[2] = 0.807843;
	materialSpecular[3] = 1.0f;

	materialShininess = 0.21794872 * 128;

	//transformations

	modelViewMatrix = mat4::identity();

	translationMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

	modelMatrix = translationMatrix;

	viewMatrix = mat4::identity();

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLightingEnable == TRUE)
	{
		glUniform1i(keyPressUniform, 1);

		glUniform3fv(lightAmbientUniform, 1, lightAmbient);

		glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

		glUniform3fv(lightSpecularUniform, 1, lightSpecular);

		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);

		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

		glUniform3fv(materialSpecularUniform, 1, materialSpecular);

		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}


	glDrawElements(GL_TRIANGLES, gNumSphereElements, GL_UNSIGNED_SHORT, 0); // 3rd parameter should be that

	//second column second sphere

	glViewport((horizontalDistance), (verticalDistance * 4), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	materialAmbient[0] = 0.2125;
	materialAmbient[1] = 0.1275;
	materialAmbient[2] = 0.054;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.714;
	materialDiffuse[1] = 0.568627;
	materialDiffuse[2] = 0.113725;
	materialDiffuse[3] = 1.0f;

	materialSpecular[0] = 0.992157;
	materialSpecular[1] = 0.941176;
	materialSpecular[2] = 0.807843;
	materialSpecular[3] = 1.0f;


	materialShininess = 0.21794872 * 128;

	//transformations

	modelViewMatrix = mat4::identity();

	translationMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

	modelMatrix = translationMatrix;

	viewMatrix = mat4::identity();

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLightingEnable == TRUE)
	{
		glUniform1i(keyPressUniform, 1);

		glUniform3fv(lightAmbientUniform, 1, lightAmbient);

		glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

		glUniform3fv(lightSpecularUniform, 1, lightSpecular);

		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);

		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

		glUniform3fv(materialSpecularUniform, 1, materialSpecular);

		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}


	glDrawElements(GL_TRIANGLES, gNumSphereElements, GL_UNSIGNED_SHORT, 0); // 3rd parameter should be that


	//second column third sphere

	glViewport((horizontalDistance), (verticalDistance * 3), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	materialAmbient[0] = 0.25;
	materialAmbient[1] = 0.25;
	materialAmbient[2] = 0.25;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.4;
	materialDiffuse[1] = 0.4;
	materialDiffuse[2] = 0.4;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.774597;
	materialSpecular[1] = 0.774597;
	materialSpecular[2] = 0.774597;
	materialSpecular[3] = 1.0f;


	materialShininess = 0.6 * 128; 

	//transformations

	modelViewMatrix = mat4::identity();

	translationMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

	modelMatrix = translationMatrix;

	viewMatrix = mat4::identity();

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLightingEnable == TRUE)
	{
		glUniform1i(keyPressUniform, 1);

		glUniform3fv(lightAmbientUniform, 1, lightAmbient);

		glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

		glUniform3fv(lightSpecularUniform, 1, lightSpecular);

		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);

		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

		glUniform3fv(materialSpecularUniform, 1, materialSpecular);

		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}


	glDrawElements(GL_TRIANGLES, gNumSphereElements, GL_UNSIGNED_SHORT, 0); // 3rd parameter should be that


	//second column fourth sphere

	glViewport((horizontalDistance), (verticalDistance * 2), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	materialAmbient[0] = 0.19125;
	materialAmbient[1] = 0.0735;
	materialAmbient[2] = 0.0225;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.7038;
	materialDiffuse[1] = 0.27048;
	materialDiffuse[2] = 0.0828;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.256777;
	materialSpecular[1] = 0.137622;
	materialSpecular[2] = 0.086014;
	materialSpecular[3] = 1.0f;


	materialShininess = 0.1 * 128;

	//transformations

	modelViewMatrix = mat4::identity();

	translationMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

	modelMatrix = translationMatrix;

	viewMatrix = mat4::identity();

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLightingEnable == TRUE)
	{
		glUniform1i(keyPressUniform, 1);

		glUniform3fv(lightAmbientUniform, 1, lightAmbient);

		glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

		glUniform3fv(lightSpecularUniform, 1, lightSpecular);

		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);

		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

		glUniform3fv(materialSpecularUniform, 1, materialSpecular);

		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}


	glDrawElements(GL_TRIANGLES, gNumSphereElements, GL_UNSIGNED_SHORT, 0); // 3rd parameter should be that

	//second column fifth sphere

	glViewport((horizontalDistance), (verticalDistance * 1), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	materialAmbient[0] = 0.24725;
	materialAmbient[1] = 0.1995;
	materialAmbient[2] = 0.0745;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.75164;
	materialDiffuse[1] = 0.60648;
	materialDiffuse[2] = 0.22648;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.628281;
	materialSpecular[1] = 0.555802;
	materialSpecular[2] = 0.366065;
	materialSpecular[3] = 1.0f;


	materialShininess = 0.4 * 128;

	//transformations

	modelViewMatrix = mat4::identity();

	translationMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

	modelMatrix = translationMatrix;

	viewMatrix = mat4::identity();

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLightingEnable == TRUE)
	{
		glUniform1i(keyPressUniform, 1);

		glUniform3fv(lightAmbientUniform, 1, lightAmbient);

		glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

		glUniform3fv(lightSpecularUniform, 1, lightSpecular);

		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);

		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

		glUniform3fv(materialSpecularUniform, 1, materialSpecular);

		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}


	glDrawElements(GL_TRIANGLES, gNumSphereElements, GL_UNSIGNED_SHORT, 0); // 3rd parameter should be that


	//second column Sixth sphere

	glViewport((horizontalDistance), 0, (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	materialAmbient[0] = 0.19225;
	materialAmbient[1] = 0.19225;
	materialAmbient[2] = 0.19225;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.50754;
	materialDiffuse[1] = 0.50754;
	materialDiffuse[2] = 0.50754;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.508273;
	materialSpecular[1] = 0.508273;
	materialSpecular[2] = 0.508273;
	materialSpecular[3] = 1.0f;


	materialShininess = 0.4 * 128;

	//transformations

	modelViewMatrix = mat4::identity();

	translationMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

	modelMatrix = translationMatrix;

	viewMatrix = mat4::identity();

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLightingEnable == TRUE)
	{
		glUniform1i(keyPressUniform, 1);

		glUniform3fv(lightAmbientUniform, 1, lightAmbient);

		glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

		glUniform3fv(lightSpecularUniform, 1, lightSpecular);

		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);

		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

		glUniform3fv(materialSpecularUniform, 1, materialSpecular);

		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}


	glDrawElements(GL_TRIANGLES, gNumSphereElements, GL_UNSIGNED_SHORT, 0); // 3rd parameter should be that

	//Third column First sphere

	glViewport((horizontalDistance * 2), (verticalDistance * 5), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	materialAmbient[0] = 0.0f;
	materialAmbient[1] = 0.0f;
	materialAmbient[2] = 0.0f;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.01;
	materialDiffuse[1] = 0.01;
	materialDiffuse[2] = 0.01;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.5;
	materialSpecular[1] = 0.5;
	materialSpecular[2] = 0.5;
	materialSpecular[3] = 1.0f;


	materialShininess = 0.25 * 128;

	//transformations

	modelViewMatrix = mat4::identity();

	translationMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

	modelMatrix = translationMatrix;

	viewMatrix = mat4::identity();

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLightingEnable == TRUE)
	{
		glUniform1i(keyPressUniform, 1);

		glUniform3fv(lightAmbientUniform, 1, lightAmbient);

		glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

		glUniform3fv(lightSpecularUniform, 1, lightSpecular);

		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);

		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

		glUniform3fv(materialSpecularUniform, 1, materialSpecular);

		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}


	glDrawElements(GL_TRIANGLES, gNumSphereElements, GL_UNSIGNED_SHORT, 0); // 3rd parameter should be that

	//Third column Second sphere

	glViewport((horizontalDistance * 2), (verticalDistance * 4), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.1;
	materialAmbient[2] = 0.06;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.0;
	materialDiffuse[1] = 0.50980392;
	materialDiffuse[2] = 0.50980392;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.50196078;
	materialSpecular[1] = 0.50196078;
	materialSpecular[2] = 0.50196078;
	materialSpecular[3] = 1.0f;


	materialShininess = 0.25 * 128;

	//transformations

	modelViewMatrix = mat4::identity();

	translationMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

	modelMatrix = translationMatrix;

	viewMatrix = mat4::identity();

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLightingEnable == TRUE)
	{
		glUniform1i(keyPressUniform, 1);

		glUniform3fv(lightAmbientUniform, 1, lightAmbient);

		glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

		glUniform3fv(lightSpecularUniform, 1, lightSpecular);

		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);

		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

		glUniform3fv(materialSpecularUniform, 1, materialSpecular);

		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}


	glDrawElements(GL_TRIANGLES, gNumSphereElements, GL_UNSIGNED_SHORT, 0); // 3rd parameter should be that

	//Third column Third sphere

	glViewport((horizontalDistance * 2), (verticalDistance * 3), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.0;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.1;
	materialDiffuse[1] = 0.35;
	materialDiffuse[2] = 0.1;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.45;
	materialSpecular[1] = 0.55;
	materialSpecular[2] = 0.45;
	materialSpecular[3] = 1.0f;

	materialShininess = 0.25 * 128;

	//transformations

	modelViewMatrix = mat4::identity();

	translationMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

	modelMatrix = translationMatrix;

	viewMatrix = mat4::identity();

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLightingEnable == TRUE)
	{
		glUniform1i(keyPressUniform, 1);

		glUniform3fv(lightAmbientUniform, 1, lightAmbient);

		glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

		glUniform3fv(lightSpecularUniform, 1, lightSpecular);

		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);

		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

		glUniform3fv(materialSpecularUniform, 1, materialSpecular);

		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}


	glDrawElements(GL_TRIANGLES, gNumSphereElements, GL_UNSIGNED_SHORT, 0); // 3rd parameter should be that

	//Third column fourth sphere

	glViewport((horizontalDistance * 2), (verticalDistance * 2), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.0;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.5;
	materialDiffuse[1] = 0.0;
	materialDiffuse[2] = 0.0;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.7;
	materialSpecular[1] = 0.6;
	materialSpecular[2] = 0.6;
	materialSpecular[3] = 1.0f;

	materialShininess = 0.25 * 128;

	//transformations

	modelViewMatrix = mat4::identity();

	translationMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

	modelMatrix = translationMatrix;

	viewMatrix = mat4::identity();

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLightingEnable == TRUE)
	{
		glUniform1i(keyPressUniform, 1);

		glUniform3fv(lightAmbientUniform, 1, lightAmbient);

		glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

		glUniform3fv(lightSpecularUniform, 1, lightSpecular);

		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);

		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

		glUniform3fv(materialSpecularUniform, 1, materialSpecular);

		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}


	glDrawElements(GL_TRIANGLES, gNumSphereElements, GL_UNSIGNED_SHORT, 0); // 3rd parameter should be that

	//Third column fifth sphere

	glViewport((horizontalDistance * 2), (verticalDistance ), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.0;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.55;
	materialDiffuse[1] = 0.55;
	materialDiffuse[2] = 0.55;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.70;
	materialSpecular[1] = 0.70;
	materialSpecular[2] = 0.70;
	materialSpecular[3] = 1.0f;

	materialShininess = 0.25 * 128;

	//transformations

	modelViewMatrix = mat4::identity();

	translationMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

	modelMatrix = translationMatrix;

	viewMatrix = mat4::identity();

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLightingEnable == TRUE)
	{
		glUniform1i(keyPressUniform, 1);

		glUniform3fv(lightAmbientUniform, 1, lightAmbient);

		glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

		glUniform3fv(lightSpecularUniform, 1, lightSpecular);

		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);

		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

		glUniform3fv(materialSpecularUniform, 1, materialSpecular);

		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}


	glDrawElements(GL_TRIANGLES, gNumSphereElements, GL_UNSIGNED_SHORT, 0); // 3rd parameter should be that

	//Third column sixth sphere

	glViewport((horizontalDistance * 2), 0, (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.0;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.5;
	materialDiffuse[1] = 0.5;
	materialDiffuse[2] = 0.0;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.60;
	materialSpecular[1] = 0.60;
	materialSpecular[2] = 0.50;
	materialSpecular[3] = 1.0f;

	materialShininess = 0.25 * 128;

	//transformations

	modelViewMatrix = mat4::identity();

	translationMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

	modelMatrix = translationMatrix;

	viewMatrix = mat4::identity();

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLightingEnable == TRUE)
	{
		glUniform1i(keyPressUniform, 1);

		glUniform3fv(lightAmbientUniform, 1, lightAmbient);

		glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

		glUniform3fv(lightSpecularUniform, 1, lightSpecular);

		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);

		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

		glUniform3fv(materialSpecularUniform, 1, materialSpecular);

		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}


	glDrawElements(GL_TRIANGLES, gNumSphereElements, GL_UNSIGNED_SHORT, 0); // 3rd parameter should be that

	//Fourth Column First sphere

	glViewport((horizontalDistance * 3), (verticalDistance * 5), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	materialAmbient[0] = 0.02;
	materialAmbient[1] = 0.02;
	materialAmbient[2] = 0.02;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.01;
	materialDiffuse[1] = 0.01;
	materialDiffuse[2] = 0.01;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.4;
	materialSpecular[1] = 0.4;
	materialSpecular[2] = 0.4;
	materialSpecular[3] = 1.0f;

	materialShininess = 0.078125 * 128;

	//transformations

	modelViewMatrix = mat4::identity();

	translationMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

	modelMatrix = translationMatrix;

	viewMatrix = mat4::identity();

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLightingEnable == TRUE)
	{
		glUniform1i(keyPressUniform, 1);

		glUniform3fv(lightAmbientUniform, 1, lightAmbient);

		glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

		glUniform3fv(lightSpecularUniform, 1, lightSpecular);

		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);

		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

		glUniform3fv(materialSpecularUniform, 1, materialSpecular);

		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}


	glDrawElements(GL_TRIANGLES, gNumSphereElements, GL_UNSIGNED_SHORT, 0); // 3rd parameter should be that

	//Fourth Column Second sphere

	glViewport((horizontalDistance * 3), (verticalDistance * 4), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.05;
	materialAmbient[2] = 0.05;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.4;
	materialDiffuse[1] = 0.5;
	materialDiffuse[2] = 0.5;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.04;
	materialSpecular[1] = 0.7;
	materialSpecular[2] = 0.7;
	materialSpecular[3] = 1.0f;

	materialShininess = 0.078125 * 128;

	//transformations

	modelViewMatrix = mat4::identity();

	translationMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

	modelMatrix = translationMatrix;

	viewMatrix = mat4::identity();

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLightingEnable == TRUE)
	{
		glUniform1i(keyPressUniform, 1);

		glUniform3fv(lightAmbientUniform, 1, lightAmbient);

		glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

		glUniform3fv(lightSpecularUniform, 1, lightSpecular);

		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);

		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

		glUniform3fv(materialSpecularUniform, 1, materialSpecular);

		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}


	glDrawElements(GL_TRIANGLES, gNumSphereElements, GL_UNSIGNED_SHORT, 0); // 3rd parameter should be that

	//Fourth Column Third sphere

	glViewport((horizontalDistance * 3), (verticalDistance * 3), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.05;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.4;
	materialDiffuse[1] = 0.5;
	materialDiffuse[2] = 0.4;
	materialDiffuse[3] = 1.0f;

	materialSpecular[0] = 0.04;
	materialSpecular[1] = 0.7;
	materialSpecular[2] = 0.04;
	materialSpecular[3] = 1.0f;

	materialShininess = 0.078125 * 128;

	//transformations

	modelViewMatrix = mat4::identity();

	translationMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

	modelMatrix = translationMatrix;

	viewMatrix = mat4::identity();

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLightingEnable == TRUE)
	{
		glUniform1i(keyPressUniform, 1);

		glUniform3fv(lightAmbientUniform, 1, lightAmbient);

		glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

		glUniform3fv(lightSpecularUniform, 1, lightSpecular);

		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);

		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

		glUniform3fv(materialSpecularUniform, 1, materialSpecular);

		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}


	glDrawElements(GL_TRIANGLES, gNumSphereElements, GL_UNSIGNED_SHORT, 0); // 3rd parameter should be that

	//Fourth Column fourth sphere

	glViewport((horizontalDistance * 3), (verticalDistance * 2), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	materialAmbient[0] = 0.05;
	materialAmbient[1] = 0.0;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.5;
	materialDiffuse[1] = 0.4;
	materialDiffuse[2] = 0.4;
	materialDiffuse[3] = 1.0f;

	materialSpecular[0] = 0.7;
	materialSpecular[1] = 0.04;
	materialSpecular[2] = 0.04;
	materialSpecular[3] = 1.0f;

	materialShininess = 0.078125 * 128;

	//transformations

	modelViewMatrix = mat4::identity();

	translationMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

	modelMatrix = translationMatrix;

	viewMatrix = mat4::identity();

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLightingEnable == TRUE)
	{
		glUniform1i(keyPressUniform, 1);

		glUniform3fv(lightAmbientUniform, 1, lightAmbient);

		glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

		glUniform3fv(lightSpecularUniform, 1, lightSpecular);

		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);

		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

		glUniform3fv(materialSpecularUniform, 1, materialSpecular);

		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}


	glDrawElements(GL_TRIANGLES, gNumSphereElements, GL_UNSIGNED_SHORT, 0); // 3rd parameter should be that

	//Fourth Column fifth sphere

	glViewport((horizontalDistance * 3), (verticalDistance * 1), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	materialAmbient[0] = 0.05;
	materialAmbient[1] = 0.05;
	materialAmbient[2] = 0.05;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.5;
	materialDiffuse[1] = 0.5;
	materialDiffuse[2] = 0.5;
	materialDiffuse[3] = 1.0f;

	materialSpecular[0] = 0.7;
	materialSpecular[1] = 0.7;
	materialSpecular[2] = 0.7;
	materialSpecular[3] = 1.0f;

	materialShininess = 0.078125 * 128;

	//transformations

	modelViewMatrix = mat4::identity();

	translationMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

	modelMatrix = translationMatrix;

	viewMatrix = mat4::identity();

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLightingEnable == TRUE)
	{
		glUniform1i(keyPressUniform, 1);

		glUniform3fv(lightAmbientUniform, 1, lightAmbient);

		glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

		glUniform3fv(lightSpecularUniform, 1, lightSpecular);

		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);

		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

		glUniform3fv(materialSpecularUniform, 1, materialSpecular);

		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}


	glDrawElements(GL_TRIANGLES, gNumSphereElements, GL_UNSIGNED_SHORT, 0); // 3rd parameter should be that

	//Fourth Column sixth sphere

	glViewport((horizontalDistance * 3), 0, (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	materialAmbient[0] = 0.05;
	materialAmbient[1] = 0.05;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.5;
	materialDiffuse[1] = 0.5;
	materialDiffuse[2] = 0.4;
	materialDiffuse[3] = 1.0f;

	materialSpecular[0] = 0.7;
	materialSpecular[1] = 0.7;
	materialSpecular[2] = 0.04;
	materialSpecular[3] = 1.0f;


	materialShininess = 0.078125 * 128;

	//transformations

	modelViewMatrix = mat4::identity();

	translationMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

	modelMatrix = translationMatrix;

	viewMatrix = mat4::identity();

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLightingEnable == TRUE)
	{
		glUniform1i(keyPressUniform, 1);

		glUniform3fv(lightAmbientUniform, 1, lightAmbient);

		glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

		glUniform3fv(lightSpecularUniform, 1, lightSpecular);

		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);

		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

		glUniform3fv(materialSpecularUniform, 1, materialSpecular);

		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}


	glDrawElements(GL_TRIANGLES, gNumSphereElements, GL_UNSIGNED_SHORT, 0); // 3rd parameter should be that

	// *** unbind vao ***
	glBindVertexArray(0);

	//step 5: Unuse shader program object
	glUseProgram(0);

	SwapBuffers(ghdc);

}

void update(void)
{
	//code
	//only at time of change animation here code will come

	if (keyPressed == 1)
	{
		if (angleForXRotation < (2 * M_PI))   // rotation around X axis
		{
			lightPosition[0] = 0.0f;
			lightPosition[1] = 45.0f * (GLfloat)sin(angleForXRotation);
			lightPosition[2] = 45.0f * (GLfloat)cos(angleForXRotation);
			lightPosition[3] = 1.0f;

			angleForXRotation = angleForXRotation + 0.01;
		}

		else
		{
			angleForXRotation = angleForXRotation - (2 * M_PI);
		}
		
	}

	if (keyPressed == 2)   // rotation around Y axis
	{
		if (angleForYRotation < (2 * M_PI))
		{
			lightPosition[0] = 45.0f * (GLfloat)sin(angleForYRotation);
			lightPosition[1] = 0.0f;
			lightPosition[2] = 45.0f * (GLfloat)cos(angleForYRotation);
			lightPosition[3] = 1.0f;

			angleForYRotation = angleForYRotation + 0.01;
		}

		else
		{
			angleForYRotation = angleForYRotation - (2 * M_PI);
		}

	}

	if (keyPressed == 3)   // rotation around Z axis
	{
		if (angleForZRotation < (2 * M_PI))
		{
			lightPosition[0] = 45.0f * (GLfloat)sin(angleForZRotation);
			lightPosition[1] = 45.0f * (GLfloat)cos(angleForZRotation);
			lightPosition[2] = 0.0f;
			lightPosition[3] = 1.0f;

			angleForZRotation = angleForZRotation + 0.01;
		}

		else
		{
			angleForZRotation = angleForZRotation - (2 * M_PI);
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

	//step 9 : delete vbo of color

	//sphere

	// delete vbo for element
	if (gVbo_sphere_element)
	{
		glDeleteBuffers(1, &gVbo_sphere_element);
		gVbo_sphere_element = 0;
	}

	// delete vbo for normal
	if (gVbo_sphere_normal)
	{
		glDeleteBuffers(1, &gVbo_sphere_normal);
		gVbo_sphere_normal = 0;
	}


	//step 10 : delete vbo for position

	if (gVbo_sphere_position)
	{
		glDeleteBuffers(1, &gVbo_sphere_position);
		gVbo_sphere_position = 0;
	}

	//step 11 : delete VAO 

	if (gVao)
	{
		glDeleteVertexArrays(1, &gVao);
		gVao = 0;
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

