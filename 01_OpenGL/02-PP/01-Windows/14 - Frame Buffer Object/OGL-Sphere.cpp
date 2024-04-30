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
#include "Sphere.h"



using namespace vmath;

//#include<GL/glu.h>  //not allowed in PP  (There is no GLU utility in PP)

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Link With OpenGL Library
#pragma comment(lib,"glew32.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib,"Sphere.lib")
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



GLuint shaderProgramObjectPerVertex_Sphere = 0;
GLuint shaderProgramObjectPerFragment_Sphere = 0;




enum
{
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_COLOR,
	AMC_ATTRIBUTE_NORMAL
};


GLuint numSphereElements = 0;


//GLuint vaoPyramid = 0;
GLuint vaoSphere_Sphere = 0;

//GLuint vbo_Position_Pyramid = 0;
//GLuint vbo_Color_Pyramid = 0;

GLuint vbo_Position_Sphere = 0;
GLuint vbo_Normal_Sphere = 0;
GLuint vbo_Element_Sphere = 0;

//GLuint uMVPMatrix;
mat4 perspectiveProjectionMatrix_Sphere;   //using 4*4 matrix.
                                   //mat4 - represents 4*4 matrix.   
								   //mat4 is not c++ datattype, it is defined in vmath.h.

// Uniforms

GLuint modelMatrixUniform_Sphere = 0;
GLuint viewMatrixUniform_Sphere = 0;
GLuint projectionMatrixUniform_Sphere = 0;

//for per vertex lighting

GLuint lightDiffuseUniform_Sphere[3];
GLuint lightAmbientUniform_Sphere[3];
GLuint lightSpecularUniform_Sphere[3];
GLuint lightPositionUniform_Sphere[3];

GLuint materialDiffuseUniform_Sphere = 0;
GLuint materialAmbientUniform_Sphere = 0;
GLuint materialSpecularUniform_Sphere = 0;
GLuint materialShininessUniform_Sphere = 0;

//for per fragment lighting


GLuint keyPressUniform_Sphere = 0;

BOOL bLightingEnable = FALSE;

BOOL bAnimationEnable = FALSE;

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

struct Light light[3];

struct perFragLightSources
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec4 position;
};

struct perFragLightSources fLights[3];



BOOL enablePerFragmentLighting = FALSE;
BOOL enablePerVertexLighting = FALSE;

//light movement variable

GLfloat vLight0 = 0.0f;
GLfloat vLight1 = 0.0f;
GLfloat vLight2 = 0.0f;

GLfloat fLight0 = 0.0f;
GLfloat fLight1 = 0.0f;
GLfloat fLight2 = 0.0f;



// Entry-Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) 
{
	// Function declarations
	int initialize_Sphere(void);
	void uninitialize_Sphere(void);
	void display_Sphere(void);
	void update_Sphere(void);

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
							TEXT("Prathamesh Laxmikant Paropkari :       Three Moving Lights On Sphere"),
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
	iResult = initialize_Sphere();

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
					display_Sphere();

					
					if (bLightingEnable == TRUE && enablePerVertexLighting == TRUE || enablePerFragmentLighting == TRUE)
					{
						// Update
						update_Sphere();
					}
					
				

					
				}
			}
		}


	// Uninitialization
		uninitialize_Sphere();
	
	return((int)msg.wParam);
} //WinMain ends here


