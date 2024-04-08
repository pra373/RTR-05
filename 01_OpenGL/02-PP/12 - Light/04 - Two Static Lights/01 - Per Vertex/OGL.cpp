// Windows header files
// Common headers 
#include<windows.h> //For all WIN32 API's
#include<stdio.h>   //for file I/O
#include<stdlib.h>  //for exit()

#include "OGL.h" // swatachi header file

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
//#pragma comment(lib, "glu32.lib") //file me OpenGL    //dont need in PP (There is no GLU utility in PP)

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


GLuint shaderProgramObject = 0;

GLfloat AnglePyramid = 0.0f;


enum
{
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_COLOR,
	AMC_ATTRIBUTE_NORMAL
};


GLuint vaoPyramid = 0;

GLuint vbo_Position_Pyramid = 0;


GLuint vbo_Normal = 0;

//GLuint uMVPMatrix;
mat4 perspectiveProjectionMatrix;   //using 4*4 matrix.
                                   //mat4 - represents 4*4 matrix.   
								   //mat4 is not c++ datattype, it is defined in vmath.h.

// Uniforms

GLuint modelMatrixUniform = 0;
GLuint viewMatrixUniform = 0;
GLuint projectionMatrixUniform = 0;

GLuint lightDiffuseUniform[2];
GLuint lightAmbientUniform[2];
GLuint lightSpecularUniform[2];
GLuint lightPositionUniform[2];

GLuint materialDiffuseUniform = 0;
GLuint materialAmbientUniform = 0;
GLuint materialSpecularUniform = 0;
GLuint materialShininessUniform = 0;

GLuint keyPressUniform = 0;

BOOL bLightingEnable = FALSE;

BOOL bAnimationEnable = FALSE;


// Light Variables

GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // white diffuse light
GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f }; 
//GLfloat materialDiffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };


GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialShininess = 128.0f;


struct Light
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec4 position;
};

