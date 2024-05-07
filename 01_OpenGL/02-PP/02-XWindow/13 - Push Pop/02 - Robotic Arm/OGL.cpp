// standard header file
#include<stdio.h>
#include<stdlib.h> // for exit
#include<memory.h> // for memset

//OpenGL Header files
#include<GL/glew.h>
#include<GL/glx.h>  
#include<GL/gl.h>
#include "Sphere.h"


// X-11 header files
#include<X11/Xlib.h> // for all X-Window API
#include<X11/Xutil.h>  // for XVisualInfo and related API
#include<X11/XKBlib.h> 
#include"vmath.h"

using namespace vmath;

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define MODEL_VIEW_MATRIX_STACK 32

void initializeMatrixStack(void);
void pushMatrix(mat4);
mat4 popMatrix(void);

//PushPop Related variables

mat4 matrixStack[MODEL_VIEW_MATRIX_STACK];
int matrixStackTop = -1;


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

GLuint numSphereElements = 0;

enum
{
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_NORMAL,
	AMC_ATTRIBUTE_TEXCOORDS

};

//GLuint vaoPyramid = 0;
GLuint vaoSphere = 0;

//GLuint vbo_Position_Pyramid = 0;
//GLuint vbo_Color_Pyramid = 0;

GLuint vbo_Position_Sphere = 0;
GLuint vbo_Normal_Sphere = 0;
GLuint vbo_Element_Sphere = 0;
GLuint vbo_Texture_Sphere = 0;


//GLuint uMVPMatrix;

mat4 perspectiveProjectionMatrix;   //using 4*4 matrix.
                                   //mat4 - represents 4*4 matrix.   
								   //mat4 is not c++ datattype, it is defined in vmath.h.

GLuint modelMatrixUniform = 0;
GLuint viewMatrixUniform = 0;
GLuint projectionMatrixUniform = 0;
GLuint modelViewMatrixUniform = 0;
GLuint lightDiffuseUniform = 0;

GLuint lightPositionUniform = 0;
GLuint lightAmbientUniform = 0;
GLuint lightSpecularUniform = 0;


GLuint materialAmbientUniform = 0;
GLuint materialDiffuseUniform = 0;
GLuint materialSpecularUniform = 0;
GLuint materialShininessUniform = 0;

GLuint keyPressUniform = 0;
GLuint mvpMatrixUniform = 0;

Bool bLightingEnabled = False;


GLfloat lightAmbient[] = { 0.1f,0.1f,0.1f,1.0f };
GLfloat lightDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightPosition[] = { 100.0f,100.0f,100.0f,1.0f };

GLfloat materialAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat materialDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialShininess = 50.0f;

// //PushPop Related variables

// mat4 matrixStack[MODEL_VIEW_MATRIX_STACK];
// int matrixStackTop = -1;

//robotic arm variables