// Call Back Function
LRESULT CALLBACK WndProc(HWND hwnd,UINT imsg,WPARAM wParam,LPARAM lParam)
{
   // function declaration | FUNCTION PROTOTYPE
   void ToggleFullScreen(void);
   void resize_Sphere(int, int);

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
		resize_Sphere(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_ERASEBKGND:
		return(0);
	case WM_KEYDOWN:
	switch (LOWORD(wParam))
	{
	case 'Q':                      //VK_ESCAPE has 27 ascii value
	case'q':
		DestroyWindow(hwnd);
		break;
	}
	break;
 case WM_CHAR:
	switch (LOWORD(wParam))
	{
	case VK_ESCAPE:
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
	case 'F':
	case 'f':
		enablePerVertexLighting = FALSE;

		if (enablePerFragmentLighting == FALSE)
		{
			enablePerFragmentLighting = TRUE;
		}
		break;

	case 'V':
	case 'v':
		enablePerFragmentLighting = FALSE;

		if (enablePerVertexLighting == FALSE)
		{
			enablePerVertexLighting = TRUE;
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
int initialize_Sphere(void)
{
	
	//function declarations
	void printGLInfo(void);
	void uninitialize_Sphere(void);
	void resize_Sphere(int, int);
	
	

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
	const GLchar* vertexShaderSourceCodePerVertex =
		"#version 460 core\n"              //(version of GLSL * 100) core means you are using core profile i.e PP
		"\n"
		"in vec4 aPosition;\n"
		"in vec3 aNormal;\n"
		"uniform mat4 uModelMatrix;\n"
		"uniform mat4 uViewMatrix;\n"
		"uniform mat4 uProjectionMatrix;\n"
		"uniform vec3 ulightAmbient[3];\n"
		"uniform vec3 ulightDiffuse[3];\n"
		"uniform vec3 ulightSpecular[3];\n"
		"uniform vec4 ulightPosition[3];\n"
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
		"vec3 lightAmbient[3];\n"
		"vec3 lightDirection[3];\n"
		"vec3 lightDiffuse[3];\n"
		"vec3 reflectionVector[3];\n"
		"vec3 lightSpecular[3];\n"
		"for(int i = 0; i < 3; i++)\n"
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

	GLuint vertexShaderObjectPerVertex = glCreateShader(GL_VERTEX_SHADER);

	//step 3 : Give vertex shader code to OpenGL
	glShaderSource(vertexShaderObjectPerVertex, 1, (const GLchar**)&vertexShaderSourceCodePerVertex, NULL);

	//Step 4 : Let OpenGL engine Compile the vertex shader code
	glCompileShader(vertexShaderObjectPerVertex);
	
	GLint status = 0;
	GLint infoLogLength = 0;
	GLchar* szInfoLog = NULL;

	//step 5 : Check for vertex shader compilation
	//step 5 (a) :  get compile status in status variable
	glGetShaderiv(vertexShaderObjectPerVertex, GL_COMPILE_STATUS, &status);

	//step 5 (b) : if error occur i.e status == GL_FALSE,  get length of error message into infoLogLength variable
	if (status == GL_FALSE)
	{
		 
		glGetShaderiv(vertexShaderObjectPerVertex, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			//step 5 (c) : declare a string variable allocate memory equal to above length i.e length of error message
			szInfoLog = (GLchar*)malloc(infoLogLength);


			if (szInfoLog != NULL)
			{
				// step 5 (d) : get the compilation error log info into memory allocated string
				glGetShaderInfoLog(vertexShaderObjectPerVertex, infoLogLength, NULL, szInfoLog);

				//step 5 (e) : print error message into log file
				fprintf(gpFile, "Vertex shader per vertex compilation error log : %s\n", szInfoLog);


				//step 5 (f) : free allocated memory
				free(szInfoLog);
				szInfoLog = NULL;


				//step 5 (g) : uninitialize and exit.
				

			}

		}

		uninitialize_Sphere();
	}

	//follow all steps of vertex shader for fragment shader

	//step 6  : Write the source code for fragment shader   

	//fragment shader
	const GLchar* fragmentShaderSourceCodePerVertex =
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

	GLuint fragmentShaderObjectPerVertex = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShaderObjectPerVertex, 1, (const GLchar**)&fragmentShaderSourceCodePerVertex, NULL);

	glCompileShader(fragmentShaderObjectPerVertex);

	 status = 0;
	 infoLogLength = 0;
	 szInfoLog = NULL;

	 glGetShaderiv(fragmentShaderObjectPerVertex, GL_COMPILE_STATUS, &status);

	 if (status == GL_FALSE)
	 {
		
		 glGetShaderiv(fragmentShaderObjectPerVertex, GL_INFO_LOG_LENGTH, &infoLogLength);

		 if (infoLogLength > 0)
		 {
			 szInfoLog = (GLchar*)malloc(infoLogLength);


			 if (szInfoLog != NULL)
			 {
				 glGetShaderInfoLog(fragmentShaderObjectPerVertex, infoLogLength, NULL, szInfoLog);
				 fprintf(gpFile, "Fragment shader per vertex compilation error log : %s\n", szInfoLog);
				 free(szInfoLog);
				 szInfoLog = NULL;
				 

			 }

			 

		 }

		 uninitialize_Sphere();
	 }

	 //linking process for shader program.

	 // shader program

	 //step 7 : create shader program object

	 shaderProgramObjectPerVertex_Sphere = glCreateProgram();

	 //step 8 : Attach both shaders into this program            also know as prelinking binding  should be done before linking and after attachment
	 glAttachShader(shaderProgramObjectPerVertex_Sphere, vertexShaderObjectPerVertex);
	 glAttachShader(shaderProgramObjectPerVertex_Sphere, fragmentShaderObjectPerVertex);

	 //step 9: bind attribute location to the shader program object

	 glBindAttribLocation(shaderProgramObjectPerVertex_Sphere, AMC_ATTRIBUTE_POSITION, "aPosition");

	 glBindAttribLocation(shaderProgramObjectPerVertex_Sphere, AMC_ATTRIBUTE_COLOR, "aColor");

	 glBindAttribLocation(shaderProgramObjectPerVertex_Sphere, AMC_ATTRIBUTE_NORMAL , "aNormal");

	 //step 10: Link the shader program

	 glLinkProgram(shaderProgramObjectPerVertex_Sphere);

	 //step 11: do linking error check fpr shader program

	 status = 0;
	 infoLogLength = 0;
	 szInfoLog = NULL;

	 glGetProgramiv(shaderProgramObjectPerVertex_Sphere, GL_LINK_STATUS, &status);

	 if (status == GL_FALSE)
	 {
		 glGetProgramiv(shaderProgramObjectPerVertex_Sphere, GL_INFO_LOG_LENGTH, &infoLogLength);

		 if (infoLogLength > 0)
		 {
			 szInfoLog = (GLchar*)malloc(infoLogLength);


			 if (szInfoLog != NULL)
			 {
				 glGetProgramInfoLog(shaderProgramObjectPerVertex_Sphere, infoLogLength, NULL, szInfoLog);
				 fprintf(gpFile, "shader program first linking error log : %s\n", szInfoLog);
				 free(szInfoLog);
				 szInfoLog = NULL;
				

			 }



		 }

		 uninitialize_Sphere();
	 }


	 // Get shader uniform location

	 modelMatrixUniform_Sphere = glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "uModelMatrix");

	 viewMatrixUniform_Sphere = glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "uViewMatrix");

	 projectionMatrixUniform_Sphere = glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "uProjectionMatrix");

	 //light0

	 lightAmbientUniform_Sphere[0] = glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "ulightAmbient[0]");

	 lightDiffuseUniform_Sphere[0] = glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "ulightDiffuse[0]");

	 lightSpecularUniform_Sphere[0] = glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "ulightSpecular[0]");

	 lightPositionUniform_Sphere[0] = glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "ulightPosition[0]");

	 //light 1

	 lightAmbientUniform_Sphere[1] = glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "ulightAmbient[1]");

	 lightDiffuseUniform_Sphere[1] = glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "ulightDiffuse[1]");

	 lightSpecularUniform_Sphere[1] = glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "ulightSpecular[1]");

	 lightPositionUniform_Sphere[1] = glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "ulightPosition[1]");

	 //light 2

	 lightAmbientUniform_Sphere[2] = glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "ulightAmbient[2]");

	 lightDiffuseUniform_Sphere[2] = glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "ulightDiffuse[2]");

	 lightSpecularUniform_Sphere[2] = glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "ulightSpecular[2]");

	 lightPositionUniform_Sphere[2] = glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "ulightPosition[2]");

	 //material properties


	 materialAmbientUniform_Sphere = glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "uMaterialAmbient");

	 materialDiffuseUniform_Sphere = glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "uMaterialDiffuse");


	 materialSpecularUniform_Sphere = glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "uMaterialSpecular");

	 materialShininessUniform_Sphere = glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "uMaterialShininess");

	 keyPressUniform_Sphere = glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "uKeyPress");

