// standard header file
#include<stdio.h>
#include<stdlib.h> // for exit
#include<memory.h> // for memset

#include<iostream>
#include<vector>

//OpenGL Header files
#include<GL/glew.h>
#include<GL/glx.h>  
#include<GL/gl.h>
#include<iostream>
#include<vector>

#include"vmath.h"

using namespace vmath;
using namespace std;


// X-11 header files
#include<X11/Xlib.h> // for all X-Window API
#include<X11/Xutil.h>  // for XVisualInfo and related API
#include<X11/XKBlib.h> 



// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global Variable Declarations

FILE *gpFile = NULL;
Display *display = NULL;
Colormap colormap;
Window window; // X-window la represent karnara structure...
XVisualInfo *visualInfo = NULL;   //this must be obtained from framebuffer config(GLXFBConfig) (first change to be done to shift in PP)

//OpenGL Global Variables

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display *,GLXFBConfig,GLXContext,Bool,const int *);

glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;

GLXFBConfig glxFBConfig;
GLXContext glxContext = NULL;   // must not be created by createcontext but should be created by core profile attributes and visuainfo created by framebuffer config.
                                //for PP GLXContext should be supporting direct rendering AKA hardware rendering.
                                // all this changes must be done to shift in PP
Bool bFullscreen = False;
Bool bActiveWindow = False;


GLuint shaderProgramObject = 0;
GLuint shaderProgramObject_Shapes = 0;


vector<GLfloat>linePositions;
vector<GLfloat>circlePositions;
vector<GLfloat>CircleColor;


enum
{
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_COLOR
	
};

enum
{
	AMC_ATTRIBUTE_POSITION_SHAPES = 0,
	AMC_ATTRIBUTE_COLOR_SHAPES

};

GLuint vao = 0;
GLuint vbo_Position = 0;

GLuint vbo_Position_Triangle = 0;
GLuint vbo_Color_Triangle = 0;

GLuint vbo_Position_Square = 0;
GLuint vbo_Color_Square = 0;

GLuint vbo_Position_Circle = 0;
GLuint vbo_Color_Circle = 0;

GLuint vao_Triangle = 0;
GLuint vao_Square = 0;
GLuint vao_Circle = 0;

GLuint mvpMatrixUniform = 0;

GLuint mvpMatrixUniform_Shapes = 0;
mat4 perspectiveProjectionMatrix;  //using 4*4 matrix.
                                   //mat4 - represents 4*4 matrix.   
								   //mat4 is not c++ datattype, it is defined in vmath.h.

Bool enableGraph = False;
Bool enableTriangle = False;
Bool enableSquare = False;
Bool enableCircle = False;


