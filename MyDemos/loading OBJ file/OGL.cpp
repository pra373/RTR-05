// Windows header files
// Common headers 
#include<windows.h> //For all WIN32 API's
#include<stdio.h>   //for file I/O
#include<stdlib.h>  //for exit()

#include "OGL.h" // swatachi header file
#include "Sphere.h" // header for sphere
#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<sstream>



// OpenGL header Files
#include<gl/glew.h>   //this must be before gl/gl.h header file
#include<gl/GL.h> 
#include"vmath.h"



using namespace vmath;
using namespace std;

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


GLuint vao_Letters = 0;
//GLuint gVbo_sphere_position = 0;
//GLuint gVbo_sphere_normal = 0;
//GLuint gVbo_sphere_element = 0;
//GLuint vbo_Texcoord = 0;

GLuint vbo_Position = 0;
GLuint vbo_Normals = 0;


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




////unsigned int  gNumVertices;
//unsigned int  gNumSphereElements;


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


BOOL bLightingEnable = FALSE;

BOOL bAnimationEnable = FALSE;


// Light Variables

GLfloat lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //white diffuse light
GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightPosition[] = { 100.0f, 100.0f, 100.0f, 1.0f }; //


//GLfloat materialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//GLfloat materialDiffuse[] = { 0.802268f, 0.047259f,  0.085333f, 1.0f };
//GLfloat materialSpecular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
//GLfloat materialShininess = 250.0f;

GLfloat materialAmbient[] = { 0.2f, 0.0113f, 0.01145f, 1.0f };
GLfloat materialDiffuse[] = { 1.0f, 0.0f,  0.0f, 1.0f };
GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
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

GLfloat alphaValue = 0.0f;

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
		TEXT("Prathamesh Laxmikant Paropkari :    First OBJ loading Program  "),
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
	//Local variable declaration for sphere
	/*float sphere_position[1146];
	float sphere_normals[1146];
	float sphere_texcoord[764];
	unsigned short sphere_elements[2280];*/


	//function declarations
	void printGLInfo(void);
	void uninitialize(void);
	void resize(int, int);
	void loadOBJFile(void);
	void printVertices(void);



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

	 printVertices();

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
		 "void main(void)\n"
		 "{\n"
		 "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;\n"  //v
		 "otransformedNormals = (mat3(uViewMatrix * uModelMatrix) * aNormal);\n" // v
		 "olightDirection = (vec3 (ulightPosition - eyeCoordinates));\n" //v
		 "oviewerVector = (- eyeCoordinates.xyz);\n"
		 "gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;\n"
		 "vblendFactor = ublendFactor;\n"
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
		"uniform vec3 ulightAmbient;\n"
		"uniform vec3 ulightDiffuse;\n"
		"uniform vec3 ulightSpecular;\n"
		"uniform vec3 uMaterialAmbient;\n"
		"uniform vec3 uMaterialDiffuse;\n"
		"uniform vec3 uMaterialSpecular;\n"
		"uniform float uMaterialShininess;\n"
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

	//keyPressUniform = glGetUniformLocation(shaderProgramObject, "uKeyPress");




	/*getSphereVertexData(sphere_position, sphere_normals, sphere_texcoord, sphere_elements);*/

	// gNumVertices = getNumberOfSphereVertices();

	/*gNumSphereElements = getNumberOfSphereElements();*/




	////step 13: Create vertex array object

	// vao
	glGenVertexArrays(1, &vao_Letters);
	glBindVertexArray(vao_Letters);

	// position vbo
	glGenBuffers(1, &vbo_Position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Position);
	glBufferData(GL_ARRAY_BUFFER, positionBuffer.size() * sizeof(float), positionBuffer.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// normal vbo
	glGenBuffers(1, &vbo_Normals);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Normals);
	glBufferData(GL_ARRAY_BUFFER, normalBuffer.size() * sizeof(float), normalBuffer.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	//// VBO for Texcoord
	//glGenBuffers(1, &vbo_Texcoord);

	//glBindBuffer(GL_ARRAY_BUFFER, vbo_Texcoord);

	//glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_texcoord), sphere_texcoord, GL_STATIC_DRAW);

	//glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	//glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//step 18 : unbind with VAO
	glBindVertexArray(0);



	//// element vbo
	//glGenBuffers(1, &gVbo_sphere_element);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);

	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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


	//glMatrixMode(GL_PROJECTION); //not allowed in PP 
	//glLoadIdentity();            not allowed in PP 

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	//set Perspective projection matrix

	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	//gluPerspective(45.0f,((GLfloat)width / (GLfloat)height),0.1f,100.0f);  //not allowed in PP 


}