// *************************************** Per fragment shader program object starts **********************

	 const GLchar* vertexShaderSourceCodePerFragment = 
	 
		 "#version 460 core\n"              // (version of GLSL * 100) core means you are using core profile i.e PP
		 "\n"
		 "in vec4 aPosition;\n"
		 "in vec3 aNormal;\n"
		 "uniform mat4 uModelMatrix;\n"
		 "uniform mat4 uViewMatrix;\n"
		 "uniform mat4 uProjectionMatrix;\n"
		 "uniform vec4 ulightPosition[3];\n"
		 "uniform int uKeyPress;\n"
		 "out vec3 otransformedNormals;\n"
		 "out vec3 oviewerVector;\n"
		 "out vec3 olightDirection[3];\n"
		 "void main(void)\n"
		 "{\n"
		 "if(uKeyPress == 1)\n"
		 "{\n"
		 "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;\n"
		 "otransformedNormals = (mat3(uViewMatrix * uModelMatrix) * aNormal);\n"
		 "for(int i = 0; i < 3; i++)\n"
		 "{\n"
		 "olightDirection[i] = (vec3(ulightPosition[i] - eyeCoordinates));\n"
		 "oviewerVector = (-eyeCoordinates.xyz);\n"
		 "}\n"
		 "}\n"
		 "else\n"
		 "{\n"
		 //"oPhongADSLight = vec3( 0.0, 0.0, 0.0);\n
			/* "otransformedNormals = vec3(0.0, 0.0, 0.0);\n"
			 "for(int i = 0; i < 2; i++)\n"
			 "{\n"
				 "olightDirection[i] = vec3 (0.0, 0.0, 0.0);\n"

			 "}\n"
			  "oviewerVector = vec3(0.0, 0.0, 0.0);\n"*/
		 "}\n"
		 "gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;\n"
		 "}";

	 //step 2: Create vertex shader object

	 GLuint vertexShaderObjectPerFragment = glCreateShader(GL_VERTEX_SHADER);

	 //step 3 : Give vertex shader code to OpenGL
	 glShaderSource(vertexShaderObjectPerFragment, 1, (const GLchar**)&vertexShaderSourceCodePerFragment, NULL);

	 //Step 4 : Let OpenGL engine Compile the vertex shader code
	 glCompileShader(vertexShaderObjectPerFragment);

	  status = 0;
	  infoLogLength = 0;
	  szInfoLog = NULL;

	 //step 5 : Check for vertex shader compilation
	 //step 5 (a) :  get compile status in status variable
	 glGetShaderiv(vertexShaderObjectPerFragment, GL_COMPILE_STATUS, &status);

	 //step 5 (b) : if error occur i.e status == GL_FALSE,  get length of error message into infoLogLength variable
	 if (status == GL_FALSE)
	 {

		 glGetShaderiv(vertexShaderObjectPerFragment, GL_INFO_LOG_LENGTH, &infoLogLength);

		 if (infoLogLength > 0)
		 {
			 //step 5 (c) : declare a string variable allocate memory equal to above length i.e length of error message
			 szInfoLog = (GLchar*)malloc(infoLogLength);


			 if (szInfoLog != NULL)
			 {
				 // step 5 (d) : get the compilation error log info into memory allocated string
				 glGetShaderInfoLog(vertexShaderObjectPerFragment, infoLogLength, NULL, szInfoLog);

				 //step 5 (e) : print error message into log file
				 fprintf(gpFile, "Vertex shader Per Fragment compilation error log : %s\n", szInfoLog);


				 //step 5 (f) : free allocated memory
				 free(szInfoLog);
				 szInfoLog = NULL;


				 //step 5 (g) : uninitialize and exit.


			 }

		 }

		 uninitialize_Sphere();
	 }


	 const GLchar* FragmentShaderSourceCodePerFragment = 

		 "#version 460 core\n"
		 "\n"
		 "in vec3 otransformedNormals;\n"
		 "in vec3 olightDirection[3];\n"
		 "in vec3 oviewerVector;\n"
		 "uniform mat4 uModelMatrix;\n"
		 "uniform mat4 uViewMatrix;\n"
		 "uniform mat4 uProjectionMatrix;\n"
		 "uniform vec3 ulightAmbient[3];\n"
		 "uniform vec3 ulightDiffuse[3];\n"
		 "uniform vec3 ulightSpecular[3];\n"
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
		 "vec3 normalizedLightDirection[3];\n"
		 "vec3 normalizedTransformedNormals = normalize(otransformedNormals);\n"
		 "vec3 normalizedViwerVector = normalize(oviewerVector);\n"
		 "vec3 lightAmbient[3];\n"
		 "vec3 lightDiffuse[3];\n"
		 "vec3 reflectionVector[3];\n"
		 "vec3 lightSpecular[3];\n"
		 "for(int i = 0; i < 3; i++)\n"
		 "{\n"
		 "lightAmbient[i] = ulightAmbient[i] * uMaterialAmbient;\n"
		 "\n"
		 "normalizedLightDirection[i] = normalize(olightDirection[i]);\n"
		 "\n"
		 "lightDiffuse[i] = ulightDiffuse[i] * uMaterialDiffuse * max(dot(normalizedLightDirection[i], normalizedTransformedNormals),0.0);\n"
		 "\n"
		 "reflectionVector[i] = reflect(-normalizedLightDirection[i], normalizedTransformedNormals);\n"
		 "\n"
		 "lightSpecular[i] = ulightSpecular[i] * uMaterialSpecular * pow(max(dot(reflectionVector[i], normalizedViwerVector), 0.0),uMaterialShininess);\n"
		 "\n"
		 "PhongADSLight = PhongADSLight + lightAmbient[i] + lightDiffuse[i] + lightSpecular[i];\n"
		 "}\n"
		 "FragColor = vec4(PhongADSLight, 1.0);\n"
		 "}\n"
		 "else\n"
		 "{\n"
		 "FragColor = vec4(1.0, 1.0, 1.0,1.0);\n"
		 "}\n"

		 "}";

	 GLuint fragmentShaderObjectPerFragment = glCreateShader(GL_FRAGMENT_SHADER);

	 glShaderSource(fragmentShaderObjectPerFragment, 1, (const GLchar**)&FragmentShaderSourceCodePerFragment, NULL);

	 glCompileShader(fragmentShaderObjectPerFragment);

	 status = 0;
	 infoLogLength = 0;
	 szInfoLog = NULL;

	 glGetShaderiv(fragmentShaderObjectPerFragment, GL_COMPILE_STATUS, &status);

	 if (status == GL_FALSE)
	 {

		 glGetShaderiv(fragmentShaderObjectPerFragment, GL_INFO_LOG_LENGTH, &infoLogLength);

		 if (infoLogLength > 0)
		 {
			 szInfoLog = (GLchar*)malloc(infoLogLength);


			 if (szInfoLog != NULL)
			 {
				 glGetShaderInfoLog(fragmentShaderObjectPerFragment, infoLogLength, NULL, szInfoLog);
				 fprintf(gpFile, "Fragment shader per Fragment compilation error log : %s\n", szInfoLog);
				 free(szInfoLog);
				 szInfoLog = NULL;


			 }



		 }

		 uninitialize_Sphere();
	 }

	 //linking process for shader program.

	 // shader program

	 //step 7 : create shader program object

	 shaderProgramObjectPerFragment_Sphere = glCreateProgram();

	 //step 8 : Attach both shaders into this program            also know as prelinking binding  should be done before linking and after attachment
	 glAttachShader(shaderProgramObjectPerFragment_Sphere, vertexShaderObjectPerFragment);
	 glAttachShader(shaderProgramObjectPerFragment_Sphere, fragmentShaderObjectPerFragment);

	 //step 9: bind attribute location to the shader program object

	 glBindAttribLocation(shaderProgramObjectPerFragment_Sphere, AMC_ATTRIBUTE_POSITION, "aPosition");

	 glBindAttribLocation(shaderProgramObjectPerFragment_Sphere, AMC_ATTRIBUTE_COLOR, "aColor");

	 glBindAttribLocation(shaderProgramObjectPerFragment_Sphere, AMC_ATTRIBUTE_NORMAL, "aNormal");

	 //step 10: Link the shader program

	 glLinkProgram(shaderProgramObjectPerFragment_Sphere);

	 //step 11: do linking error check fpr shader program

	 status = 0;
	 infoLogLength = 0;
	 szInfoLog = NULL;

	 glGetProgramiv(shaderProgramObjectPerFragment_Sphere, GL_LINK_STATUS, &status);

	 if (status == GL_FALSE)
	 {
		 glGetProgramiv(shaderProgramObjectPerFragment_Sphere, GL_INFO_LOG_LENGTH, &infoLogLength);

		 if (infoLogLength > 0)
		 {
			 szInfoLog = (GLchar*)malloc(infoLogLength);


			 if (szInfoLog != NULL)
			 {
				 glGetProgramInfoLog(shaderProgramObjectPerFragment_Sphere, infoLogLength, NULL, szInfoLog);
				 fprintf(gpFile, "shader program first linking error log : %s\n", szInfoLog);
				 free(szInfoLog);
				 szInfoLog = NULL;


			 }



		 }

		 uninitialize_Sphere();
	 }

	 modelMatrixUniform_Sphere = glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "uModelMatrix");

	 viewMatrixUniform_Sphere = glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "uViewMatrix");

	 projectionMatrixUniform_Sphere = glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "uProjectionMatrix");

	 //light 0

	 lightAmbientUniform_Sphere[0] = glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "ulightAmbient[0]");

	 lightDiffuseUniform_Sphere[0] = glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "ulightDiffuse[0]");

	 lightSpecularUniform_Sphere[0] = glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "ulightSpecular[0]");

	 lightPositionUniform_Sphere[0] = glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "ulightPosition[0]");

	 //light 1

	 lightAmbientUniform_Sphere[1] = glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "ulightAmbient[1]");

	 lightDiffuseUniform_Sphere[1] = glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "ulightDiffuse[1]");

	 lightSpecularUniform_Sphere[1] = glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "ulightSpecular[1]");

	 lightPositionUniform_Sphere[1] = glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "ulightPosition[1]");

	 //light 2

	 lightAmbientUniform_Sphere[2] = glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "ulightAmbient[2]");

	 lightDiffuseUniform_Sphere[2] = glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "ulightDiffuse[2]");

	 lightSpecularUniform_Sphere[2] = glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "ulightSpecular[2]");

	 lightPositionUniform_Sphere[2] = glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "ulightPosition[2]");


	 materialAmbientUniform_Sphere = glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "uMaterialAmbient");

	 materialDiffuseUniform_Sphere = glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "uMaterialDiffuse");


	 materialSpecularUniform_Sphere = glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "uMaterialSpecular");

	 materialShininessUniform_Sphere = glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "uMaterialShininess");

	 keyPressUniform_Sphere = glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "uKeyPress");


	 float sphere_Positions[1146];
	 float sphere_normals[1146];
	 float sphere_Texcoords[764];
	 unsigned short sphere_elements[2280];


	 getSphereVertexData(sphere_Positions, sphere_normals, sphere_Texcoords, sphere_elements);

	 numSphereElements = getNumberOfSphereElements();

	
	

	 ////step 13: Create vertex array object
	 //VAO

    //step 13: Create vertex array object
	 //VAO

	 // ********************************************** for Sphere   ***********************************************

	 glGenVertexArrays(1, &vaoSphere_Sphere);

	 //step 14: Bind with VAO

	 glBindVertexArray(vaoSphere_Sphere);

	 //step 15: Create vertex buffer object (VBO) for position

	 //VBO for position

	 glGenBuffers(1, &vbo_Position_Sphere);

	 //step 16: Bind with VBO of position

	 glBindBuffer(GL_ARRAY_BUFFER, vbo_Position_Sphere);

	 glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_Positions), sphere_Positions, GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	 glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);



	 //VBO for mormal

	 glGenBuffers(1, &vbo_Normal_Sphere);

	 glBindBuffer(GL_ARRAY_BUFFER, vbo_Normal_Sphere);

	 glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	 glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);

	 //VBO for elements

	 glGenBuffers(1, &vbo_Element_Sphere);
	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_Element_Sphere);
	 glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);

	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



	 //step 18 : unbind with VAO
	 glBindVertexArray(0);



	// 2nd change for depth or Enabling depth
	
	glClearDepth(1.0f); // Compulsory
	glEnable(GL_DEPTH_TEST); // 8 test paike ek(depth) test enable kr
	glDepthFunc(GL_LEQUAL); 
	


	// Set The ClearColor Of Window 
	// Clear OpenGl Stars
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 


	//vertex shader light properties

	
	//ambient property

	light[0].ambient = vec3(0.0, 0.0, 0.0);
	light[1].ambient = vec3(0.0, 0.0, 0.0);
	light[2].ambient = vec3(0.0, 0.0, 0.0);

	//diffuse property
	light[0].diffuse = vec3(0.2, 1.0, 1.5);  //rotate along X axis
	light[1].diffuse = vec3(0.7, 0.7, 0.8);   // rotate along Y axis
	light[2].diffuse = vec3(1.0, 0.5, 0.0); //rotate along Z axis

	//specular

	light[0].specular = vec3(0.2, 1.0, 1.5);
	light[1].specular = vec3(0.7, 0.7, 0.8);
	light[2].specular = vec3(1.0, 0.5, 0.0);

	//position property

	light[0].position = vec4(-1.0, 0.0, 0.0, 1.0);
	light[1].position = vec4(1.0, 0.0, 0.0, 1.0);
	light[1].position = vec4(0.0, 0.0, 0.0, 1.0);

	//fragment shader light properties

	//ambient property

	fLights[0].ambient = vec3(0.0, 0.0, 0.0);
	fLights[1].ambient = vec3(0.0, 0.0, 0.0);
	fLights[2].ambient = vec3(0.0, 0.0, 0.0);

	//diffuse property

	fLights[0].diffuse = vec3(0.0, 1.0, 1.0);
	fLights[1].diffuse = vec3(1.0, 0.0, 0.0);
	fLights[2].diffuse = vec3(1.0, 1.0, 0.0);

	//specular property

	fLights[0].specular = vec3(0.0, 1.0, 1.0);
	fLights[1].specular = vec3(1.0, 0.0, 0.0);
	fLights[2].specular = vec3(1.0, 1.0, 0.0);

	//position property

	fLights[0].position = vec4(-1.0, 0.0, 0.0, 1.0);
	fLights[1].position = vec4(1.0, 0.0, 0.0, 1.0);
	fLights[2].position = vec4(0.0, 0.0, 0.0, 1.0);

	


	perspectiveProjectionMatrix_Sphere = vmath::mat4::identity();

	resize_Sphere(WIN_WIDTH, WIN_HEIGHT);

	

	//code

	return(0);
}