int shoulder = 0;
int elbow = 0;
int wrist = 0;



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

    printf("before creating log file");

    gpFile = fopen("log.txt","w");

    printf("after creating log file");

    if(gpFile == NULL)
    {
        printf("fail to create log file\n");
        exit(0);
    }

    else
    {
        
        fprintf(gpFile,"Log File created successfully !!!\n\n");
    }

     printf("after else");

    
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
    XStoreName(display, window, "Prathamesh Paropkari - Robotic Arm\n");

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

                    case'S':
                        shoulder = (shoulder + 3) % 360;
                        break;
                    case 's':
                        shoulder = (shoulder - 3) % 360;
                        break;
                    case 'E':
                        elbow = (elbow + 3) % 360;
                        break;

                    case 'e':
                        elbow = (elbow - 3) % 360;
                        break;

                    case 'W':
                        wrist = (wrist + 3) % 360;
                        break;

                    case 'w':
                        wrist = (wrist - 3) % 360;
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

    //function declaration
    void printGLInfo(void);
    void uninitialize(void);
    void resize(int,int);
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

    fprintf(gpFile,"Reached function call");

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

    if (glewInit() != GLEW_OK)
	{
		fprintf(gpFile, "glewInit() failed , failed to initialize glew !!\n");
        uninitialize();
		return(-6);
	}



    //print GL info

	printGLInfo();

	//Steps to follow in initialize function for PP

	// step 1 : write vertex shader code
	
	//vertex shader

	const GLchar* vertexShaderSourceCode =
		"#version 460 core\n"             // (version of GLSL * 100) core means you are using core profile i.e PP
		"\n"
		"in vec4 aPosition;\n"
		"uniform mat4 uMVPMatrix;\n"
		"void main(void)\n"
		"{\n"
		"gl_Position=uMVPMatrix*aPosition;\n"
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
		"out vec4 FragColor;\n"
		"void main(void)\n"
		"{\n"
		"FragColor= vec4(0.8, 0.6, 0.4, 1.0);\n"
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



	
		

	 modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "uModelMatrix");

	 viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "uViewMatrix");

	 projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "uProjectionMatrix");

	 lightAmbientUniform = glGetUniformLocation(shaderProgramObject, "uLightAmbient");

	 lightDiffuseUniform = glGetUniformLocation(shaderProgramObject, "uLightDiffuse");

	 lightSpecularUniform = glGetUniformLocation(shaderProgramObject, "uLightSpecular");

	 lightPositionUniform = glGetUniformLocation(shaderProgramObject, "uLightPosition");

	 materialAmbientUniform = glGetUniformLocation(shaderProgramObject, "uMaterialAmbient");

	 materialDiffuseUniform = glGetUniformLocation(shaderProgramObject, "uMaterialDiffuse");

	 materialSpecularUniform = glGetUniformLocation(shaderProgramObject, "uMaterialSpecular");

	 materialShininessUniform = glGetUniformLocation(shaderProgramObject, "uMaterialShininess");
		 

	 keyPressUniform = glGetUniformLocation(shaderProgramObject, "uKeyPress");


	
	 float sphere_Positions[1146];
	 float sphere_normals[1146];
	 float sphere_Texcoords[764];
	 unsigned short sphere_elements[2280];

	 
	 getSphereVertexData(sphere_Positions, sphere_normals, sphere_Texcoords, sphere_elements);
	 
	 numSphereElements = getNumberOfSphereElements();

	 ////step 13: Create vertex array object
	 //VAO

	

	 // ********************************************** for sphere ***********************************************

	 

	 glGenVertexArrays(1, &vaoSphere);

	 //step 14: Bind with VAO

	 glBindVertexArray(vaoSphere);

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

	

	 //although we are not gonna use texture in this program, vreating this VBO for future use if needed

	 //vbo for texture


	 glGenBuffers(1, &vbo_Texture_Sphere);

	 glBindBuffer(GL_ARRAY_BUFFER, vbo_Texture_Sphere);

	 glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_Texcoords), sphere_Texcoords, GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	 glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORDS);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);

	 //VBO for elements

	 glGenBuffers(1, &vbo_Element_Sphere);
	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_Element_Sphere);
	 glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);

	 // shader madhe attribute madhun elements ghet nahit mhanun  glVertexAttribPointer() ani glEnableVertexAttribArray() function call nahi ahe

	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	 //step 18 : unbind with VAO
	 glBindVertexArray(0);


	// 2nd change for depth or Enabling depth
	
	glClearDepth(1.0f); // Compulsory
	glEnable(GL_DEPTH_TEST); // 8 test paike ek(depth) test enable kr
	glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); 
	


	// Set The ClearColor Of Window To Blue
	// Clear OpenGl Stars
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Blue Color

	perspectiveProjectionMatrix = vmath::mat4::identity();

    initializeMatrixStack();

	resize(WIN_WIDTH, WIN_HEIGHT);

    return(0);

}

void resize(int width ,int height)
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