struct Light light[2];



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
							TEXT("Prathamesh Laxmikant Paropkari : Two Static Lights On Pyramid"),
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

					if (bAnimationEnable == TRUE)
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
	case 'A':
	case 'a':
		if (bAnimationEnable == FALSE)
		{
			bAnimationEnable = TRUE;
		}
		else
		{
			bAnimationEnable = FALSE;
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
	void uninitialize(void);
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
		"uniform vec3 ulightAmbient[2];\n"
		"uniform vec3 ulightDiffuse[2];\n"
		"uniform vec3 ulightSpecular[2];\n"
		"uniform vec4 ulightPosition[2];\n"
		"uniform vec3 uMaterialAmbient;\n"
		"uniform vec3 uMaterialDiffuse;\n"
		"uniform vec3 uMaterialSpecular;\n"
		"uniform float uMaterialShininess;\n"
		"uniform int uKeyPress;\n"
		"out vec3 oPhongADSLight;\n"
		"void main(void)\n"
		"{\n"
		"if(uKeyPress == 1)\n"
		"{\n"
		"vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;\n"
		"vec3 transformedNormals = normalize(mat3(uViewMatrix * uModelMatrix) * aNormal);\n"
		"vec3 lightAmbient[2];\n"
		"vec3 lightDirection[2];\n"
		"vec3 lightDiffuse[2];\n"
		"vec3 reflectionVector[2];\n"
		"vec3 lightSpecular[2];\n"
		"for(int i = 0; i < 2; i++)\n"
		"{\n"
		"lightAmbient[i] = ulightAmbient[i] * uMaterialAmbient;\n"
		"lightDirection[i] = normalize(vec3 (ulightPosition[i] - eyeCoordinates));\n"
		"lightDiffuse[i] = ulightDiffuse[i] * uMaterialDiffuse * max(dot(lightDirection[i], transformedNormals),0.0);\n"
		"reflectionVector[i] = reflect(-lightDirection[i], transformedNormals);\n"
		"vec3 viewerVector = normalize (- eyeCoordinates.xyz);\n"
		"lightSpecular[i] = ulightSpecular[i] * uMaterialSpecular * pow(max(dot(reflectionVector[i], viewerVector), 0.0),uMaterialShininess);\n"
		"oPhongADSLight = oPhongADSLight + lightAmbient[i] + lightDiffuse[i] + lightSpecular[i];\n"
		"}\n"
		"}\n"
		"else\n"
		"{\n"
		"oPhongADSLight = vec3( 0.0, 0.0, 0.0);\n"
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
		"in vec3 oPhongADSLight;\n"
		"uniform int uKeyPress;\n"
		"out vec4 FragColor;\n"
		"void main(void)\n"
		"{\n"
		"if(uKeyPress == 1)\n"
		"{\n"
		"  FragColor = vec4 (oPhongADSLight, 1.0);\n"
		"}\n"
		"else\n"
		"{\n"
		"  FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
		"}\n"
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

	 glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_COLOR, "aColor");

	 glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_NORMAL , "aNormal");

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

	 lightAmbientUniform[0] = glGetUniformLocation(shaderProgramObject, "ulightAmbient[0]");

	 lightDiffuseUniform[0] = glGetUniformLocation(shaderProgramObject, "ulightDiffuse[0]");

	 lightSpecularUniform[0] = glGetUniformLocation(shaderProgramObject, "ulightSpecular[0]");

	 lightPositionUniform[0] = glGetUniformLocation(shaderProgramObject, "ulightPosition[0]");

	 lightAmbientUniform[1] = glGetUniformLocation(shaderProgramObject, "ulightAmbient[1]");

	 lightDiffuseUniform[1] = glGetUniformLocation(shaderProgramObject, "ulightDiffuse[1]");

	 lightSpecularUniform[1] = glGetUniformLocation(shaderProgramObject, "ulightSpecular[1]");

	 lightPositionUniform[1] = glGetUniformLocation(shaderProgramObject, "ulightPosition[1]");


	 materialAmbientUniform = glGetUniformLocation(shaderProgramObject, "uMaterialAmbient");

	 materialDiffuseUniform = glGetUniformLocation(shaderProgramObject, "uMaterialDiffuse");


	 materialSpecularUniform = glGetUniformLocation(shaderProgramObject, "uMaterialSpecular");

	 materialShininessUniform = glGetUniformLocation(shaderProgramObject, "uMaterialShininess");

	 keyPressUniform = glGetUniformLocation(shaderProgramObject, "uKeyPress");


	
	 // position
	 GLfloat pyramidVertices[] =
	 {
		 // front
		 0.0f,  1.0f,  0.0f, // front-top
		-1.0f, -1.0f,  1.0f, // front-left
		 1.0f, -1.0f,  1.0f, // front-right

		 // right
		 0.0f,  1.0f,  0.0f, // right-top
		 1.0f, -1.0f,  1.0f, // right-left
		 1.0f, -1.0f, -1.0f, // right-right

		 // back
		 0.0f,  1.0f,  0.0f, // back-top
		 1.0f, -1.0f, -1.0f, // back-left
		-1.0f, -1.0f, -1.0f, // back-right

		// left
		0.0f,  1.0f,  0.0f, // left-top
	   -1.0f, -1.0f, -1.0f, // left-left
	   -1.0f, -1.0f,  1.0f, // left-right
	 };


	 // normals
	 GLfloat pyramidNormals[] =
	 {
		 // front
		 0.000000f, 0.447214f,  0.894427f, // front-top
		 0.000000f, 0.447214f,  0.894427f, // front-left
		 0.000000f, 0.447214f,  0.894427f, // front-right

		 // right			    
		 0.894427f, 0.447214f,  0.000000f, // right-top
		 0.894427f, 0.447214f,  0.000000f, // right-left
		 0.894427f, 0.447214f,  0.000000f, // right-right

		 // back
		 0.000000f, 0.447214f, -0.894427f, // back-top
		 0.000000f, 0.447214f, -0.894427f, // back-left
		 0.000000f, 0.447214f, -0.894427f, // back-right

		 // left
		-0.894427f, 0.447214f,  0.000000f, // left-top
		-0.894427f, 0.447214f,  0.000000f, // left-left
		-0.894427f, 0.447214f,  0.000000f, // left-right
	 };




	 ////step 13: Create vertex array object
	 //VAO

    //step 13: Create vertex array object
	 //VAO

	 // ********************************************** for Pyramid ***********************************************

	 glGenVertexArrays(1, &vaoPyramid);

	 //step 14: Bind with VAO

	 glBindVertexArray(vaoPyramid);

	 //step 15: Create vertex buffer object (VBO) for position

	 //VBO for position

	 glGenBuffers(1, &vbo_Position_Pyramid);

	 //step 16: Bind with VBO of position

	 glBindBuffer(GL_ARRAY_BUFFER, vbo_Position_Pyramid);

	 glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	 glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);


	 glGenBuffers(1, &vbo_Normal);

	 glBindBuffer(GL_ARRAY_BUFFER, vbo_Normal);

	 glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidNormals), pyramidNormals, GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	 glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);

	 //step 18 : unbind with VAO
	 glBindVertexArray(0);



	// 2nd change for depth or Enabling depth
	
	glClearDepth(1.0f); // Compulsory
	glEnable(GL_DEPTH_TEST); // 8 test paike ek(depth) test enable kr
	glDepthFunc(GL_LEQUAL); 
	


	// Set The ClearColor Of Window 
	// Clear OpenGl Stars
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 

	

	light[0].ambient = vec3(0.0, 0.0, 0.0);
	light[1].ambient = vec3(0.0, 0.0, 0.0);
	light[0].diffuse = vec3(1.0, 0.0, 0.0);
	light[1].diffuse = vec3(0.0, 0.0, 1.0);
	light[0].specular = vec3(1.0, 0.0, 0.0);
	light[1].specular = vec3(0.0, 0.0, 1.0);
	light[0].position = vec4(-2.0, 0.0, 0.0, 1.0);
	light[1].position = vec4(2.0, 0.0, 1.0, 1.0);


	perspectiveProjectionMatrix = vmath::mat4::identity();

	resize(WIN_WIDTH, WIN_HEIGHT);

	

	//code

	return(0);
}