void resize_Sphere(int width, int height)
{
	// code
	if(height <= 0)
		height = 1;

	
	//glMatrixMode(GL_PROJECTION); //not allowed in PP 
	//glLoadIdentity();            not allowed in PP 

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	//set Perspective projection matrix

	perspectiveProjectionMatrix_Sphere = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	//gluPerspective(45.0f,((GLfloat)width / (GLfloat)height),0.1f,100.0f);  //not allowed in PP 


}

void display_Sphere(GLint textureWidth, GLint textureHeight)
{
	

	

	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	//PP steps for display function

	//step 1: Use shader program object

	glUseProgram(shaderProgramObjectPerVertex_Sphere);

	if (enablePerFragmentLighting == TRUE)
	{
		glUseProgram(shaderProgramObjectPerFragment_Sphere);
	}
	else
	{
		glUseProgram(shaderProgramObjectPerVertex_Sphere);
	}

	
	

	


	//Pyramid


     //transformations

	mat4 modelViewMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 translationMatrix = mat4::identity();
	

	// Set up translation matrix
	translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

	// Combine translation and rotation
	modelViewMatrix = translationMatrix;

	///push above mvp into vertex shader's mvp uniform

	// Pass modelViewMatrix to the vertex shader's uniform
	glUniformMatrix4fv(modelMatrixUniform_Sphere, 1, GL_FALSE, modelViewMatrix);

	// Pass viewMatrix to the vertex shader's uniform
	glUniformMatrix4fv(viewMatrixUniform_Sphere, 1, GL_FALSE, viewMatrix);

	// Pass perspectiveProjectionMatrix to the vertex shader's uniform
	glUniformMatrix4fv(projectionMatrixUniform_Sphere, 1, GL_FALSE, perspectiveProjectionMatrix_Sphere);

	
	if (bLightingEnable == TRUE)
	{
		if (enablePerVertexLighting == FALSE && enablePerFragmentLighting == FALSE)
		{
			//enablePerVertexLighting = TRUE;

			glUniform1i(keyPressUniform_Sphere, 1);

			//light 0 uniforms

			glUniform3fv(lightAmbientUniform_Sphere[0], 1, light[0].ambient);

			glUniform3fv(lightDiffuseUniform_Sphere[0], 1, light[0].diffuse);

			glUniform3fv(lightSpecularUniform_Sphere[0], 1, light[0].specular);

			glUniform4fv(lightPositionUniform_Sphere[0], 1, light[0].position);

			//light 1 uniforms

			glUniform3fv(lightAmbientUniform_Sphere[1], 1, light[1].ambient);

			glUniform3fv(lightDiffuseUniform_Sphere[1], 1, light[1].diffuse);

			glUniform3fv(lightSpecularUniform_Sphere[1], 1, light[1].specular);

			glUniform4fv(lightPositionUniform_Sphere[1], 1, light[1].position);

			//light 2 uniforms

			

			glUniform3fv(lightAmbientUniform_Sphere[2], 1, light[2].ambient);

			glUniform3fv(lightDiffuseUniform_Sphere[2], 1, light[2].diffuse);

			glUniform3fv(lightSpecularUniform_Sphere[2], 1, light[2].specular);

			glUniform4fv(lightPositionUniform_Sphere[2], 1, light[2].position);


			glUniform3fv(materialAmbientUniform_Sphere, 1, materialAmbient);

			glUniform3fv(materialDiffuseUniform_Sphere, 1, materialDiffuse);

			glUniform3fv(materialSpecularUniform_Sphere, 1, materialSpecular);

			glUniform1f(materialShininessUniform_Sphere, materialShininess);

		}

		else if (enablePerVertexLighting == TRUE)
		{
			glUniform1i(keyPressUniform_Sphere, 1);

			glUniform3fv(lightAmbientUniform_Sphere[0], 1, light[0].ambient);

			glUniform3fv(lightDiffuseUniform_Sphere[0], 1, light[0].diffuse);

			glUniform3fv(lightSpecularUniform_Sphere[0], 1, light[0].specular);

			glUniform4fv(lightPositionUniform_Sphere[0], 1, light[0].position);

			glUniform3fv(lightAmbientUniform_Sphere[1], 1, light[1].ambient);

			glUniform3fv(lightDiffuseUniform_Sphere[1], 1, light[1].diffuse);

			glUniform3fv(lightSpecularUniform_Sphere[1], 1, light[1].specular);

			glUniform4fv(lightPositionUniform_Sphere[1], 1, light[1].position);

			//light 2 uniforms



			glUniform3fv(lightAmbientUniform_Sphere[2], 1, light[2].ambient);

			glUniform3fv(lightDiffuseUniform_Sphere[2], 1, light[2].diffuse);

			glUniform3fv(lightSpecularUniform_Sphere[2], 1, light[2].specular);

			glUniform4fv(lightPositionUniform_Sphere[2], 1, light[2].position);

			glUniform3fv(materialAmbientUniform_Sphere, 1, materialAmbient);

			glUniform3fv(materialDiffuseUniform_Sphere, 1, materialDiffuse);

			glUniform3fv(materialSpecularUniform_Sphere, 1, materialSpecular);

			glUniform1f(materialShininessUniform_Sphere, materialShininess);

		}

		else if(enablePerFragmentLighting == TRUE)
		{
			glUniform1i(keyPressUniform_Sphere, 1);

			//light 0 uniforms

			glUniform3fv(lightAmbientUniform_Sphere[0], 1, fLights[0].ambient);

			glUniform3fv(lightDiffuseUniform_Sphere[0], 1, fLights[0].diffuse);

			glUniform3fv(lightSpecularUniform_Sphere[0], 1, fLights[0].specular);

			glUniform4fv(lightPositionUniform_Sphere[0], 1, fLights[0].position);

			//light 1 uniforms

			glUniform3fv(lightAmbientUniform_Sphere[1], 1, fLights[1].ambient);

			glUniform3fv(lightDiffuseUniform_Sphere[1], 1, fLights[1].diffuse);

			glUniform3fv(lightSpecularUniform_Sphere[1], 1, fLights[1].specular);

			glUniform4fv(lightPositionUniform_Sphere[1], 1, fLights[1].position);

			//light 2 uniforms

			glUniform3fv(lightAmbientUniform_Sphere[2], 1, fLights[2].ambient);

			glUniform3fv(lightDiffuseUniform_Sphere[2], 1, fLights[2].diffuse);

			glUniform3fv(lightSpecularUniform_Sphere[2], 1, fLights[2].specular);

			glUniform4fv(lightPositionUniform_Sphere[2], 1, fLights[2].position);


			glUniform3fv(materialAmbientUniform_Sphere, 1, materialAmbient);

			glUniform3fv(materialDiffuseUniform_Sphere, 1, materialDiffuse);

			glUniform3fv(materialSpecularUniform_Sphere, 1, materialSpecular);

			glUniform1f(materialShininessUniform_Sphere, materialShininess);
		}

		
	}

	else
	{
		glUniform1i(keyPressUniform_Sphere, 0);
	}

	

	
	//step 2: Bind with (VAO)

	glBindVertexArray(vaoSphere_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_Element_Sphere);

	//step 3: Draw the geometry

	glDrawElements(GL_TRIANGLES, numSphereElements, GL_UNSIGNED_SHORT, 0);

	//step 4: unBind with (VAO)

	//step 5: Unuse shader program object
	glUseProgram(0);
	
	SwapBuffers(ghdc);

}