// Main
int main(void)
{
    // local function declarations

    void uninitialize(void);
    void toggleFullscreen(void);
    int initialize(void);
    void resize(int,int);
    void draw(void);
    void update (void);

    // local variable declarations

     int defaultScreen;
     Bool bDone = False;

     //PP related variables

     GLXFBConfig *glxFBConfigs = NULL;
     GLXFBConfig bestglxFBConfig;
     XVisualInfo *tempXvisualInfo = NULL;

     int bestFBConfig = -1;
     int bestNumSamples = -1;
     int worstFrameBufferConfig = -1;
     int worstNumberOfSamples = 9999;
     int sampleBuffers,samples;
     int i=0;

     int numFBConfigs;


     int frameBufferAttributes[]={GLX_DOUBLEBUFFER,True,
                                  GLX_X_RENDERABLE,True,                   //pp
                                  GLX_DRAWABLE_TYPE,GLX_WINDOW_BIT,       //pp
                                  GLX_RENDER_TYPE,GLX_RGBA_BIT,          //pp
                                  GLX_X_VISUAL_TYPE,GLX_TRUE_COLOR,   //pp                           
                                  GLX_RED_SIZE,8,
                                  GLX_GREEN_SIZE,8,
                                  GLX_BLUE_SIZE,8,
                                  GLX_ALPHA_SIZE,8,
                                  GLX_DEPTH_SIZE,24,
                                  GLX_STENCIL_SIZE,8,
                                  None};

     
     // variable for centering 
     int screenWidth, screenHeight;

     

     //XVisualInfo visualInfo;

     
     XSetWindowAttributes windowAttributes;
     int styleMask;
     Atom windowManagerDelete;
     XEvent event;
     KeySym keySym;

     char keys[26];


    // code

    //fileIO code

    gpFile = fopen("log.txt","w");

    if(gpFile == NULL)
    {
        printf("fail to create log file\n");
        exit(0);
    }

    else
    {
        fprintf(gpFile,"Log File created successfully !!!\n\n");
    }

    
    display = XOpenDisplay(NULL);

    if(display == NULL)
    {
        printf("XOpenDisplay Failed\n");

        uninitialize();
        exit(1);  // abortive
    }

    // 2nd Step -> Get default screen from above display...
    defaultScreen = XDefaultScreen(display);


    //get available FB Configs from Xwindows

    glxFBConfigs = glXChooseFBConfig(display,DefaultScreen(display),frameBufferAttributes,&numFBConfigs);

    if(glxFBConfigs == NULL)
    {
        fprintf(gpFile,"Matching glxFBconfigs cannot be found\n\n");
        uninitialize();
        exit(-1);
    }

    fprintf(gpFile,"%d matching FBconfigs found",numFBConfigs);

    //find best matching FB config from above array

    for(i = 0;i < numFBConfigs; i++)
    {
        tempXvisualInfo = glXGetVisualFromFBConfig(display,glxFBConfigs[i]);

        if(tempXvisualInfo != NULL)
        {
            //get sample buffers
            glXGetFBConfigAttrib(display,glxFBConfigs[i],GLX_SAMPLE_BUFFERS,&sampleBuffers);

            //get samples
            glXGetFBConfigAttrib(display,glxFBConfigs[i],GLX_SAMPLES,&samples);

            if(bestFBConfig < 0 || sampleBuffers && samples > bestNumSamples)
            {
                bestFBConfig = i;
                bestNumSamples = samples;
            }

            if(worstFrameBufferConfig < 0 || !sampleBuffers || samples < worstNumberOfSamples)
            {
                worstFrameBufferConfig = i;
                worstNumberOfSamples = samples;
            }

            XFree(tempXvisualInfo);
        }

        
        

        
        

        
    }

    //accordingly get best GLX FB config

    bestglxFBConfig = glxFBConfigs[bestFBConfig];

    //assign this FBconfig to global GLX FB config

    glxFBConfig = bestglxFBConfig;

    //free memory given to array

    XFree(glxFBConfigs);

    //now get the final visual from best FBconfig

    visualInfo = glXGetVisualFromFBConfig(display,glxFBConfig);

    fprintf(gpFile,"The chosen visual ID is = 0x%lu\n",visualInfo->visualid);

    // Step 5 -> Set Window Attributes/Properties...
    memset((void*)&windowAttributes, 0, sizeof(XSetWindowAttributes));

    windowAttributes.border_pixel = 0; // hya window chya border la color asnar ahe ka.. 0 means mala dyaycha nhi tu default dey..
    windowAttributes.background_pixel = XBlackPixel(display, visualInfo->screen);
    windowAttributes.background_pixmap = 0;
    windowAttributes.colormap = XCreateColormap(display,XRootWindow(display, visualInfo->screen),visualInfo->visual,AllocNone);
    
    // Step 6 -> Assign this colormap to global Colormap
    colormap = windowAttributes.colormap;

    // Set the style of window
    styleMask = CWBorderPixel | CWBackPixel | CWColormap | CWEventMask;

    // Step 8 -> Now Finally Create The Window
    window = XCreateWindow(display,
                           XRootWindow(display, visualInfo->screen), 
                           0, 
                           0,
                           WIN_WIDTH,
                           WIN_HEIGHT,
                           0,
                           visualInfo->depth,
                           InputOutput,
                           visualInfo->visual,
                           styleMask,
                           &windowAttributes
                           );

    if(!window)
    {
        printf("XCreateWindow failed\n");

        uninitialize();
        exit(1);
    }

    // Step 9 -> Specify to which events this window should respond...
    XSelectInput(display,window,ExposureMask | VisibilityChangeMask | StructureNotifyMask | KeyPressMask | ButtonPressMask | PointerMotionMask | FocusChangeMask);

    // Step 10 -> Specify window manager delete atom...
    windowManagerDelete = XInternAtom(display, "WM_DELETE_WINDOW", True);

    // Step 11 -> Add/Set above atom as protocol for window manager
    XSetWMProtocols(display, window, &windowManagerDelete, 1);

    // Step 12 -> Give caption to the window
    XStoreName(display, window, "Prathamesh Paropkari - Graph Paper All Shapes");

    //step 13 - show map window

    XMapWindow(display, window);

    screenWidth = XWidthOfScreen(XScreenOfDisplay(display, visualInfo->screen));
    screenHeight = XHeightOfScreen(XScreenOfDisplay(display, visualInfo->screen));
    XMoveWindow(display, window, (screenWidth - WIN_WIDTH)/2, (screenHeight - WIN_HEIGHT)/2);

    

    //OpenGL Initialization

    initialize();

    // Step 14 -> Event Loop

    while(bDone==False)
    {
        while(XPending(display))
        {
            XNextEvent(display, &event);
            switch(event.type)
            {
               case KeyPress:
               keySym = XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0);

               switch(keySym)
               {
                   case XK_Escape:
                   bDone = True;
                   break;

                   default:
                   break;
                    
               }

                XLookupString(&event.xkey, keys, sizeof(keys), NULL, NULL);
                switch(keys[0])
                {
                    case 'F':
                    case 'f':
                    if(bFullscreen == False)
                    {
                       toggleFullscreen();
                       bFullscreen = True;
                    }
                    else
                    {
                        toggleFullscreen();
                        bFullscreen = False;
                    }
                    break;
                    case 'g':
	                case 'G':
		            if (enableGraph == False)
		            {
			
			            enableGraph = True;
			            enableCircle = False;
			            enableSquare = False;
			            enableTriangle = False;
		            }
		            else
		            {
			
			            enableGraph = False;
		            }
		            break;

                    case 'T':
	                case 't':
		            if (enableTriangle == False)
		            {

			            enableTriangle = True;
			            enableSquare = False;
			            enableCircle = False;
		            }
		            else
		            {

			            enableTriangle = False;
		            }
		            break;

                    case 's':
                    case 'S':
                        if (enableSquare == False)
                        {

                            enableSquare = True;
                            enableTriangle = False;
                            enableCircle = False;
                        }
                        else
                        {

                            enableSquare = False;
                        }
                        break;

                        case 'c':
                        case 'C':
                            if (enableCircle == False)
                            {

                                enableCircle = True;
                                enableTriangle = False;
                                enableSquare = False;
                            }
                            else
                            {

                                enableCircle = False;
                            }
                            break;
                        default:
                            break;
                }
               break;

               case MapNotify:
              
               break;

               case FocusIn:
               bActiveWindow = True;
               
               break;

               case FocusOut:
               bActiveWindow = False;
               
               break;

               case ConfigureNotify:
               resize(event.xconfigure.width,event.xconfigure.height);
               
               break;

               

               case ButtonPress:
               switch(event.xbutton.button)
               {
                   
                  
                   default:
                   break;
                }

                break;

               case 33:
               bDone = True;
               break;

               default:
               break;
            }
        }
        
        //rendering

        if(bActiveWindow == True)
        {
            draw();

            //update function

            update();
        }


    }

    
    

    printf("End of the program\n");

    uninitialize();

    return(0);

}

