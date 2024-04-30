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

//GLfloat AnglePyramid = 0.0f;

GLfloat AngleCube = 0.0f;





enum
{
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_COLOR,
	AMC_ATTRIBUTE_NORMAL

};

//GLuint vaoPyramid = 0;
GLuint vaoCube = 0;

//GLuint vbo_Position_Pyramid = 0;
//GLuint vbo_Color_Pyramid = 0;

GLuint vbo_Position_Cube = 0;
GLuint vbo_Normal_Cube = 0;
GLuint vbo_Color_Cube = 0;


//GLuint uMVPMatrix;

mat4 perspectiveProjectionMatrix;   //using 4*4 matrix.
                                   //mat4 - represents 4*4 matrix.   
								   //mat4 is not c++ datattype, it is defined in vmath.h.

GLuint modelViewMatrixUniform = 0;
GLuint projectionMatrixUniform = 0;

GLuint ldUniform = 0;
GLuint kdUniform = 0;
GLuint lightPositionUniform = 0;
GLuint keyPressUniform = 0;

BOOL bLightingEnabled = FALSE;
BOOL bAnimationEnabled = FALSE;

//GLfloat lightAmbient[] = { 0.5f,0.5f,0.5f,1.0f };
GLfloat lightDiffuse[] = { 1.0f,0.5f,0.0f,1.0f };
GLfloat lightPosition[] = { 0.0f,0.0f,2.0f,1.0f };
GLfloat materialDiffuse[] = { 1.0f,0.0f,1.0f,1.0f };


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
							TEXT("Prathamesh Laxmikant Paropkari  :      diffuse light live code  (3D Cube) : 30 March 2024 "),
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

					if (bAnimationEnabled == TRUE)
					{
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
		if (bAnimationEnabled == FALSE)
		{
			bAnimationEnabled = TRUE;
		}

		else
		{
			bAnimationEnabled = FALSE;
		}

		break;

	case 'L':
	case 'l':
		if (bLightingEnabled == FALSE)
		{
			bLightingEnabled = TRUE;
		}

		else
		{
			bLightingEnabled = FALSE;
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
	//variable declaration
	
	

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
		"#version 460 core\n"             //(version of GLSL * 100) core means you are using core profile i.e PP
		"\n"
		"in vec4 aPosition;\n"
		"in vec3 aNormal;\n"
		"uniform mat4 umodelViewMatrix;\n"
		"uniform mat4 uProjectionMatrix;\n"
		"uniform vec3 uLd;\n"
		"uniform vec3 uKd;\n"
		"uniform vec4 uLightPosition;\n"
		"uniform int uKeyPress;\n"
		"out vec3 oDiffuseLight;\n"
		
		"void main(void)\n"
		"{\n"
		"  if(uKeyPress == 1) \n"
		"  {\n"
		"   vec4 eyePosition = umodelViewMatrix * aPosition;\n"   //bring object coordinates to eye space.
		"   mat3 normalMatrix = mat3(transpose(inverse(umodelViewMatrix)));\n"  //normalize and do necessary math with model view matrix
		"   vec3 n = normalize(normalMatrix * aNormal);\n"   //now fetch the normals from the array and normalize them and bring them to eye space as well       //transformed normal
		"   vec3 s = normalize(vec3(uLightPosition - eyePosition));\n"   //calculate light source direction vector from each vertex                  //light source
		"   oDiffuseLight = uLd * uKd * dot(s,n);\n"   //calculte intensity of diffuse light for each point
		"  }\n"
		"  else                                  \n"
		"  {\n"
		"    oDiffuseLight = vec3(0.0, 0.0, 0.0);\n"
		"  }\n"
		"       \n"
		"  gl_Position = uProjectionMatrix * umodelViewMatrix * aPosition;"
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
		"in vec3 oDiffuseLight;\n"
		"uniform int uKeyPress;\n"
		"out vec4 FragColor;\n"
		"void main(void)\n"
		"{\n"
		"   if( uKeyPress == 1)\n"
		"   {\n"
		"     FragColor = vec4(oDiffuseLight,1.0);  \n"
		"   }\n"
		"   else\n"
		"   {\n"
		"     FragColor = vec4(1.0, 1.0, 1.0, 1.0);  \n"
		"   }\n"
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



	// mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "uMVPMatrix");  //must be done after linkage of shader program
	 //step 12: Declare position and color arrays 

	 modelViewMatrixUniform = glGetUniformLocation(shaderProgramObject, "umodelViewMatrix");

	 projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "uProjectionMatrix");

	 ldUniform = glGetUniformLocation(shaderProgramObject, "uLd");

	 kdUniform = glGetUniformLocation(shaderProgramObject, "uKd");

	 lightPositionUniform = glGetUniformLocation(shaderProgramObject, "uLightPosition");

	 keyPressUniform = glGetUniformLocation(shaderProgramObject, "uKeyPress");


	


	 const GLfloat cubePosition[] =
	 {
		 // front
	 1.0f,  1.0f,  1.0f, // top-right of front
	-1.0f,  1.0f,  1.0f, // top-left of front
	-1.0f, -1.0f,  1.0f, // bottom-left of front
	 1.0f, -1.0f,  1.0f, // bottom-right of front

	 // right
	  1.0f,  1.0f, -1.0f, // top-right of right
	  1.0f,  1.0f,  1.0f, // top-left of right
	  1.0f, -1.0f,  1.0f, // bottom-left of right
	  1.0f, -1.0f, -1.0f, // bottom-right of right

	  // back
	   1.0f,  1.0f, -1.0f, // top-right of back
	  -1.0f,  1.0f, -1.0f, // top-left of back
	  -1.0f, -1.0f, -1.0f, // bottom-left of back
	   1.0f, -1.0f, -1.0f, // bottom-right of back

	   // left
	   -1.0f,  1.0f,  1.0f, // top-right of left
	   -1.0f,  1.0f, -1.0f, // top-left of left
	   -1.0f, -1.0f, -1.0f, // bottom-left of left
	   -1.0f, -1.0f,  1.0f, // bottom-right of left

	   // top
		1.0f,  1.0f, -1.0f, // top-right of top
	   -1.0f,  1.0f, -1.0f, // top-left of top
	   -1.0f,  1.0f,  1.0f, // bottom-left of top
		1.0f,  1.0f,  1.0f, // bottom-right of top

		// bottom
		 1.0f, -1.0f,  1.0f, // top-right of bottom
		-1.0f, -1.0f,  1.0f, // top-left of bottom
		-1.0f, -1.0f, -1.0f, // bottom-left of bottom
		 1.0f, -1.0f, -1.0f, // bottom-right of bottom
	 };

	

	 GLfloat cubeNormals[] =
	 {
		 // front surface
		 0.0f,  0.0f,  1.0f, // top-right of front
		 0.0f,  0.0f,  1.0f, // top-left of front
		 0.0f,  0.0f,  1.0f, // bottom-left of front
		 0.0f,  0.0f,  1.0f, // bottom-right of front

		 // right surface
		 1.0f,  0.0f,  0.0f, // top-right of right
		 1.0f,  0.0f,  0.0f, // top-left of right
		 1.0f,  0.0f,  0.0f, // bottom-left of right
		 1.0f,  0.0f,  0.0f, // bottom-right of right

		 // back surface
		 0.0f,  0.0f, -1.0f, // top-right of back
		 0.0f,  0.0f, -1.0f, // top-left of back
		 0.0f,  0.0f, -1.0f, // bottom-left of back
		 0.0f,  0.0f, -1.0f, // bottom-right of back

		 // left surface
		-1.0f,  0.0f,  0.0f, // top-right of left
		-1.0f,  0.0f,  0.0f, // top-left of left
		-1.0f,  0.0f,  0.0f, // bottom-left of left
		-1.0f,  0.0f,  0.0f, // bottom-right of left

		// top surface
		0.0f,  1.0f,  0.0f, // top-right of top
		0.0f,  1.0f,  0.0f, // top-left of top
		0.0f,  1.0f,  0.0f, // bottom-left of top
		0.0f,  1.0f,  0.0f, // bottom-right of top

		// bottom surface
		0.0f, -1.0f,  0.0f, // top-right of bottom
		0.0f, -1.0f,  0.0f, // top-left of bottom
		0.0f, -1.0f,  0.0f, // bottom-left of bottom
		0.0f, -1.0f,  0.0f, // bottom-right of bottom
	 };


	 ////step 13: Create vertex array object
	 //VAO

	

	 // ********************************************** for cube ***********************************************

	 

	 glGenVertexArrays(1, &vaoCube);

	 //step 14: Bind with VAO

	 glBindVertexArray(vaoCube);

	 //step 15: Create vertex buffer object (VBO) for position

	 //VBO for position

	 glGenBuffers(1, &vbo_Position_Cube);

	 //step 16: Bind with VBO of position

	 glBindBuffer(GL_ARRAY_BUFFER, vbo_Position_Cube);

	 glBufferData(GL_ARRAY_BUFFER, sizeof(cubePosition), cubePosition, GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	 glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);


	
	 //VBO for mormal

	 glGenBuffers(1, &vbo_Normal_Cube);

	 glBindBuffer(GL_ARRAY_BUFFER, vbo_Normal_Cube);

	 glBufferData(GL_ARRAY_BUFFER, sizeof(cubeNormals), cubeNormals, GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	 glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);

	 //step 18 : unbind with VAO
	 glBindVertexArray(0);



	 


	// 2nd change for depth or Enabling depth
	
	glClearDepth(1.0f); // Compulsory
	glEnable(GL_DEPTH_TEST); // 8 test paike ek(depth) test enable kr
	glDepthFunc(GL_LEQUAL); 
	


	// Set The ClearColor Of Window To Blue
	// Clear OpenGl Stars
	glClearColor(0.0f,0.0f, 0.0f, 1.0f); //Black Color

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

	

	//Cube
	
	//transformations

	 mat4 modelViewMatrix = mat4::identity();

	 mat4 translationMatrix = mat4::identity();

	 translationMatrix = vmath::translate(0.0f, 0.0f, -4.5f);

	 mat4 scaleMatrix = mat4::identity();

	 scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	 mat4 rotationMatrix1 = mat4::identity();

	 rotationMatrix1 = mat4::identity();

	 rotationMatrix1 = vmath::rotate(AngleCube, 1.0f, 0.0f, 0.0f);

	 mat4 rotationMatrix2 = mat4::identity();

	 rotationMatrix2 = mat4::identity();

	 rotationMatrix2 = vmath::rotate(AngleCube, 0.0f, 1.0f, 0.0f);

	 mat4 rotationMatrix3 = mat4::identity();

	 rotationMatrix3 = mat4::identity();

	 rotationMatrix3 = vmath::rotate(AngleCube, 0.0f, 0.0f, 1.0f);

	mat4 rotationMatrix = rotationMatrix1 * rotationMatrix2 * rotationMatrix3;

	 modelViewMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	 
	 
	//push above mvp into vertex shader's mvp uniform

	 glUniformMatrix4fv(modelViewMatrixUniform, 1, GL_FALSE, modelViewMatrix);

	 glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);


	 

	//step 2: Bind with (VAO)

	glBindVertexArray(vaoCube);

	if (bLightingEnabled == TRUE)
	{
		glUniform1i(keyPressUniform, 1);
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform4fv(lightPositionUniform, 1, lightPosition);
	}

	else
	{
		glUniform1i(keyPressUniform, 0);
	}

	//step 3: Draw the geometry

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);


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

	AngleCube = AngleCube + 0.2f;

	if (AngleCube >= 360.0f)
	{
		AngleCube = 0.0f;
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

	if (vbo_Normal_Cube)
	{
		glDeleteBuffers(1, &vbo_Normal_Cube);
		vbo_Normal_Cube = 0;
	}

	//step 10 : delete vbo for position

	if (vbo_Position_Cube)
	{
		glDeleteBuffers(1, &vbo_Position_Cube);
		vbo_Position_Cube = 0;
	}

	

	//step 11 : delete VAO 

	if (vaoCube)
	{
		glDeleteVertexArrays(1, &vaoCube);
		vaoCube = 0;
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











	