void resize(int width, int height)
{
	// code
	if(height <= 0)
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


	//Pyramid


     //transformations

	mat4 modelViewMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 translationMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();

	// Set up rotation matrix
	rotationMatrix = vmath::rotate(AnglePyramid, 0.0f, 1.0f, 0.0f);

	// Set up translation matrix
	translationMatrix = vmath::translate(0.0f, 0.0f, -4.0f);

	// Combine translation and rotation
	modelViewMatrix = translationMatrix * rotationMatrix;

	///push above mvp into vertex shader's mvp uniform

	// Pass modelViewMatrix to the vertex shader's uniform
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelViewMatrix);

	// Pass viewMatrix to the vertex shader's uniform
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

	// Pass perspectiveProjectionMatrix to the vertex shader's uniform
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	

	if (bLightingEnable == TRUE)
	{
		glUniform1i(keyPressUniform, 1);

		glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);

		glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);

		glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);

		glUniform4fv(lightPositionUniform[0], 1, light[0].position);

		glUniform3fv(lightAmbientUniform[1], 1, light[1].ambient);

		glUniform3fv(lightDiffuseUniform[1], 1, light[1].diffuse);

		glUniform3fv(lightSpecularUniform[1], 1, light[1].specular);

		glUniform4fv(lightPositionUniform[1], 1, light[1].position);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);

		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

		glUniform3fv(materialSpecularUniform, 1, materialSpecular);

		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}


	//step 2: Bind with (VAO)

	glBindVertexArray(vaoPyramid);

	//step 3: Draw the geometry

	glDrawArrays(GL_TRIANGLES, 0, 12);


	//step 4: unBind with (VAO)

	glBindVertexArray(0);

	//step 5: Unuse shader program object
	glUseProgram(0);
	
	SwapBuffers(ghdc);

}

void update(void)
{
	//code
	//only at time of change animation here code will come

	AnglePyramid = AnglePyramid - 0.2f;

	if (AnglePyramid <= 0.0f)
	{
		AnglePyramid = 360.0f;
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

	//square

	
	// delete vbo for normal
	if (vbo_Normal)
	{
		glDeleteBuffers(1, &vbo_Normal);
		vbo_Normal = 0;
	}

	//step 10 : delete vbo for position
	if (vbo_Position_Pyramid)
	{
		glDeleteBuffers(1, &vbo_Position_Pyramid);
		vbo_Position_Pyramid = 0;
	}



	//step 11 : delete VAO 

	if (vaoPyramid)
	{
		glDeleteVertexArrays(1, &vaoPyramid);
		vaoPyramid = 0;
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