void draw(void)
{
    
	//code
	// 3rd Change GL_DEPTH_BUFFER_BIT
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	//PP steps for display function

	//step 1: Use shader program object

	glUseProgram(shaderProgramObject);

	

	
	
	//transformations

	 mat4 modelViewMatrix = mat4::identity();

	 mat4 viewMatrix = mat4::identity();


	 mat4 translationMatrix = mat4::identity();
	 mat4 rotationMatrix = mat4::identity();
	 mat4 scaleMatrix = mat4::identity();

	 mat4 modelMatrix = mat4::identity();

	 mat4 modelViewProjectionMatrix = mat4::identity();

	 translationMatrix = vmath::translate(0.0f, 0.0f, -12.0f);

	 modelMatrix = translationMatrix;

	pushMatrix(modelMatrix);
	{
		rotationMatrix = mat4::identity();
		rotationMatrix = vmath::rotate((GLfloat)shoulder, 0.0f, 0.0f, 1.0f);

		translationMatrix = mat4::identity();
		translationMatrix = vmath::translate(1.0f, 0.0f, 0.0f);

		modelMatrix = modelMatrix * rotationMatrix * translationMatrix;

		pushMatrix(modelMatrix);
		{
			
			scaleMatrix = vmath::scale(2.0f, 0.5f, 1.0f);
			modelMatrix = modelMatrix * scaleMatrix;

			modelViewProjectionMatrix = perspectiveProjectionMatrix * viewMatrix * modelMatrix;

			//modelViewMatrix = modelMatrix * viewMatrix;

			//draw sphere

			//push above mvp into vertex shader's mvp uniform
			glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
			

			//glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

			


			

			//step 2: Bind with (VAO)

			glBindVertexArray(vaoSphere);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_Element_Sphere);

			//step 3: Draw the geometry

			glDrawElements(GL_TRIANGLES, numSphereElements, GL_UNSIGNED_SHORT, 0);

			//step 4: unBind with (VAO)




			glBindVertexArray(0);
		}
		modelMatrix = popMatrix();

		//do transformations for forearm in CTM

		translationMatrix = mat4::identity();
		translationMatrix = vmath::translate(1.0f, 0.0f, 0.0f);

		rotationMatrix = mat4::identity();
		rotationMatrix = vmath::rotate((GLfloat)elbow, 0.0f, 0.0f, 1.0f);

		modelMatrix = modelMatrix * translationMatrix * rotationMatrix;

		translationMatrix = mat4::identity();
		
		translationMatrix = vmath::translate(1.05f, 0.0f, 0.0f);

		modelMatrix = modelMatrix * translationMatrix;

		//draw arm

		pushMatrix(modelMatrix);
		{

			scaleMatrix = vmath::scale(2.0f, 0.5f, 1.0f);
			modelMatrix = modelMatrix * scaleMatrix;

			modelViewProjectionMatrix = perspectiveProjectionMatrix * viewMatrix * modelMatrix;

			//modelViewMatrix = modelMatrix * viewMatrix;

			//draw sphere

			//push above mvp into vertex shader's mvp uniform
			glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);


			//glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);






			//step 2: Bind with (VAO)

			glBindVertexArray(vaoSphere);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_Element_Sphere);

			//step 3: Draw the geometry

			glDrawElements(GL_TRIANGLES, numSphereElements, GL_UNSIGNED_SHORT, 0);

			//step 4: unBind with (VAO)




			glBindVertexArray(0);
		}

		modelMatrix = popMatrix();
		// do transformation for wrist

		translationMatrix = mat4::identity();

		translationMatrix = vmath::translate(0.95f, 0.0f, 0.0f);

		rotationMatrix = mat4::identity();

		rotationMatrix = vmath::rotate((GLfloat)wrist, 0.0f, 0.0f, 1.0f);

		modelMatrix = modelMatrix * translationMatrix * rotationMatrix;


		translationMatrix = mat4::identity();

		translationMatrix = vmath::translate(0.41f, 0.0f, 0.0f);

		modelMatrix = modelMatrix * translationMatrix;

		// draw wrist

		pushMatrix(modelMatrix);
		{
			//scaleMatrix = mat4::identity();
			scaleMatrix = vmath::scale(0.50f, 0.50f, 1.0f);

			modelMatrix = modelMatrix * scaleMatrix;

			//modelViewMatrix = viewMatrix * modelMatrix;

			modelViewProjectionMatrix = perspectiveProjectionMatrix * viewMatrix * modelMatrix;

			// draw sphere
			glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

			// *** bind vao ***
			glBindVertexArray(vaoSphere);

			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_Element_Sphere);

			glDrawElements(GL_TRIANGLES, numSphereElements, GL_UNSIGNED_SHORT, 0);

			// *** unbind vao ***
			glBindVertexArray(0);
		}
		modelMatrix = popMatrix();

		//draw middle finger

		translationMatrix = mat4::identity();

		translationMatrix = vmath::translate(0.2f, 0.0f, 0.0f);

		rotationMatrix = mat4::identity();

		//rotationMatrix = vmath::rotate((GLfloat)wrist, 0.0f, 0.0f, 1.0f);

		modelMatrix = modelMatrix * translationMatrix * rotationMatrix;


		translationMatrix = mat4::identity();

		translationMatrix = vmath::translate(0.41f, 0.0f, 0.0f);

		modelMatrix = modelMatrix * translationMatrix;

		pushMatrix(modelMatrix);
		{
			//scaleMatrix = mat4::identity();
			scaleMatrix = vmath::scale(0.3f, 0.08f, 0.1f);

			modelMatrix = modelMatrix * scaleMatrix;

			modelViewProjectionMatrix = perspectiveProjectionMatrix * viewMatrix * modelMatrix;

		
			glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

			
			glBindVertexArray(vaoSphere);

			
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_Element_Sphere);

			glDrawElements(GL_TRIANGLES, numSphereElements, GL_UNSIGNED_SHORT, 0);

			
			glBindVertexArray(0);
		}
		modelMatrix = popMatrix();


		//draw index finger

		translationMatrix = mat4::identity();

		translationMatrix = vmath::translate(-0.45f, -0.15f, 0.0f);

		rotationMatrix = mat4::identity();

		//rotationMatrix = vmath::rotate((GLfloat)wrist, 0.0f, 0.0f, 1.0f);

		modelMatrix = modelMatrix * translationMatrix * rotationMatrix;


		translationMatrix = mat4::identity();

		translationMatrix = vmath::translate(0.41f, 0.0f, 0.0f);

		modelMatrix = modelMatrix * translationMatrix;

		pushMatrix(modelMatrix);
		{
			//scaleMatrix = mat4::identity();
			scaleMatrix = vmath::scale(0.3f, 0.08f, 0.1f);

			modelMatrix = modelMatrix * scaleMatrix;

			modelViewProjectionMatrix = perspectiveProjectionMatrix * viewMatrix * modelMatrix;


			glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);


			glBindVertexArray(vaoSphere);


			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_Element_Sphere);

			glDrawElements(GL_TRIANGLES, numSphereElements, GL_UNSIGNED_SHORT, 0);


			glBindVertexArray(0);
		}
		modelMatrix = popMatrix();

		//draw ring finger

		translationMatrix = mat4::identity();

		translationMatrix = vmath::translate(-0.44f, 0.3f, 0.0f);

		//rotationMatrix = mat4::identity();

		//rotationMatrix = vmath::rotate((GLfloat)wrist, 0.0f, 0.0f, 1.0f);

		modelMatrix = modelMatrix * translationMatrix * rotationMatrix;


		translationMatrix = mat4::identity();

		translationMatrix = vmath::translate(0.41f, 0.0f, 0.0f);

		modelMatrix = modelMatrix * translationMatrix;

		pushMatrix(modelMatrix);
		{
			//scaleMatrix = mat4::identity();
			scaleMatrix = vmath::scale(0.3f, 0.08f, 0.1f);

			modelMatrix = modelMatrix * scaleMatrix;

			modelViewProjectionMatrix = perspectiveProjectionMatrix * viewMatrix * modelMatrix;


			glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);


			glBindVertexArray(vaoSphere);


			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_Element_Sphere);

			glDrawElements(GL_TRIANGLES, numSphereElements, GL_UNSIGNED_SHORT, 0);


			glBindVertexArray(0);
		}
		modelMatrix = popMatrix();

		//draw thumb

		translationMatrix = mat4::identity();

		translationMatrix = vmath::translate(-0.5f, -0.43f, 0.0f);

		//rotationMatrix = mat4::identity();

		//rotationMatrix = vmath::rotate((GLfloat)wrist, 0.0f, 0.0f, 1.0f);

		modelMatrix = modelMatrix * translationMatrix * rotationMatrix;


		translationMatrix = mat4::identity();

		translationMatrix = vmath::translate(0.41f, 0.0f, 0.0f);

		modelMatrix = modelMatrix * translationMatrix;

		pushMatrix(modelMatrix);
		{
			//scaleMatrix = mat4::identity();
			scaleMatrix = vmath::scale(0.3f, 0.08f, 0.1f);

			modelMatrix = modelMatrix * scaleMatrix;

			modelViewProjectionMatrix = perspectiveProjectionMatrix * viewMatrix * modelMatrix;


			glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);


			glBindVertexArray(vaoSphere);


			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_Element_Sphere);

			glDrawElements(GL_TRIANGLES, numSphereElements, GL_UNSIGNED_SHORT, 0);


			glBindVertexArray(0);
		}
		modelMatrix = popMatrix();


		//draw little finger

		translationMatrix = mat4::identity();

		translationMatrix = vmath::translate(-0.49f, 0.57f, 0.0f);

		//rotationMatrix = mat4::identity();

		//rotationMatrix = vmath::rotate((GLfloat)wrist, 0.0f, 0.0f, 1.0f);

		modelMatrix = modelMatrix * translationMatrix * rotationMatrix;


		translationMatrix = mat4::identity();

		translationMatrix = vmath::translate(0.41f, 0.0f, 0.0f);

		modelMatrix = modelMatrix * translationMatrix;

		pushMatrix(modelMatrix);
		{
			//scaleMatrix = mat4::identity();
			scaleMatrix = vmath::scale(0.3f, 0.08f, 0.1f);

			modelMatrix = modelMatrix * scaleMatrix;

			modelViewProjectionMatrix = perspectiveProjectionMatrix * viewMatrix * modelMatrix;


			glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);


			glBindVertexArray(vaoSphere);


			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_Element_Sphere);

			glDrawElements(GL_TRIANGLES, numSphereElements, GL_UNSIGNED_SHORT, 0);


			glBindVertexArray(0);
		}
		modelMatrix = popMatrix();



	}
	modelMatrix = popMatrix();


	
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
    //local variable declarations

    GLXContext currentGLXContext=NULL;

    // code

    if(visualInfo)
    {
        free(visualInfo);
        visualInfo = NULL;
    }

    // Uncurrent the context

    currentGLXContext=glXGetCurrentContext();

    if((currentGLXContext != NULL) && (currentGLXContext == glxContext)) 
    {
        glXMakeCurrent(display,0,0);
    }

    if(glxContext)
    {
        glXDestroyContext(display,glxContext);
        glxContext = NULL;
    }

    //Close The Log File
	if (gpFile)
	{
		fprintf(gpFile, "Program Ended Sucessfully...\n");
		fclose(gpFile);
		gpFile = NULL;
	}

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

	if (vaoSphere)
	{
		glDeleteVertexArrays(1, &vaoSphere);
		vaoSphere = 0;
	}



    if(window)
    {
        XDestroyWindow(display, window);
    }

    if(colormap)
    {
        XFreeColormap(display, colormap);
    }

    if(display)
    {
        XCloseDisplay(display);
        display = NULL;
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