void toggleFullscreen(void)
{
    // local variable declarations
    Atom windowManagerStateNormal;
    Atom windowManagerStateFullscreen;
    XEvent event;

    // code
    windowManagerStateNormal = XInternAtom(display, "_NET_WM_STATE", False);

    windowManagerStateFullscreen = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);

    // memset the event structure and fill it with above two atoms...
    memset((void*)&event, 0, sizeof(XEvent));

    event.type = ClientMessage;
    event.xclient.window = window;
    event.xclient.message_type = windowManagerStateNormal;
    event.xclient.format = 32;
    event.xclient.data.l[0] = bFullscreen?0:1;
    event.xclient.data.l[1] = windowManagerStateFullscreen;

    // step 4 send event
    XSendEvent(display,XRootWindow(display, visualInfo->screen),False,SubstructureNotifyMask,&event);


}

int initialize(void)
{

    //variable declaration
	
	GLfloat upperX = 0.0f;
	GLfloat lowerX = 0.0f;
	GLfloat upperY = 1.0f;
	GLfloat lowerY = -1.0f;


    //function declaration

    void uninitialize(void);
    void resize(int,int);
    void printGLInfo(void);
    //local variables

    int attribs_new[] = {GLX_CONTEXT_MAJOR_VERSION_ARB,4,
                         GLX_CONTEXT_MINOR_VERSION_ARB,6,
                         GLX_CONTEXT_PROFILE_MASK_ARB,GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
                         None
                        };

    int attribs_old[] = {GLX_CONTEXT_MAJOR_VERSION_ARB,1,
                         GLX_CONTEXT_MINOR_VERSION_ARB,0,
                         None
                        };

    //create OpenGL Context
    

    //get the address of function in function pointer

    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddress((GLubyte*)"glXCreateContextAttribsARB");

    if(glXCreateContextAttribsARB == NULL)
    {
        fprintf(gpFile,"Failed to get required function address\n\n");
        uninitialize();
        exit(-2);
    }

    //create PP compatible GLX context

    glxContext = glXCreateContextAttribsARB(display,glxFBConfig,0,True,attribs_new);

    if(!glxContext)
    {
        fprintf(gpFile,"Core Profile based GLX Context cannot be obtained.\n falling back to old Context\n");

        //getting old context

        glxContext = glXCreateContextAttribsARB(display,glxFBConfig,0,True,attribs_old);

        if(!glxContext)
        {
            fprintf(gpFile,"Old glx Context cannot be found\n");
            uninitialize();
            exit(-3);
        }

        else
        {
            fprintf(gpFile,"Old GLX create context found\n\n");
        }
    }

    else
    {
        fprintf(gpFile,"Core Profile based GLX context obtained successfully\n\n");
    }

    //is the context supports direct rendering

    if(!glXIsDirect(display,glxContext))
    {
        fprintf(gpFile,"Not supporting hardware rendering\n\n");
    }

    else
    {
        fprintf(gpFile, "supporting hardware rendering\n\n");
    }


    //make this context as current context

    if(glXMakeCurrent(display,window,glxContext) == False)
    {
        printf("glXMakeCurrent failed !!!");
        uninitialize();
        exit(-1);
    }

    //initialize GLEW used for PP

	if (glewInit() != GLEW_OK)
	{
		fprintf(gpFile, "glewInit() failed , failed to initialize glew !!\n");
		return(-6);
	}

    //print GL info

	printGLInfo();

   //vertex shader

	const GLchar* vertexShaderSourceCode =
		"#version 460 core\n"             // (version of GLSL * 100) core means you are using core profile i.e PP
		"\n"
		"in vec4 aPosition;\n"
		"out vec4 oColor;\n"
		"vec2 position = aPosition.xy;"
		"uniform mat4 uMVPMatrix;\n"          //u for uniform datatype
		"void main(void)\n"
		"{\n"
		   "gl_Position=uMVPMatrix * vec4(position,0.0,1.0);\n"
		   "if(gl_VertexID < 2)\n"
		   "{\n"
		       "oColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
		   "}\n"
		   "else if(gl_VertexID >= 2 && gl_VertexID <= 3 )\n"
		   "{\n"
		       "oColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
		   "}\n"
		   "else\n"
		   "{\n"
		       "oColor = vec4(0.0, 0.0, 1.0, 1.0);\n"
		   "}\n"
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
		"in vec4 oColor;"
		"out vec4 FragColor;\n" 
		"void main(void)\n" 
		"{\n" 
		"FragColor = oColor;\n"                       //vec4 is working as a constructor here not as a datatype
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
	 //glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_POSITION, "aColor");
	

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

	 /*GLfloat linePositions[] =
	 {
		 0.0f,1.0f,
		 0.0,-1.0f,
		 -1.0f,0.0f,
		 1.0f,0.0f
	 };*/

	 linePositions ={
		0.0f,1.0f,
		0.0,-1.0f,
		-1.0f,0.0f,
		1.0f,0.0f
	 };

	 for (int i = 0; i <= 39; i++)   //loop for +x vertical lines
	 {
		 upperX = upperX + 0.025f;
		 lowerX = lowerX + 0.025f;

		 linePositions.push_back(upperX);

		 linePositions.push_back(upperY);

		 linePositions.push_back(lowerX);

		 linePositions.push_back(lowerY);

		 
	 }

	 upperX = 0.0f;
	 lowerX = 0.0f;

	 for (int i = 0; i <= 39; i++)   //loop for -X vertical lines
	 {
		 upperX = upperX - 0.025f;
		 lowerX = lowerX - 0.025f;

		 linePositions.push_back(upperX);

		 linePositions.push_back(upperY);

		 linePositions.push_back(lowerX);

		 linePositions.push_back(lowerY);


	 }

	 GLfloat leftX = -1.0f;
	 GLfloat rightX = 1.0f;
	 
	 GLfloat leftY = 0.0f;
	 GLfloat rightY = 0.0f;

	 for (int i = 0; i <= 39; i++)    //loop for +y vertical lines
	 {
		 leftY = leftY + 0.025f;
		 rightY = rightY + 0.025f;

		 linePositions.push_back(leftX);

		 linePositions.push_back(leftY);

		 linePositions.push_back(rightX);

		 linePositions.push_back(rightY);


	 }

	  leftX = -1.0f;
	  rightX = 1.0f;

	  leftY = 0.0f;
	  rightY = 0.0f;

	 for (int i = 0; i <= 39; i++)    //loop for -y vertical lines
	 {
		 leftY = leftY - 0.025f;
		 rightY = rightY - 0.025f;

		 linePositions.push_back(leftX);

		 linePositions.push_back(leftY);

		 linePositions.push_back(rightX);

		 linePositions.push_back(rightY);


	 }

	 
 
	 

	 ////step 13: Create vertex array object
	 //VAO

	 glGenVertexArrays(1, &vao);

	 //step 14: Bind with VAO

	 glBindVertexArray(vao);

	 //step 15: Create vertex buffer object (VBO) for position
	 
	 //VBO for position

	 glGenBuffers(1, &vbo_Position);

	 //step 16: Bind with VBO of position

	 glBindBuffer(GL_ARRAY_BUFFER, vbo_Position);

	 glBufferData(GL_ARRAY_BUFFER, linePositions.size() * sizeof(GLfloat), linePositions.data(), GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	 glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);


	 

	
	 //step 18 : unbind with VAO
	 glBindVertexArray(0);

//************************************** shader for shape handeling *************************************


	 const GLchar* vertexShaderSourceCode_Shapes =
		 "#version 460 core\n"
		 "in vec4 inputPosition;\n"
		 "in vec4 inputColor;\n"
		 "out vec4 FragColor;\n"
		 "vec2 position = inputPosition.xy;"
		 "uniform mat4 uMVPMatrixShapes;"
		 "void main(void)\n"
		 "{\n"
		   "gl_Position = uMVPMatrixShapes * vec4(position, 0.0, 1.0);\n"
		   "FragColor = inputColor;\n"
		 "}\n";

		 //step 2: Create vertex shader object

	GLuint vertexShaderObject_Shapes = glCreateShader(GL_VERTEX_SHADER);

	 //step 3 : Give vertex shader code to OpenGL
	 glShaderSource(vertexShaderObject_Shapes, 1, (const GLchar**)&vertexShaderSourceCode_Shapes, NULL);

	 //Step 4 : Let OpenGL engine Compile the vertex shader code
	 glCompileShader(vertexShaderObject_Shapes);

	 status = 0;
	 infoLogLength = 0;
	 szInfoLog = NULL;

	 //step 5 : Check for vertex shader compilation
	 //step 5 (a) :  get compile status in status variable
	 glGetShaderiv(vertexShaderObject_Shapes, GL_COMPILE_STATUS, &status);

	 //step 5 (b) : if error occur i.e status == GL_FALSE,  get length of error message into infoLogLength variable
	 if (status == GL_FALSE)
	 {

		 glGetShaderiv(vertexShaderObject_Shapes, GL_INFO_LOG_LENGTH, &infoLogLength);

		 if (infoLogLength > 0)
		 {
			 //step 5 (c) : declare a string variable allocate memory equal to above length i.e length of error message
			 szInfoLog = (GLchar*)malloc(infoLogLength);


			 if (szInfoLog != NULL)
			 {
				 // step 5 (d) : get the compilation error log info into memory allocated string
				 glGetShaderInfoLog(vertexShaderObject_Shapes, infoLogLength, NULL, szInfoLog);

				 //step 5 (e) : print error message into log file
				 fprintf(gpFile, "Vertex shader for shapes compilation error log : %s\n", szInfoLog);


				 //step 5 (f) : free allocated memory
				 free(szInfoLog);
				 szInfoLog = NULL;


				 //step 5 (g) : uninitialize and exit.


			 }

		 }

		 uninitialize();
	 }

	 const GLchar* fragmentShaderSourceCode_Shapes =
		 "#version 460 core\n"
		 "in vec4 FragColor;\n"
		 "out vec4 outputColor;\n"
		 "void main(void)\n"
		 "{\n"
		 "outputColor = FragColor;\n"
		 "}\n";

	 GLuint fragmentShaderObject_Shapes = glCreateShader(GL_FRAGMENT_SHADER);

	 glShaderSource(fragmentShaderObject_Shapes, 1, (const GLchar**)&fragmentShaderSourceCode_Shapes, NULL);

	 glCompileShader(fragmentShaderObject_Shapes);

	 status = 0;
	 infoLogLength = 0;
	 szInfoLog = NULL;

	 glGetShaderiv(fragmentShaderObject_Shapes, GL_COMPILE_STATUS, &status);

	 if (status == GL_FALSE)
	 {

		 glGetShaderiv(fragmentShaderObject_Shapes, GL_INFO_LOG_LENGTH, &infoLogLength);

		 if (infoLogLength > 0)
		 {
			 szInfoLog = (GLchar*)malloc(infoLogLength);


			 if (szInfoLog != NULL)
			 {
				 glGetShaderInfoLog(fragmentShaderObject_Shapes, infoLogLength, NULL, szInfoLog);
				 fprintf(gpFile, "Fragment shader for shapes compilation error log : %s\n", szInfoLog);
				 free(szInfoLog);
				 szInfoLog = NULL;


			 }



		 }

		 uninitialize();
	 }

	 //linking process for shader program.

	 // shader program

	 //step 7 : create shader program object

	 shaderProgramObject_Shapes = glCreateProgram();

	 //step 8 : Attach both shaders into this program            also know as prelinking binding  should be done before linking and after attachment
	 glAttachShader(shaderProgramObject_Shapes, vertexShaderObject_Shapes);
	 glAttachShader(shaderProgramObject_Shapes, fragmentShaderObject_Shapes);

	 //step 9: bind attribute location to the shader program object

	 glBindAttribLocation(shaderProgramObject_Shapes, AMC_ATTRIBUTE_POSITION_SHAPES, "inputPosition");
	 glBindAttribLocation(shaderProgramObject_Shapes, AMC_ATTRIBUTE_COLOR_SHAPES, "inputColor");


	 //step 10: Link the shader program

	 glLinkProgram(shaderProgramObject_Shapes);

	 //step 11: do linking error check fpr shader program

	 status = 0;
	 infoLogLength = 0;
	 szInfoLog = NULL;

	 glGetProgramiv(shaderProgramObject_Shapes, GL_LINK_STATUS, &status);

	 if (status == GL_FALSE)
	 {
		 glGetProgramiv(shaderProgramObject_Shapes, GL_INFO_LOG_LENGTH, &infoLogLength);

		 if (infoLogLength > 0)
		 {
			 szInfoLog = (GLchar*)malloc(infoLogLength);


			 if (szInfoLog != NULL)
			 {
				 glGetProgramInfoLog(shaderProgramObject_Shapes, infoLogLength, NULL, szInfoLog);
				 fprintf(gpFile, "shader program for shapes linking error log : %s\n", szInfoLog);
				 free(szInfoLog);
				 szInfoLog = NULL;


			 }



		 }

		 uninitialize();
	 }

	 //step 12: Declare position and color arrays 

	 //get Shader uniform Locations

	 mvpMatrixUniform_Shapes = glGetUniformLocation(shaderProgramObject_Shapes, "uMVPMatrixShapes");  //must be done after linkage of shader program



	 const GLfloat trianglePosition[] =
	 {
		0.0f,0.5f,  //top vertex
		-0.5f,-0.5f, //left vertex

		-0.5f,-0.5f, //left vertex
		0.5f,-0.5f, //right vertex

		0.5f,-0.5f, //right vertex
		0.0f,0.5f,  //top vertex

	 };

	 const GLfloat triangleColor[] =
	 {
		 1.0f,1.0f,0.0f,
		 1.0f,1.0f,0.0f,
		 1.0f,1.0f,0.0f,
		 1.0f,1.0f,0.0f,
		 1.0f,1.0f,0.0f,
		 1.0f,1.0f,0.0f
	 };

	 const GLfloat squarePosition[] =
	 {
		 0.5f,0.5f, //right top
		 -0.5f,0.5f, //left top

		 -0.5f,0.5f, //left top
		 -0.5f,-0.5f, //left bottom

		  -0.5f,-0.5f, //left bottom
		 0.5f,-0.5f,   //right bottom

		  0.5f,-0.5f,   //right bottom
		  0.5f,0.5f     //right top


	 };

	 const GLfloat squareColor[] =
	 {
		 1.0f,1.0f,0.0f,
		 1.0f,1.0f,0.0f,
		 1.0f,1.0f,0.0f,
		 1.0f,1.0f,0.0f,
		 1.0f,1.0f,0.0f,
		 1.0f,1.0f,0.0f,
		 1.0f,1.0f,0.0f,
		 1.0f,1.0f,0.0f
	 };

	 //code to find circle positions

	 GLfloat radious = 0.5f;

	 for (GLfloat angle = 0.0f; angle <= 360.0f; angle = angle + 0.01f)
	 {

		 //float radianangle = angle * (M_PI * 180.0f);
		 GLfloat x = radious * cos((angle * M_PI) / 180.0f) + (0.0f);
		 GLfloat y = radious * sin((angle * M_PI) / 180.0f) + (0.0f);

		 circlePositions.push_back(x);
		 circlePositions.push_back(y);


	 }

	 for (int i = 0; i < circlePositions.size(); i++)
	 {
		 CircleColor.push_back(1.0f);
		 CircleColor.push_back(1.0f);
		 CircleColor.push_back(0.0f);
	 }

	 ////step 13: Create vertex array object
	//VAO

	 glGenVertexArrays(1, &vao_Triangle);

	 //step 14: Bind with VAO

	 glBindVertexArray(vao_Triangle);

	 //step 15: Create vertex buffer object (VBO) for position

	 //VBO for position

	 glGenBuffers(1, &vbo_Position_Triangle);

	 //step 16: Bind with VBO of position

	 glBindBuffer(GL_ARRAY_BUFFER, vbo_Position_Triangle);

	 glBufferData(GL_ARRAY_BUFFER, sizeof(trianglePosition), trianglePosition, GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_POSITION_SHAPES, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	 glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION_SHAPES);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);

	 //vbo for color

	 glGenBuffers(1, &vbo_Color_Triangle);

	 //step 16: Bind with VBO of position

	 glBindBuffer(GL_ARRAY_BUFFER, vbo_Color_Triangle);

	 glBufferData(GL_ARRAY_BUFFER, sizeof(triangleColor), triangleColor, GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_COLOR_SHAPES, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	 glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR_SHAPES);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);





	 //step 18 : unbind with VAO
	 glBindVertexArray(0);

	 //vao for square
	  ////step 13: Create vertex array object
	//VAO

	 glGenVertexArrays(1, &vao_Square);

	 //step 14: Bind with VAO

	 glBindVertexArray(vao_Square);

	 //step 15: Create vertex buffer object (VBO) for position

	 //VBO for position

	 glGenBuffers(1, &vbo_Position_Square);

	 //step 16: Bind with VBO of position

	 glBindBuffer(GL_ARRAY_BUFFER, vbo_Position_Square);

	 glBufferData(GL_ARRAY_BUFFER, sizeof(squarePosition), squarePosition, GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_POSITION_SHAPES, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	 glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION_SHAPES);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);

	 //vbo for color

	 glGenBuffers(1, &vbo_Color_Square);

	 //step 16: Bind with VBO of position

	 glBindBuffer(GL_ARRAY_BUFFER, vbo_Color_Square);

	 glBufferData(GL_ARRAY_BUFFER, sizeof(squareColor), squareColor, GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_COLOR_SHAPES, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	 glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR_SHAPES);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);





	 //step 18 : unbind with VAO
	 glBindVertexArray(0);

	 //VAO for circle

	 glGenVertexArrays(1, &vao_Circle);

	 //step 14: Bind with VAO

	 glBindVertexArray(vao_Circle);

	 //step 15: Create vertex buffer object (VBO) for position

	 //VBO for position

	 glGenBuffers(1, &vbo_Position_Circle);

	 //step 16: Bind with VBO of position

	 glBindBuffer(GL_ARRAY_BUFFER, vbo_Position_Circle);

	 glBufferData(GL_ARRAY_BUFFER, circlePositions.size()*sizeof(float), circlePositions.data(), GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_POSITION_SHAPES, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	 glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION_SHAPES);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);

	 //vbo for color

	 glGenBuffers(1, &vbo_Color_Circle);

	 //step 16: Bind with VBO of position

	 glBindBuffer(GL_ARRAY_BUFFER, vbo_Color_Circle);

	 glBufferData(GL_ARRAY_BUFFER, CircleColor.size() * sizeof(float), CircleColor.data(), GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_COLOR_SHAPES, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	 glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR_SHAPES);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);





	 //step 18 : unbind with VAO
	 glBindVertexArray(0);





	// 2nd change for depth or Enabling depth
	
	glClearDepth(1.0f); // Compulsory
	glEnable(GL_DEPTH_TEST); // 8 test paike ek(depth) test enable kr
	glDepthFunc(GL_LEQUAL); 
	


	// Set The ClearColor Of Window To Blue
	// Clear OpenGl Stars
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Blue Color

	//warmup resize

	//initialize orthographic projection matrix

	perspectiveProjectionMatrix = vmath::mat4::identity();
	resize(WIN_WIDTH, WIN_HEIGHT);
	

    return(True);

	//code

}