void update_Sphere(void)
{
	//code
	//only at time of change animation here code will come

	if (bLightingEnable == TRUE)
	{
		if (enablePerVertexLighting == TRUE)
		{
			if (vLight0 < (2 * M_PI))
			{


				//light 0    //x rotation
				light[0].position[0] = 0.0f;
				light[0].position[1] = 2.0f * (GLfloat)sin(vLight0);
				light[0].position[2] = (-2.0f) + (2.0f * (GLfloat)cos(vLight0));
				light[0].position[3] = 1.0f;

				fprintf(gpFile, "value of position[1] = %f", light[0].position[1]);

				//light 1   //y rotation

				light[1].position[0] = 2.0f * (GLfloat)sin(vLight1);
				light[1].position[1] = 0.0f;
				light[1].position[2] = (-2.0f) + (2.0f * (GLfloat)cos(vLight1));
				light[1].position[3] = 1.0f;

				//light 2   //Z rotation

				light[2].position[0] = 2.0f * (GLfloat)sin(vLight2);
				light[2].position[1] = 2.0f * (GLfloat)cos(vLight2);
				light[2].position[2] = 0.0f;
				light[2].position[3] = 1.0f;

				vLight0 = vLight0 + 0.004f;
				vLight1 = vLight1 + 0.004f;
				vLight2 = vLight2 + 0.004f;
			}

			else
			{
				vLight0 = 0.0f;
				vLight1 = 0.0f;
				vLight2 = 0.0f;
			}
		}

		if (enablePerFragmentLighting == TRUE)
		{
			if (fLight0 < (2*M_PI))
			{
				//light 0    //x rotation
				fLights[0].position[0] = 0.0f;
				fLights[0].position[1] = 2.0f * (GLfloat)sin(fLight0);
				fLights[0].position[2] = 2.0f * (GLfloat)cos(fLight0);
				fLights[0].position[3] = 1.0f;

				fprintf(gpFile, "value of position[1] = %f", light[0].position[1]);

				//light 1   //y rotation

				fLights[1].position[0] = 2.0f * (GLfloat)sin(fLight1);
				fLights[1].position[1] = 0.0f;
				fLights[1].position[2] = 2.0f * (GLfloat)cos(fLight1);
				fLights[1].position[3] = 1.0f;

				//light 2   //Z rotation

				fLights[2].position[0] = 2.0f * (GLfloat)sin(fLight2);
				fLights[2].position[1] = 2.0f * (GLfloat)cos(fLight2);
				fLights[2].position[2] = 0.0f;
				fLights[2].position[3] = 1.0f;

				fLight0 = fLight0 + 0.004f;
				fLight1 = fLight1 + 0.004f;
				fLight2 = fLight2 + 0.004f;
			}

			else
			{
				fLight0 = 0.0f;
				fLight1 = 0.0f;
				fLight2 = 0.0f;
			}


		}
			
		
	}
	

	
}

