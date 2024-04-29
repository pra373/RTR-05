// Windows header files
// Common headers 
#include<windows.h> //For all WIN32 API's
#include<stdio.h>   //for file I/O
#include<stdlib.h>  //for exit()
#include<algorithm>
#include<random>
#include<iostream>
#include<stdarg.h>
#include<vector>
#include<fstream>
#include<string>
#include<sstream>

using namespace std;


#include "OGL.h" // swatachi header file

#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
#include"helper_timer.h"


// OpenGL header Files
#include<gl/glew.h>   //this must be before gl/gl.h header file
#include<gl/GL.h>
#include"vmath.h"

using namespace vmath;
//timer initialization

StopWatchInterface* sceneTimer = NULL;
float elapsedTime = 0.0f;

using namespace std;



//#include<GL/glu.h>  //not allowed in PP  (There is no GLU utility in PP)

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define HORIZONTAL_QUADS 300
#define VERTICAL_QUADS 300
#define MODEL_VIEW_MATRIX_STACK 32

// Link With OpenGL Library
#pragma comment(lib,"glew32.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib,"winmm.lib")
//#pragma comment(lib, "glu32.lib") //file me OpenGL    //dont need in PP (There is no GLU utility in PP)

// Global Function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global Variable Declarations

//OpenGL Related Global Variables
HDC ghdc = NULL;
HGLRC  ghrc = NULL;       // handle to graphic rendering contex

FILE *gpFile = NULL;
FILE* OBJFile = NULL;

HWND ghwnd = NULL;        // global window handel 
BOOL gbActive = FALSE;    //means by default mazi window active nhi
DWORD dwStyle = 0;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) }; //wpPrev = windowplacementPrevious
BOOL gbFullScreen = FALSE;


GLuint shaderProgramObject = 0;



//Vertex coordinates arrays

GLfloat rightTopX[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };
GLfloat rightTopY[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };

GLfloat leftTopX[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };
GLfloat leftTopY[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };

GLfloat leftBottomX[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };
GLfloat leftBottomY[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };

GLfloat rightBottomX[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };
GLfloat rightBottomY[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };

GLint numOfRows = VERTICAL_QUADS;
GLint numOfColumns = HORIZONTAL_QUADS;
GLint totalQuads = HORIZONTAL_QUADS * VERTICAL_QUADS;

//Texture Coordinates arrays

GLfloat tRightTopX[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };
GLfloat tRightTopY[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };

GLfloat tLeftTopX[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };
GLfloat tLeftTopY[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };

GLfloat tLeftBottomX[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };
GLfloat tLeftBottomY[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };

GLfloat tRightBottomX[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };
GLfloat tRightBottomY[HORIZONTAL_QUADS * VERTICAL_QUADS] = { 0 };

vector <int> visibilityNum;
vector <float> positionBuffer(totalQuads * 8);
vector <float> textureBuffer(totalQuads * 8);

GLint indexUpdate = 0;


GLint randomQuadNumber = 0;

//update display function

GLint numOfVertices = 0;



enum
{
	AMC_ATTRIBUTE_POSITION = 0,
	
	
	AMC_ATTRIBUTE_TEXCOORD
	
};

enum
{
	AMC_ATTRIBUTE_POSITION_LETTERS = 0,
	AMC_ATTRIBUTE_NORMAL_LETTERS
};

GLuint vao = 0;
GLuint vbo_Position = 0;
GLuint vbo_TexCoord = 0;

//uniforms

GLuint mvpMatrixUniform = 0;
GLuint textureSamplerUniform = 0;

//blend variables

GLfloat blendLetters = 0;


//texture variable
GLuint TextureBaba = 0;

mat4 perspectiveProjectionMatrix;  //using 4*4 matrix.
                                   //mat4 - represents 4*4 matrix.   
								   //mat4 is not c++ datattype, it is defined in vmath.h.



//obj file related variables

GLuint shaderProgramObject_Letters = 0; //for letters

GLuint vao_Letters = 0;

GLuint vbo_Position_Letters = 0;
GLuint vbo_Normals_Letters = 0;


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

// Light Variables

GLfloat lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //white diffuse light
GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightPosition[] = { 100.0f, 100.0f, 100.0f, 1.0f }; //

//GLfloat materialAmbient[] = { 0.2f, 0.0113f, 0.01145f, 1.0f };
//GLfloat materialDiffuse[] = { 1.0f, 0.0f,  0.0f, 1.0f };
//GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//GLfloat materialShininess = 128.0f;