void resize(int width ,int height)
{
    // code
	if(height <= 0)
    {
        height = 1;
    }
		

	
	//glMatrixMode(GL_PROJECTION); //not allowed in PP 
	//glLoadIdentity();            not allowed in PP 

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	//set Perspective projection matrix

	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	
    

   
}


void draw(void)
{
    //code
	// 3rd Change GL_DEPTH_BUFFER_BIT
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	GLint lineCounter = 0;
	

	//PP steps for display function

	//step 1: Use shader program object

	

	glUseProgram(shaderProgramObject);

	//transformations

	mat4 modelViewMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

	mat4 madelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;  //order of multiplication is not commutativ. i.e mat1 * mat2 != mat2 * mat1

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, madelViewProjectionMatrix);

	glBindVertexArray(vao);

	//step 3: Draw the geometry

	if (enableGraph == True)
	{
		glLineWidth(3.5f);

		glDrawArrays(GL_LINES, 0, 4);

		glLineWidth(1.0f);

		int offset = 0;
		int fun = 1;

		for (int i = 0; i < 80; i++)
		{
			if ((fun % 5) == 0)
			{
				glLineWidth(3.0f);
			}
			else
			{
				glLineWidth(1.0f);
			}
			glDrawArrays(GL_LINES, 4 + offset, 2);
			fun += 1;
			offset += 2;
		}

		//glDrawArrays(GL_LINES, 4, 80);  // right side vertical lines

		offset = 0;
		fun = 1;

		for (int i = 0; i < 80; i++)
		{
			if ((fun % 5) == 0)
			{
				glLineWidth(3.0f);
			}
			else
			{
				glLineWidth(1.0f);
			}
			glDrawArrays(GL_LINES, 84 + offset, 2);
			fun += 1;
			offset += 2;
		}

		//glDrawArrays(GL_LINES, 84, 80); // left side vertical lines

		offset = 0;
		fun = 1;

		for (int i = 0; i < 80; i++)
		{
			if ((fun % 5) == 0)
			{
				glLineWidth(3.0f);
			}
			else
			{
				glLineWidth(1.0f);
			}
			glDrawArrays(GL_LINES, 164 + offset, 2);
			fun += 1;
			offset += 2;
		}

		//glDrawArrays(GL_LINES, 164, 80); //upper horizonta lines

		offset = 0;
		fun = 1;

		for (int i = 0; i < 80; i++)
		{
			if ((fun % 5) == 0)
			{
				glLineWidth(3.0f);
			}
			else
			{
				glLineWidth(1.0f);
			}
			glDrawArrays(GL_LINES, 244 + offset, 2);
			fun += 1;
			offset += 2;
		}
		//glDrawArrays(GL_LINES, 244, 80); // lower horizontal lines

		glBindVertexArray(0);

		if (enableTriangle == True)
		{
			glUseProgram(shaderProgramObject_Shapes);

			mat4 modelViewMatrixShapes = mat4::identity();

			mat4 madelViewProjectionMatrixShapes = mat4::identity();

			modelViewMatrixShapes = vmath::translate(0.0f, 0.0f, -2.5f);

			madelViewProjectionMatrixShapes = perspectiveProjectionMatrix * modelViewMatrixShapes;  //order of multiplication is not commutativ. i.e mat1 * mat2 != mat2 * mat1

			//push above mvp into vertex shader's mvp uniform

			glUniformMatrix4fv(mvpMatrixUniform_Shapes, 1, GL_FALSE, madelViewProjectionMatrixShapes);

			glBindVertexArray(vao_Triangle);

			glDrawArrays(GL_LINES, 0, 6);

			glBindVertexArray(0);

			glUseProgram(0);


		}

		if (enableSquare == True)
		{
			glUseProgram(shaderProgramObject_Shapes);

			mat4 modelViewMatrixShapes = mat4::identity();

			mat4 madelViewProjectionMatrixShapes = mat4::identity();

			modelViewMatrixShapes = vmath::translate(0.0f, 0.0f, -2.5f);

			madelViewProjectionMatrixShapes = perspectiveProjectionMatrix * modelViewMatrixShapes;  //order of multiplication is not commutativ. i.e mat1 * mat2 != mat2 * mat1

			//push above mvp into vertex shader's mvp uniform

			glUniformMatrix4fv(mvpMatrixUniform_Shapes, 1, GL_FALSE, madelViewProjectionMatrixShapes);

			glBindVertexArray(vao_Square);

			glDrawArrays(GL_LINES, 0, 8);

			glBindVertexArray(0);

			glUseProgram(0);
		}

		if (enableCircle == True)
		{
			glUseProgram(shaderProgramObject_Shapes);

			mat4 modelViewMatrixShapes = mat4::identity();

			mat4 madelViewProjectionMatrixShapes = mat4::identity();

			modelViewMatrixShapes = vmath::translate(0.0f, 0.0f, -2.5f);

			madelViewProjectionMatrixShapes = perspectiveProjectionMatrix * modelViewMatrixShapes;  //order of multiplication is not commutativ. i.e mat1 * mat2 != mat2 * mat1

			//push above mvp into vertex shader's mvp uniform

			glUniformMatrix4fv(mvpMatrixUniform_Shapes, 1, GL_FALSE, madelViewProjectionMatrixShapes);

			glBindVertexArray(vao_Circle);

			glPointSize(2.0f);

			glDrawArrays(GL_POINTS, 0, (circlePositions.size()/2));

			glPointSize(1.0f);

			glBindVertexArray(0);

			glUseProgram(0);
		}
	}

	else
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}

	
	

	



	
	//step 4: unBind with (VAO)

	glBindVertexArray(0);

	//step 5: Unuse shader program object
	glUseProgram(0);


    glXSwapBuffers(display,window);
}

void update(void)
{
    //code

    


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

	

	//step 10 : delete vbo for position

	if (vbo_Position)
	{
		glDeleteBuffers(1, &vbo_Position);
		vbo_Position = 0;
	}

	//step 11 : delete VAO 

	if (vao)
	{
		glDeleteVertexArrays(1, &vao);
		vao = 0;
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