void uninitialize_Sphere(void)
{
   // Function Declarations
	void ToggleFullScreen(void);

   //code
   
	//steps for uninitialize function in PP

	if (shaderProgramObjectPerVertex_Sphere)
	{
		//step 1 : use shader program object

		glUseProgram(shaderProgramObjectPerVertex_Sphere);

		GLint numShaders = 0;

		//step 2 : get number of attached shaders
		glGetProgramiv(shaderProgramObjectPerVertex_Sphere, GL_ATTACHED_SHADERS, &numShaders);



		if (numShaders > 0)
		{
			//step 3 : Create and allocate memory of shader objects according to number of attached shaders
			GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint));

			if (pShaders != NULL)
			{
				//step 4 : Get shader object into the allocated array.

				glGetAttachedShaders(shaderProgramObjectPerVertex_Sphere, numShaders, NULL, pShaders);

				//step 5 : Detach and delete shader objects iteratively

				for (GLint i = 0; i < numShaders; i++)
				{
					glDetachShader(shaderProgramObjectPerVertex_Sphere, pShaders[i]);
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

		glDeleteProgram(shaderProgramObjectPerVertex_Sphere);
		shaderProgramObjectPerVertex_Sphere = 0;

		
		



	}

	if (shaderProgramObjectPerFragment_Sphere)
	{
		//step 1 : use shader program object

		glUseProgram(shaderProgramObjectPerFragment_Sphere);

		GLint numShaders = 0;

		//step 2 : get number of attached shaders
		glGetProgramiv(shaderProgramObjectPerFragment_Sphere, GL_ATTACHED_SHADERS, &numShaders);



		if (numShaders > 0)
		{
			//step 3 : Create and allocate memory of shader objects according to number of attached shaders
			GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint));

			if (pShaders != NULL)
			{
				//step 4 : Get shader object into the allocated array.

				glGetAttachedShaders(shaderProgramObjectPerFragment_Sphere, numShaders, NULL, pShaders);

				//step 5 : Detach and delete shader objects iteratively

				for (GLint i = 0; i < numShaders; i++)
				{
					glDetachShader(shaderProgramObjectPerFragment_Sphere, pShaders[i]);
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

		glDeleteProgram(shaderProgramObjectPerFragment_Sphere);
		shaderProgramObjectPerFragment_Sphere = 0;






	}

	//Sphere

	
	if (vbo_Element_Sphere)
	{
		glDeleteBuffers(1, &vbo_Element_Sphere);
		vbo_Element_Sphere = 0;
	}


	if (vbo_Normal_Sphere)
	{
		glDeleteBuffers(1, &vbo_Normal_Sphere);
		vbo_Normal_Sphere = 0;
	}


	if (vbo_Position_Sphere)
	{
		glDeleteBuffers(1, &vbo_Position_Sphere);
		vbo_Position_Sphere = 0;
	}



	//step 11 : delete VAO 

	if (vaoSphere_Sphere)
	{
		glDeleteVertexArrays(1, &vaoSphere_Sphere);
		vaoSphere_Sphere = 0;
	}


	



	//step 11 : delete VAO 

	

	



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