GLfloat materialAmbient[] = { 0.2125f, 0.01275f, 0.054f, 1.0f };
GLfloat materialDiffuse[] = { 0.714f, 0.4284f,  0.18144f, 1.0f };
GLfloat materialSpecular[] = { 0.393548f, 0.271906f, 0.166721f, 1.0f };
GLfloat materialShininess = 0.2 * 128.0f;

// vectors to store vertex positions, normals

vector <GLfloat> vertices;
vector <GLfloat> normals;
vector <GLfloat> texCoords;

//vectors to store face indices

vector <GLint> vertexIndices;
vector <GLint> texCoordIndices;
vector <GLint> normalIndices;

//vectors to store position buffer texture buffer and normal buffer

vector <GLfloat> positionBuffer_Letters;
vector <GLfloat> normalBuffer_Letters;

GLfloat alphaValue = 0.0f;


//PushPop Related variables

mat4 matrixStack[MODEL_VIEW_MATRIX_STACK];
int matrixStackTop = -1;


//function declaration

void initializeMatrixStack(void);
void pushMatrix(mat4 matrix);
mat4 popMatrix(void);


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

	
	if (fopen_s(&gpFile, "Log.txt", "w") != 0) //secuare version of fopen()
	{
		MessageBox(NULL, TEXT("Log File Cannot Be Opened"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	fprintf(gpFile, "Program Started Successfully...\n\n\n");

	

	if (fopen_s(&OBJFile, "OBJLog.txt", "w") != 0) //secuare version of fopen()
	{
		MessageBox(NULL, TEXT("OBJ Log File Cannot Be Opened"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}



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

	PlaySound(MAKEINTRESOURCE(MySound), NULL, SND_RESOURCE | SND_ASYNC);

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
					elapsedTime = display();
					sprintf(caption, " Prathamesh Paropkari       Baba Birthday Demo                    Elapsed Time : %f", elapsedTime);
					SetWindowText(hwnd, caption);

					Sleep(80);

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
	//variable declaration
	
	BOOL bResult = 0;

	GLint rowNum = 0;
	GLint columnNum = 0;

	GLint quadIndex = 0;

	GLint numOfIterationX = 1;
	GLint numOfIterationY = 1;

	int j = 0;

	//function declarations
	void printGLInfo(void);
	void uninitialize(void);
	void resize(int,int);
	BOOL loadGLTexture(GLuint*, TCHAR[]);
	BOOL LoadGLPNGTexture(GLuint*, const char*);
	void loadOBJFile(void);
	void printLoadedData(void);

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

	//printGLInfo();

	loadOBJFile();

	printLoadedData();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//initialize array of vertices and texcoords

	for (rowNum = 0; rowNum < numOfRows; rowNum++)
	{
		numOfIterationX = 1;

		for (columnNum = 0; columnNum < numOfColumns; columnNum++)
		{
			//for right top Vertex coordinates

			rightTopX[quadIndex] = ((2.0f) / (numOfColumns)) * numOfIterationX;
			rightTopY[quadIndex] = ((2.0f) / (numOfRows)) * numOfIterationY;

			//for right top Texture coordinates

			tRightTopX[quadIndex] = ((1.0f) / (numOfColumns)) * numOfIterationX;
			tRightTopY[quadIndex] = ((1.0f) / (numOfRows)) * numOfIterationY;

			//for left top Vertex coordinates

			leftTopX[quadIndex] = rightTopX[quadIndex] - ((2.0f) / (numOfColumns));
			leftTopY[quadIndex] = rightTopY[quadIndex];

			//for left top Texture coordinates

			tLeftTopX[quadIndex] = tRightTopX[quadIndex] - ((1.0f) / (numOfColumns));
			tLeftTopY[quadIndex] = tRightTopY[quadIndex];

			//for left bottom vertex coordinate

			leftBottomX[quadIndex] = leftTopX[quadIndex];
			leftBottomY[quadIndex] = leftTopY[quadIndex] - ((2.0f) / (numOfRows));

			//for left bottom texture coordinates

			tLeftBottomX[quadIndex] = tLeftTopX[quadIndex];
			tLeftBottomY[quadIndex] = tLeftTopY[quadIndex] - ((1.0f) / (numOfRows));

			//for right bottom vertex coordinates

			rightBottomX[quadIndex] = rightTopX[quadIndex];
			rightBottomY[quadIndex] = rightTopY[quadIndex] - ((2.0f) / (numOfRows));

			//for right bottom Texture coordinates

			tRightBottomX[quadIndex] = tRightTopX[quadIndex];
			tRightBottomY[quadIndex] = tRightTopY[quadIndex] - ((1.0f) / (numOfRows));

			//necessory variable updates

			numOfIterationX++;
			quadIndex++;
		}

		numOfIterationY++;

	}

	//create a vector to store visibility control numbers sequentially

	for (int i = 0; i < totalQuads; i++)
	{
		visibilityNum.push_back(i);
		fprintf(gpFile, "%d\n", visibilityNum[i]);
	}

	//seed the random number generator with the current time (Seconds passed since the unix epoch)

	default_random_engine rng(time(NULL));



	//shuffle the array of visibility control variables using shuffe function

	shuffle(visibilityNum.begin(), visibilityNum.end(), rng);



	fprintf(gpFile, "Vector Values After Shuffeling : \n\n");

	//print shuffed vector into log file

	for (int i = 0; i < totalQuads; i++)
	{

		fprintf(gpFile, "%d\n", visibilityNum[i]);
	}


	//fill position buffer by visibilityNum vector

	for (int i = 0; i < totalQuads; i++)
	{

		fprintf(gpFile, "J = %d\n", j);
		positionBuffer[j] = rightTopX[visibilityNum[i]];  //j = 0 , 8
		textureBuffer[j] = tRightTopX[visibilityNum[i]];

		j++;  //j=1
		fprintf(gpFile, "J = %d\n", j);
		

		positionBuffer[j] = rightTopY[visibilityNum[i]]; //j = 1
		textureBuffer[j] = tRightTopY[visibilityNum[i]];

		j++; // j = 2
		fprintf(gpFile, "J = %d\n", j);

		positionBuffer[j] = leftTopX[visibilityNum[i]]; // j = 2
		textureBuffer[j] = tLeftTopX[visibilityNum[i]];

		j++;  //j = 3
		fprintf(gpFile, "J = %d\n", j);

		positionBuffer[j] = leftTopY[visibilityNum[i]];
		textureBuffer[j] = tLeftTopY[visibilityNum[i]];

		j++; //j = 4
		fprintf(gpFile, "J = %d\n", j);

		positionBuffer[j] = leftBottomX[visibilityNum[i]];
		textureBuffer[j] = tLeftBottomX[visibilityNum[i]];

		j++; //j = 5
		fprintf(gpFile, "J = %d\n", j);

		positionBuffer[j] = leftBottomY[visibilityNum[i]];
		textureBuffer[j] = tLeftBottomY[visibilityNum[i]];

		j++; //j = 6
		fprintf(gpFile, "J = %d\n", j);

		positionBuffer[j] = rightBottomX[visibilityNum[i]];
		textureBuffer[j] = tRightBottomX[visibilityNum[i]];

		j++;//j = 7
		fprintf(gpFile, "J = %d\n", j);

		positionBuffer[j] = rightBottomY[visibilityNum[i]];
		textureBuffer[j] = tRightBottomY[visibilityNum[i]];

		j++; // j = 8
		

	}

	//Steps to follow in initialize function for PP

	// step 1 : write vertex shader code
	
	//vertex shader

	const GLchar* vertexShaderSourceCode =
		"#version 460 core\n"             // (version of GLSL * 100) core means you are using core profile i.e PP
		"\n"
		"in vec2 aPosition;\n"
		"in vec2 aTexCoord;\n"
		"out vec2 oTexCoord;\n"
		"uniform mat4 uMVPMatrix;\n"          //u for uniform datatype
		"void main(void)\n"
		"{\n"
		"gl_Position=uMVPMatrix * vec4(aPosition.xy, 0.0, 1.0);\n"
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
		"in vec2 oTexCoord;\n"
		"uniform sampler2D uTextureSampler;\n"  //sampler is a component which collects samples of image info
		"out vec4 FragColor;\n" 
		"void main(void)\n" 
		"{\n" 
		"FragColor = texture(uTextureSampler, oTexCoord);\n" //texture is a function in GLSL                                                     which calls sampler and tell                                                        him that sample the texture                                                       to coordinates given by me                                                         in the position array
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

	 //step 12: Declare position and color arrays 

	 //get Shader uniform Locations

	 mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "uMVPMatrix");  //must be done after linkage of shader program

	 textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "uTextureSampler");

	 if (textureSamplerUniform == -1)
	 {
		 fprintf(gpFile, "Loading of uniform variable failed !! Exiting now\n");
		 return(-7);
	 }

	 ////step 13: Create vertex array object
	 //VAO

	// glGenVertexArrays(1, &vao);

	 //step 14: Bind with VAO

	 glBindVertexArray(vao);

	 //step 15: Create vertex buffer object (VBO) for position
	 
	 //VBO for position

	 glGenBuffers(1, &vbo_Position);

	 //step 16: Bind with VBO of position

	 glBindBuffer(GL_ARRAY_BUFFER, vbo_Position);

	 glBufferData(GL_ARRAY_BUFFER, (positionBuffer.size() * sizeof(float)), positionBuffer.data(), GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	 glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);


	

	
	 // buffer for texcoord

	 glGenBuffers(1, &vbo_TexCoord);

	 //step 16: Bind with VBO of TEXCOORD

	 glBindBuffer(GL_ARRAY_BUFFER, vbo_TexCoord);

	 glBufferData(GL_ARRAY_BUFFER, (textureBuffer.size() * sizeof(float)), textureBuffer.data(), GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	 glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);


	 //step 18 : unbind with VAO
	 glBindVertexArray(0);

//******************** *************** Shaders related to letters ****************************************

	 const GLchar* vertexShaderSourceCode_Letters =
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

	 GLuint vertexShaderObject_Letters = glCreateShader(GL_VERTEX_SHADER);

	 //step 3 : Give vertex shader code to OpenGL
	 glShaderSource(vertexShaderObject_Letters, 1, (const GLchar**)&vertexShaderSourceCode_Letters, NULL);

	 //Step 4 : Let OpenGL engine Compile the vertex shader code
	 glCompileShader(vertexShaderObject_Letters);

	 status = 0;
	 infoLogLength = 0;
	 szInfoLog = NULL;

	 //step 5 : Check for vertex shader compilation
	 //step 5 (a) :  get compile status in status variable
	 glGetShaderiv(vertexShaderObject_Letters, GL_COMPILE_STATUS, &status);

	 //step 5 (b) : if error occur i.e status == GL_FALSE,  get length of error message into infoLogLength variable
	 if (status == GL_FALSE)
	 {

		 glGetShaderiv(vertexShaderObject_Letters, GL_INFO_LOG_LENGTH, &infoLogLength);

		 if (infoLogLength > 0)
		 {
			 //step 5 (c) : declare a string variable allocate memory equal to above length i.e length of error message
			 szInfoLog = (GLchar*)malloc(infoLogLength);


			 if (szInfoLog != NULL)
			 {
				 // step 5 (d) : get the compilation error log info into memory allocated string
				 glGetShaderInfoLog(vertexShaderObject_Letters, infoLogLength, NULL, szInfoLog);

				 //step 5 (e) : print error message into log file
				 fprintf(gpFile, "Vertex shader letters compilation error log : %s\n", szInfoLog);


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

	 const GLchar* fragmentShaderSourceCode_Letters =
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

	 GLuint fragmentShaderObject_Letters = glCreateShader(GL_FRAGMENT_SHADER);

	 glShaderSource(fragmentShaderObject_Letters, 1, (const GLchar**)&fragmentShaderSourceCode_Letters, NULL);

	 glCompileShader(fragmentShaderObject_Letters);

	 status = 0;
	 infoLogLength = 0;
	 szInfoLog = NULL;

	 glGetShaderiv(fragmentShaderObject_Letters, GL_COMPILE_STATUS, &status);

	 if (status == GL_FALSE)
	 {

		 glGetShaderiv(fragmentShaderObject_Letters, GL_INFO_LOG_LENGTH, &infoLogLength);

		 if (infoLogLength > 0)
		 {
			 szInfoLog = (GLchar*)malloc(infoLogLength);


			 if (szInfoLog != NULL)
			 {
				 glGetShaderInfoLog(fragmentShaderObject_Letters, infoLogLength, NULL, szInfoLog);
				 fprintf(gpFile, "Fragment shader letters compilation error log : %s\n", szInfoLog);
				 free(szInfoLog);
				 szInfoLog = NULL;


			 }



		 }

		 uninitialize();
	 }

	 //linking process for shader program.

	 // shader program

	 //step 7 : create shader program object

	 shaderProgramObject_Letters = glCreateProgram();

	 //step 8 : Attach both shaders into this program            also know as prelinking binding  should be done before linking and after attachment
	 glAttachShader(shaderProgramObject_Letters, vertexShaderObject_Letters);
	 glAttachShader(shaderProgramObject_Letters, fragmentShaderObject_Letters);

	 //step 9: bind attribute location to the shader program object

	 glBindAttribLocation(shaderProgramObject_Letters, AMC_ATTRIBUTE_POSITION_LETTERS, "aPosition");

	 glBindAttribLocation(shaderProgramObject_Letters, AMC_ATTRIBUTE_NORMAL_LETTERS, "aNormal");

	 //step 10: Link the shader program

	 glLinkProgram(shaderProgramObject_Letters);

	 //step 11: do linking error check fpr shader program

	 status = 0;
	 infoLogLength = 0;
	 szInfoLog = NULL;

	 glGetProgramiv(shaderProgramObject_Letters, GL_LINK_STATUS, &status);

	 if (status == GL_FALSE)
	 {
		 glGetProgramiv(shaderProgramObject_Letters, GL_INFO_LOG_LENGTH, &infoLogLength);

		 if (infoLogLength > 0)
		 {
			 szInfoLog = (GLchar*)malloc(infoLogLength);


			 if (szInfoLog != NULL)
			 {
				 glGetProgramInfoLog(shaderProgramObject_Letters, infoLogLength, NULL, szInfoLog);
				 fprintf(gpFile, "shader program letters linking error log : %s\n", szInfoLog);
				 free(szInfoLog);
				 szInfoLog = NULL;


			 }



		 }

		 uninitialize();
	 }

	 // Get shader uniform location

	 modelMatrixUniform = glGetUniformLocation(shaderProgramObject_Letters, "uModelMatrix");



	 viewMatrixUniform = glGetUniformLocation(shaderProgramObject_Letters, "uViewMatrix");

	 projectionMatrixUniform = glGetUniformLocation(shaderProgramObject_Letters, "uProjectionMatrix");


	 lightAmbientUniform = glGetUniformLocation(shaderProgramObject_Letters, "ulightAmbient");


	 lightDiffuseUniform = glGetUniformLocation(shaderProgramObject_Letters, "ulightDiffuse");

	 lightSpecularUniform = glGetUniformLocation(shaderProgramObject_Letters, "ulightSpecular");


	 lightPositionUniform = glGetUniformLocation(shaderProgramObject_Letters, "ulightPosition");

	 materialAmbientUniform = glGetUniformLocation(shaderProgramObject_Letters, "uMaterialAmbient");

	 materialDiffuseUniform = glGetUniformLocation(shaderProgramObject_Letters, "uMaterialDiffuse");


	 materialSpecularUniform = glGetUniformLocation(shaderProgramObject_Letters, "uMaterialSpecular");

	 materialShininessUniform = glGetUniformLocation(shaderProgramObject_Letters, "uMaterialShininess");

	 blendFactorUniform = glGetUniformLocation(shaderProgramObject_Letters, "ublendFactor");


	 // vao
	 glGenVertexArrays(1, &vao_Letters);
	 glBindVertexArray(vao_Letters);

	 // position vbo
	 glGenBuffers(1, &vbo_Position_Letters);
	 glBindBuffer(GL_ARRAY_BUFFER, vbo_Position_Letters);
	 glBufferData(GL_ARRAY_BUFFER, positionBuffer_Letters.size() * sizeof(float), positionBuffer_Letters.data(), GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_POSITION_LETTERS, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	 glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION_LETTERS);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);

	 // normal vbo
	 glGenBuffers(1, &vbo_Normals_Letters);
	 glBindBuffer(GL_ARRAY_BUFFER, vbo_Normals_Letters);
	 glBufferData(GL_ARRAY_BUFFER, normalBuffer_Letters.size() * sizeof(float), normalBuffer_Letters.data(), GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL_LETTERS, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	 glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL_LETTERS);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);

	 glBindVertexArray(0);


	// 2nd change for depth or Enabling depth
	
	glClearDepth(1.0f); // Compulsory
	glEnable(GL_DEPTH_TEST); // 8 test paike ek(depth) test enable kr
	glDepthFunc(GL_LEQUAL); 
	


	// Set The ClearColor Of Window To Blue
	// Clear OpenGl Stars
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Blue Color

	/*bResult = loadGLTexture(&TextureBaba, MAKEINTRESOURCE(MYSMILEY));

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of smiley texture failed !! Exiting now\n");
		return(-6);
	}*/

	bResult = LoadGLPNGTexture(&TextureBaba, "BabaPhoto2.png");


	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of baba texture failed !! Exiting now\n");
		return(-7);
	}

	

	glEnable(GL_LIGHT0);

	glEnable(GL_LIGHTING);

	glEnable(GL_COLOR_MATERIAL);


	//warmup resize

	//initialize orthographic projection matrix

	perspectiveProjectionMatrix = vmath::mat4::identity();

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
	if(height <= 0)
		height = 1;

	
	//glMatrixMode(GL_PROJECTION); //not allowed in PP 
	//glLoadIdentity();            not allowed in PP 

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	//set Perspective projection matrix

	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	

	


}

float display(void)
{
	//function declaration
	void DrawLetters(void);
	

	//code
	// 3rd Change GL_DEPTH_BUFFER_BIT
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	elapsedTime = sdkGetTimerValue(&sceneTimer);

	elapsedTime = elapsedTime / 1000;  //convert time in seconds
	

	//PP steps for display function

	//step 1: Use shader program object

	
	
	//glUseProgram(shaderProgramObject_Letters);

		
	

	
	
	glUseProgram(shaderProgramObject);
	

	

	

	//transformations

	//matrix declarations

	
	
	mat4 modelMatrix = mat4::identity();

	mat4 viewMatrix = mat4::identity();

	mat4 scaleMatrix = mat4::identity();

	modelMatrix = vmath::translate(-1.0f, -1.0f, -4.5f);

	scaleMatrix = vmath::scale(1.0f, 1.3f, 1.0f);

	mat4 modelViewScaleMatrix = modelMatrix * viewMatrix * scaleMatrix;

	mat4 madelViewScaleProjectionMatrix = perspectiveProjectionMatrix * modelViewScaleMatrix;  //order of multiplication is not commutativ. i.e mat1 * mat2 != mat2 * mat1

		//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, madelViewScaleProjectionMatrix);

		//for texture

	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, TextureBaba);

	glUniform1i(textureSamplerUniform, 0);

	glBindVertexArray(vao);

		//draw the quads

	for (int i = 0; i < indexUpdate; i++)
	{
		numOfVertices += 4;
		glDrawArrays(GL_QUADS, 0, numOfVertices);
	}

		//step 4: unBind with (VAO)

	glBindVertexArray(0);

	if (elapsedTime > 42.0f)
	{
		DrawLetters();
	}




		//step 5: Unuse shader program object
	glUseProgram(0);  

//******************************** Transformation and drawing of Letters **********************************

		
	
	
		
	
	SwapBuffers(ghdc);

	return(elapsedTime);

}

void update(void)
{
	//code
	//only at time of change animation here code will come

	if (elapsedTime > 1.6f)
	{
		if (indexUpdate < visibilityNum.size())
		{
			indexUpdate += 1;
		}

		/*if (indexUpdate >= visibilityNum.size() && blendLetters < 1.0f)
		{
			blendLetters = blendLetters + 0.001f;
		}*/
	}

	
	if (elapsedTime > 42.0f)
	{
		if (alphaValue <= 2.5f)
		{
			alphaValue += 0.025f;
		}
		
	}
		
	

}


void uninitialize(void)
{
   // Function Declarations
	void ToggleFullScreen(void);

   //code
   
	//steps for uninitialize function in PP

	if (sceneTimer)
	{
		sdkStopTimer(&sceneTimer);
		sdkDeleteTimer(&sceneTimer);

		sceneTimer = NULL;
	}

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

	//delete shader progrom related to letters

	if (shaderProgramObject_Letters)
	{
		//step 1 : use shader program object

		glUseProgram(shaderProgramObject_Letters);

		GLint numShaders = 0;

		//step 2 : get number of attached shaders
		glGetProgramiv(shaderProgramObject_Letters, GL_ATTACHED_SHADERS, &numShaders);



		if (numShaders > 0)
		{
			//step 3 : Create and allocate memory of shader objects according to number of attached shaders
			GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint));

			if (pShaders != NULL)
			{
				//step 4 : Get shader object into the allocated array.

				glGetAttachedShaders(shaderProgramObject_Letters, numShaders, NULL, pShaders);

				//step 5 : Detach and delete shader objects iteratively

				for (GLint i = 0; i < numShaders; i++)
				{
					glDetachShader(shaderProgramObject_Letters, pShaders[i]);
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

		glDeleteProgram(shaderProgramObject_Letters);
		shaderProgramObject_Letters = 0;






	}


	//delete normal buffers

	if (vbo_Normals_Letters)
	{
		glDeleteBuffers(1, &vbo_Normals_Letters);
		vbo_Normals_Letters = 0;
	}

	//step 10 : delete vbo for position

	if (vbo_Position_Letters)
	{
		glDeleteBuffers(1, &vbo_Position_Letters);
		vbo_Position_Letters = 0;
	}

	if (vbo_Position)
	{
		glDeleteBuffers(1, &vbo_Position);
		vbo_Position = 0;
	}

	

	if (TextureBaba)
	{
		glDeleteTextures(1, &TextureBaba);
		TextureBaba = 0;
	}

	//step 11 : delete VAO 

	if (vao_Letters)
	{
		glDeleteVertexArrays(1, &vao_Letters);
		vao_Letters = 0;
	}

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

BOOL loadGLTexture(GLuint* Texture, TCHAR image_Resource_ID[])
{
	//Local variable declarations
	HBITMAP hBitmap = NULL;
	BITMAP bmp;

	//work of OS to load image and extract data from image starts here
	//load the image - step5 in notebook
	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), image_Resource_ID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

	if (hBitmap == NULL)
	{
		fprintf(gpFile, "Load image function failed !! Exiting now\n");
		return(FALSE);
	}

	//Get Image Data  - step 6
	GetObject(hBitmap, sizeof(BITMAP), &bmp);

	//work of OS to load image and extract data from image ends here
	// work of OPENGL starts
	//step 7 

	glGenTextures(1, Texture); // generate textures

	//step 8
	glBindTexture(GL_TEXTURE_2D, *Texture);

	//step 9

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//step 10

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


	//step 11

	/*gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, (void*)bmp.bmBits);*/  //glu functions are not allowed in PP

	glTexImage2D(GL_TEXTURE_2D, 0, 3, bmp.bmWidth, bmp.bmHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, (void*)bmp.bmBits);

	glGenerateMipmap(GL_TEXTURE_2D);

	//step 12
	glBindTexture(GL_TEXTURE_2D, 0);

	//step 13

	DeleteObject(hBitmap);
	hBitmap = NULL;
	return(TRUE);

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

void initializeMatrixStack(void)
{
	matrixStackTop = 0;

	for (int i = 0; i < MODEL_VIEW_MATRIX_STACK; i++)
	{
		matrixStack[i] = mat4::identity();
	}
}
void pushMatrix(mat4 matrix)
{
	void uninitialize(void);

	//code

	if (matrixStackTop >= MODEL_VIEW_MATRIX_STACK)
	{
		fprintf(gpFile, "ERROR : Exceeded matrix stack limit\n");
		uninitialize();
	}

	matrixStack[matrixStackTop] = matrix;

	matrixStackTop++;
}
mat4 popMatrix(void)
{
	void uninitialize(void);

	//code

	if (matrixStackTop < 0)
	{
		fprintf(gpFile, "ERROR : Matrix Stack is empty\n");

		uninitialize();
	}

	matrixStack[matrixStackTop] = mat4::identity();
	matrixStackTop--;

	mat4 matrix = matrixStack[matrixStackTop];

	return matrix;
}

void loadOBJFile(void)
{
	ifstream dataIn;

	dataIn.open("NewObjFile.obj");

	if (!dataIn.is_open())
	{
		fprintf(OBJFile, "Failed to load the OBJ file !!!\n\n");
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
				fprintf(OBJFile, "\n\nError extracting vertex coordinates\n\n");
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
				fprintf(OBJFile, "\n\nError extracting Normals\n\n");
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
				fprintf(OBJFile, "\n\nError extracting Texture Coordinates\n\n");
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

		positionBuffer_Letters.push_back(vertexX);
		positionBuffer_Letters.push_back(vertexY);
		positionBuffer_Letters.push_back(vertexZ);

		// Move to the next vertex index
		num1++;
	}

	int num2 = 0;

	while (num2 < normalIndices.size())
	{
		//get current vertex index from the vertexIndices vector
		int normalIndex = normalIndices[num2] - 1;

		//calculate start index in vertices data vector as data is stored in x,y,z format
		int startIndex = normalIndex * 3;

		// Extract x, y, z components of the current vertex
		GLfloat normalX = normals[startIndex];
		GLfloat normalY = normals[startIndex + 1];
		GLfloat normalZ = normals[startIndex + 2];

		// Store the vertex data in your position buffer

		normalBuffer_Letters.push_back(normalX);
		normalBuffer_Letters.push_back(normalY);
		normalBuffer_Letters.push_back(normalZ);

		// Move to the next vertex index
		num2++;
	}



	dataIn.close();
}

void printLoadedData(void)
{
	GLint i = 0;
	GLint j = 0;
	GLint k = 0;
	GLint l = 0;
	GLint m = 0;
	GLint n = 0;

	

	while (i < vertices.size())
	{
		fprintf(OBJFile, "X = %f\n", vertices[i]);

		i++;

		fprintf(OBJFile, "Y = %f\n", vertices[i]);

		i++;

		fprintf(OBJFile, "Z = %f\n\n\n", vertices[i]);



		i++;

	}

	fprintf(OBJFile, "\n\n**************************** Printing Normals*****************************************\n\n");

	while (j < normals.size())
	{
		fprintf(OBJFile, "X = %f\n", normals[j]);

		j++;

		fprintf(OBJFile, "Y = %f\n", normals[j]);

		j++;

		fprintf(OBJFile, "Z = %f\n\n\n", normals[j]);



		j++;
	}

	fprintf(OBJFile, "\n\n**************************** Printing Texture Coordinates*****************************************\n\n");

	while (k < texCoords.size())
	{
		fprintf(OBJFile, "X = %f\n", texCoords[k]);

		k++;

		fprintf(OBJFile, "Y = %f\n\n", texCoords[k]);

		k++;

	}

	fprintf(OBJFile, "\n\n**************************** Printing Vertex Indices indices*****************************************\n\n");

	while (l < vertexIndices.size())
	{
		fprintf(OBJFile, "V = %d\n", vertexIndices[l]);

		l++;
	}


	fprintf(gpFile, "\n\n**************************** Printing texture Indices indices*****************************************\n\n");

	while (m < texCoordIndices.size())
	{
		fprintf(OBJFile, "T = %d\n", texCoordIndices[m]);

		m++;
	}

	fprintf(OBJFile, "\n\n**************************** Printing Normal Indices indices*****************************************\n\n");

	while (n < normalIndices.size())
	{
		fprintf(OBJFile, "N = %d\n", normalIndices[n]);

		n++;
	}

	fprintf(OBJFile, "\n\n**************************** Printing Position buffer *****************************************\n\n");

	int num = 0;
	while (num < positionBuffer_Letters.size())
	{
		fprintf(OBJFile, "vertex %d :  X = %f  y = %f  z = %f\n", num, positionBuffer_Letters[num], positionBuffer_Letters[num + 1], positionBuffer_Letters[num + 2]);

		num++;
	}

	fprintf(OBJFile, "\n\n**************************** Printing normal buffer *****************************************\n\n");

	num = 0;
	while (num < normalBuffer_Letters.size())
	{
		fprintf(OBJFile, "normal %d :  X = %f  y = %f  z = %f\n", num, normalBuffer_Letters[num], normalBuffer_Letters[num + 1], normalBuffer_Letters[num + 2]);

		num++;
	}

}

void DrawLetters(void)
{
	glUseProgram(shaderProgramObject_Letters);

	//transformations

	mat4 modelViewMatrix = mat4::identity();

	mat4 translationMatrix = mat4::identity();

	mat4 rotationMatrix = mat4::identity();

	mat4 scaleMatrix = mat4::identity();

	scaleMatrix = vmath::scale(0.6f, 0.6f, 0.6f);

	translationMatrix = vmath::translate(-2.5f, -2.27f, -7.5f);

	rotationMatrix = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);

	mat4 modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

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



	glBindVertexArray(vao_Letters);

	GLint numVertices = positionBuffer.size() / 3;



	glDrawArrays(GL_TRIANGLES, 0, numVertices);





	// *** unbind vao ***
	glBindVertexArray(0);

	//step 5: Unuse shader program object
	glUseProgram(0);

}














	

