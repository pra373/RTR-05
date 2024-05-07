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
#include<X11/Xlib.h>   //for all X-Window API
#include<X11/Xutil.h>  //for XVisualInfo and related API
#include<X11/XKBlib.h> 
#include"vmath.h"

using namespace vmath;

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

Bool bLightingEnable = False;

Bool bAnimationEnable = False;


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
    XStoreName(display, window, "Prathamesh Paropkari - 24 Spheres\n");

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

                    case 'L':
                    case 'l':
                    if(bLightingEnable == False)
                    {
                       
                       bLightingEnable = True;
                    }
                    else
                    {
                        
                        bLightingEnable = False;
                    }
                    break;

                    case 'X':
                    case 'x':
                        keyPressed = 1;
                        angleForXRotation = 0.0f; // reset rotation
                        break;

                    case 'Y':
                    case 'y':
                        keyPressed = 2;
                        angleForYRotation = 0.0f; // reset rotation
                        break;

                    case 'Z':
                    case 'z':
                        keyPressed = 3;
                        angleForZRotation = 0.0f; // reset rotation
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
    

    //Local variable declaration for sphere
	float sphere_position[1146];
	float sphere_normals[1146];
	float sphere_texcoord[764];
	unsigned short sphere_elements[2280];

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
	


	// Set The ClearColor Of Window To Blue
	// Clear OpenGl Stars
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f); //Blue Color

	perspectiveProjectionMatrix = vmath::mat4::identity();

	resize(WIN_WIDTH, WIN_HEIGHT);

    return(0);

}

void resize(int width ,int height)
{
    // code
	if(height <= 0)
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


void draw(void)
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

	if (bLightingEnable == True)
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

	if (bLightingEnable == True)
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

	if (bLightingEnable == True)
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

	if (bLightingEnable == True)
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

	if (bLightingEnable == True)
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

	if (bLightingEnable == True)
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

	if (bLightingEnable == True)
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

	if (bLightingEnable == True)
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

	if (bLightingEnable == True)
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

	if (bLightingEnable == True)
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

	if (bLightingEnable == True)
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

	if (bLightingEnable == True)
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

	if (bLightingEnable == True)
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

	if (bLightingEnable == True)
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

	if (bLightingEnable == True)
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

	if (bLightingEnable == True)
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

	if (bLightingEnable == True)
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

	if (bLightingEnable == True)
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

	if (bLightingEnable == True)
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

	if (bLightingEnable == True)
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

	if (bLightingEnable == True)
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

	if (bLightingEnable == True)
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

	if (bLightingEnable == True)
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

	if (bLightingEnable == True)
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

	
	glXSwapBuffers(display,window);

}

void update(void)
{
    //code
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

    if (gVbo_sphere_element)
	{
		glDeleteBuffers(1, &gVbo_sphere_element);
		gVbo_sphere_element = 0;
	}

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