void display(void)
{
	//code
	// 3rd Change GL_DEPTH_BUFFER_BIT
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//PP steps for display function

	//step 1: Use shader program object

	glUseProgram(shaderProgramObject);




	//Sphere


	//transformations

	mat4 modelViewMatrix = mat4::identity();

	mat4 translationMatrix = mat4::identity();

	mat4 rotationMatrix = mat4::identity();

	translationMatrix = vmath::translate(-4.0f, 0.0f, -7.5f);

	rotationMatrix = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);

	mat4 modelMatrix = translationMatrix * rotationMatrix;

	mat4 viewMatrix = mat4::identity();

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	glUniform3fv(lightAmbientUniform, 1, lightAmbient);

	glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

	glUniform3fv(lightSpecularUniform, 1, lightSpecular);

	glUniform4fv(lightPositionUniform, 1, lightPosition);

	glUniform3fv(materialAmbientUniform, 1, materialAmbient);

	glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

	glUniform3fv(materialSpecularUniform, 1, materialSpecular);

	glUniform1f(materialShininessUniform, materialShininess);

	glUniform1f(blendFactorUniform, alphaValue);

	
	

	glEnable(GL_BLEND);
	// *** bind vao ***
	glBindVertexArray(vao_Letters);

	GLint numVertices = positionBuffer.size() / 3;
	
	glDrawArrays(GL_TRIANGLES, 0, numVertices);
	 

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

	/*GLfloat materialAmbient[] = { 0.2f, 0.0113f, 0.01145f, 1.0f };
	GLfloat materialDiffuse[] = { 1.0f, 0.0f,  0.0f, 1.0f };
	GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat materialShininess = 128.0f;*/

	// Update the alpha value in the material properties array

	/*GLfloat alphaValue = 0.0f;

	if (alphaValue < 1.0f)
	{
		materialAmbient[3] = materialAmbient[3] + alphaValue;
		materialDiffuse[3] = materialDiffuse[3] + alphaValue;
		materialSpecular[3] = materialSpecular[3] + alphaValue;

		alphaValue = alphaValue + 0.00000001f;
	}	*/

	

	if (alphaValue < 1.0f)
	{
		alphaValue += 0.003f;
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


	//step 11 : delete VAO 

	if (vao_Letters)
	{
		glDeleteVertexArrays(1, &vao_Letters);
		vao_Letters = 0;
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

void loadOBJFile(void)
{
	ifstream dataIn;

	dataIn.open("NewObjFile.obj");

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

			if (sso3 >> texCoordX >> texCoordY )
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

			GLint vertexIndex, normalIndex, textureIndex;

			GLchar seperator;

			while (sso4 >> vertexIndex >> seperator >> textureIndex >> seperator >> normalIndex)
			{
				vertexIndices.push_back(vertexIndex);
				texCoordIndices.push_back(textureIndex);
				normalIndices.push_back(normalIndex);

				sso4.ignore();

			}

			


		}
	}

	//now fill position buffer and normal buffer according to the entries in face indices vectors

	int num1 = 0;

	while (num1 < vertexIndices.size())
	{
		//get current vertex index from the vertexIndices vector
		int vertexIndex = vertexIndices[num1] - 1;

		//calculate start index in vertices data vector as data is stored in x,y,z format
		int startIndex = vertexIndex * 3;

		// Extract x, y, z components of the current vertex
		GLfloat vertexX = vertices[startIndex];
		GLfloat vertexY = vertices[startIndex + 1];
		GLfloat vertexZ = vertices[startIndex + 2];

		// Store the vertex data in your position buffer
	
		positionBuffer.push_back(vertexX);
		positionBuffer.push_back(vertexY);
		positionBuffer.push_back(vertexZ);

		// Move to the next vertex index
		num1++;
	}

	int num2 = 0;

	while (num2 < normalIndices.size())
	{
		//get current vertex index from the vertexIndices vector
		int normalIndex = normalIndices[num2] - 1 ;

		//calculate start index in vertices data vector as data is stored in x,y,z format
		int startIndex = normalIndex * 3;

		// Extract x, y, z components of the current vertex
		GLfloat normalX = normals[startIndex];
		GLfloat normalY = normals[startIndex + 1];
		GLfloat normalZ = normals[startIndex + 2];

		// Store the vertex data in your position buffer

		normalBuffer.push_back(normalX);
		normalBuffer.push_back(normalY);
		normalBuffer.push_back(normalZ);

		// Move to the next vertex index
		num2++;
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

	fprintf(gpFile, "\n\n**************************** Printing Normals*****************************************\n\n");

	while (j < normals.size())
	{
		fprintf(gpFile, "X = %f\n", normals[j]);

		j++;

		fprintf(gpFile, "Y = %f\n", normals[j]);

		j++;

		fprintf(gpFile, "Z = %f\n\n\n", normals[j]);



		j++;
	}

	fprintf(gpFile, "\n\n**************************** Printing Texture Coordinates*****************************************\n\n");

	while (k < texCoords.size())
	{
		fprintf(gpFile, "X = %f\n", texCoords[k]);

		k++;

		fprintf(gpFile, "Y = %f\n\n", texCoords[k]);

		k++;

	}

	fprintf(gpFile, "\n\n**************************** Printing Vertex Indices indices*****************************************\n\n");

	while (l < vertexIndices.size())
	{
		fprintf(gpFile, "V = %d\n", vertexIndices[l]);

		l++;
	}


	fprintf(gpFile, "\n\n**************************** Printing texture Indices indices*****************************************\n\n");

	while (m < texCoordIndices.size())
	{
		fprintf(gpFile, "T = %d\n", texCoordIndices[m]);

		m++;
	}

	fprintf(gpFile, "\n\n**************************** Printing Normal Indices indices*****************************************\n\n");

	while (n < normalIndices.size())
	{
		fprintf(gpFile, "N = %d\n", normalIndices[n]);

		n++;
	}

	fprintf(gpFile, "\n\n**************************** Printing Position buffer *****************************************\n\n");

	int num = 0;
	while (num < positionBuffer.size())
	{
		fprintf(gpFile, "vertex %d :  X = %f  y = %f  z = %f\n", num, positionBuffer[num], positionBuffer[num + 1], positionBuffer[num + 2]);

		num++;
	}

	fprintf(gpFile, "\n\n**************************** Printing normal buffer *****************************************\n\n");

	num = 0;
	while (num < normalBuffer.size())
	{
		fprintf(gpFile, "normal %d :  X = %f  y = %f  z = %f\n", num, normalBuffer[num], normalBuffer[num + 1], normalBuffer[num + 2]);

		num++;
	}

}

