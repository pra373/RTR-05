// standard header file
#include<stdio.h>
#include<stdlib.h> // for exit
#include<memory.h> // for memset

//OpenGL Header files
#include<GL/glew.h>
#include<GL/glx.h>  
#include<GL/gl.h>

#include "Sphere.h" // header for sphere


// X-11 header files
#include<X11/Xlib.h> // for all X-Window API
#include<X11/Xutil.h>  // for XVisualInfo and related API
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

//openGL related global variables

GLuint shaderProgramObject_PF = 0;
GLuint shaderProgramObject_PV = 0;

GLuint gVao = 0;
GLuint gVbo_sphere_position = 0;
GLuint gVbo_sphere_normal = 0;
GLuint gVbo_sphere_element = 0;
GLuint vbo_Texcoord = 0;


//unsigned int  gNumVertices;
unsigned int  gNumSphereElements;




enum
{
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_NORMAL,
	AMC_ATTRIBUTE_TEXCOORD,
	AMC_ATTRIBUTE_COLOR
};



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

Bool bLightingEnable = False;

Bool bAnimationEnable = False;

Bool enablePFLighting = False;

Bool enablePVLighting = False;

// Light Variables

GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // white diffuse light
GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f }; 


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

struct LightTwo
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec4 position;
};

struct LightTwo lightTwo[2];


//Global variable declaration for sphere
	float sphere_position[1146];
	float sphere_normals[1146];
	float sphere_texcoord[764];
	unsigned short sphere_elements[2280];
	

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
    XStoreName(display, window, "Prathamesh Paropkari - Per Vertex Per Fragment Toggling\n");

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
                  if (bFullscreen == False)
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

                   default:
                   break;
                    
               }

                XLookupString(&event.xkey, keys, sizeof(keys), NULL, NULL);
                switch(keys[0])
                {
				case 'A':
				case 'a':
					if (bAnimationEnable == False)
					{
						bAnimationEnable = True;
					}
					else
					{
						bAnimationEnable = False;
					}
					break;
				case 'L':
				case 'l':
					if (bLightingEnable == False)
					{
						bLightingEnable = True;
					}
					else
					{
						bLightingEnable = False;
					}
					break;
				case 'F':
				case 'f':
					if (enablePFLighting == False)
					{
						enablePFLighting = True;
						enablePVLighting = False;
					}
					break;
				case 'V':
				case 'v':
					if (enablePVLighting == False)
					{
						enablePVLighting = True;
						enablePFLighting = False;
					}
					break;
				case 'Q':
				case 'q':
					bDone = True;
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
	const GLchar* vertexShaderSourceCode_PF =
		"#version 460 core\n"              // (version of GLSL * 100) core means you are using core profile i.e PP
		"\n"
		"in vec4 aPosition;\n"
		"in vec3 aNormal;\n"
		"uniform mat4 uModelMatrix;\n"
		"uniform mat4 uViewMatrix;\n"
		"uniform mat4 uProjectionMatrix;\n"
		"uniform vec4 ulightPosition[2];\n"
		"uniform int uKeyPress;\n"
		"out vec3 otransformedNormals;\n"
		"out vec3 olightDirection[2];\n"
		"out vec3 oviewerVector;\n"
		"void main(void)\n"
		"{\n"
		"if(uKeyPress == 1)\n"
		"{\n"
		"vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;\n"
		"otransformedNormals = (mat3(uViewMatrix * uModelMatrix) * aNormal);\n"
		"oviewerVector = -eyeCoordinates.xyz;\n"
		"for(int i = 0; i < 2; i++)\n"
		"{\n"
		"olightDirection[i] = vec3(ulightPosition[i] - eyeCoordinates);\n"
		"}\n"
		"}\n"
		"else\n"
		"{\n"
		"for(int i = 0; i < 2; i++)\n"
		"{\n"
		"olightDirection[i] = vec3(0.0, 0.0, 0.0);\n"
		"}\n"
		"otransformedNormals =  vec3(0.0, 0.0, 0.0);\n"
		"oviewerVector = vec3(0.0, 0.0, 0.0);\n"
		"}\n"
		"gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;\n"
		"}\n";


	//step 2: Create vertex shader object

	GLuint vertexShaderObject_PF = glCreateShader(GL_VERTEX_SHADER);

	//step 3 : Give vertex shader code to OpenGL
	glShaderSource(vertexShaderObject_PF, 1, (const GLchar**)&vertexShaderSourceCode_PF, NULL);

	//Step 4 : Let OpenGL engine Compile the vertex shader code
	glCompileShader(vertexShaderObject_PF);
	
	GLint status = 0;
	GLint infoLogLength = 0;
	GLchar* szInfoLog = NULL;

	//step 5 : Check for vertex shader compilation
	//step 5 (a) :  get compile status in status variable
	glGetShaderiv(vertexShaderObject_PF, GL_COMPILE_STATUS, &status);

	//step 5 (b) : if error occur i.e status == GL_FALSE,  get length of error message into infoLogLength variable
	if (status == GL_FALSE)
	{
		 
		glGetShaderiv(vertexShaderObject_PF, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			//step 5 (c) : declare a string variable allocate memory equal to above length i.e length of error message
			szInfoLog = (GLchar*)malloc(infoLogLength);


			if (szInfoLog != NULL)
			{
				// step 5 (d) : get the compilation error log info into memory allocated string
				glGetShaderInfoLog(vertexShaderObject_PF, infoLogLength, NULL, szInfoLog);

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
	const GLchar* fragmentShaderSourceCode_PF =
		"#version 460 core\n"
		"\n"
		"in vec3 PhongADSLight;\n"
		"in vec3 otransformedNormals;\n"
		"in vec3 olightDirection[2];\n"
		"in vec3 oviewerVector;\n"
		"uniform mat4 uModelMatrix;\n"
		"uniform mat4 uViewMatrix;\n"
		"uniform mat4 uProjectionMatrix;\n"
		"uniform vec3 ulightAmbient[2];\n"
		"uniform vec3 ulightDiffuse[2];\n"
		"uniform vec3 ulightSpecular[2];\n"
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
		"vec3 normalizedLightDirection[2];\n"
		"vec3 normalizedTransformedNormals = normalize(otransformedNormals);\n"
		"vec3 normalizedViwerVector = normalize(oviewerVector);\n"
		"vec3 lightAmbient[2];\n"
		"vec3 lightDiffuse[2];\n"
		"vec3 reflectionVector[2];\n"
		"vec3 lightSpecular[2];\n"
		"for(int i = 0; i < 2; i++)\n"
		"{\n"
		"lightAmbient[i] = ulightAmbient[i] * uMaterialAmbient;\n"
		"normalizedLightDirection[i] = normalize(olightDirection[i]);\n"
		"lightDiffuse[i] = ulightDiffuse[i] * uMaterialDiffuse * max(dot(normalizedLightDirection[i], normalizedTransformedNormals),0.0);\n"
		"reflectionVector[i] = reflect(-normalizedLightDirection[i], normalizedTransformedNormals);\n"
		"lightSpecular[i] = ulightSpecular[i] * uMaterialSpecular * pow(max(dot(reflectionVector[i], normalizedViwerVector), 0.0),uMaterialShininess);\n"
		"PhongADSLight = PhongADSLight + lightAmbient[i] + lightDiffuse[i] + lightSpecular[i];\n "
		"}\n"
		"}\n"
		"else\n"
		"{\n"
		"PhongADSLight = vec3(1.0, 1.0, 1.0);\n"
		"}\n"
		"FragColor =  vec4 (PhongADSLight, 1.0);\n"
		"}";

	GLuint fragmentShaderObject_PF = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShaderObject_PF, 1, (const GLchar**)&fragmentShaderSourceCode_PF, NULL);

	glCompileShader(fragmentShaderObject_PF);

	 status = 0;
	 infoLogLength = 0;
	 szInfoLog = NULL;

	 glGetShaderiv(fragmentShaderObject_PF, GL_COMPILE_STATUS, &status);

	 if (status == GL_FALSE)
	 {
		
		 glGetShaderiv(fragmentShaderObject_PF, GL_INFO_LOG_LENGTH, &infoLogLength);

		 if (infoLogLength > 0)
		 {
			 szInfoLog = (GLchar*)malloc(infoLogLength);


			 if (szInfoLog != NULL)
			 {
				 glGetShaderInfoLog(fragmentShaderObject_PF, infoLogLength, NULL, szInfoLog);
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

	 shaderProgramObject_PF = glCreateProgram();

	 //step 8 : Attach both shaders into this program            also know as prelinking binding  should be done before linking and after attachment
	 glAttachShader(shaderProgramObject_PF, vertexShaderObject_PF);
	 glAttachShader(shaderProgramObject_PF, fragmentShaderObject_PF);

	 //step 9: bind attribute location to the shader program object

	 glBindAttribLocation(shaderProgramObject_PF, AMC_ATTRIBUTE_POSITION, "aPosition");

	 glBindAttribLocation(shaderProgramObject_PF, AMC_ATTRIBUTE_COLOR, "aColor");

	 glBindAttribLocation(shaderProgramObject_PF, AMC_ATTRIBUTE_NORMAL , "aNormal");

	 //step 10: Link the shader program

	 glLinkProgram(shaderProgramObject_PF);

	 //step 11: do linking error check fpr shader program

	 status = 0;
	 infoLogLength = 0;
	 szInfoLog = NULL;

	 glGetProgramiv(shaderProgramObject_PF, GL_LINK_STATUS, &status);

	 if (status == GL_FALSE)
	 {
		 glGetProgramiv(shaderProgramObject_PF, GL_INFO_LOG_LENGTH, &infoLogLength);

		 if (infoLogLength > 0)
		 {
			 szInfoLog = (GLchar*)malloc(infoLogLength);


			 if (szInfoLog != NULL)
			 {
				 glGetProgramInfoLog(shaderProgramObject_PF, infoLogLength, NULL, szInfoLog);
				 fprintf(gpFile, "shader program linking error log : %s\n", szInfoLog);
				 free(szInfoLog);
				 szInfoLog = NULL;
				

			 }



		 }

		 uninitialize();
	 }


	 // Get shader uniform location

	 modelMatrixUniform = glGetUniformLocation(shaderProgramObject_PF, "uModelMatrix");

	 viewMatrixUniform = glGetUniformLocation(shaderProgramObject_PF, "uViewMatrix");

	 projectionMatrixUniform = glGetUniformLocation(shaderProgramObject_PF, "uProjectionMatrix");

	 lightAmbientUniform[0] = glGetUniformLocation(shaderProgramObject_PF, "ulightAmbient[0]");

	 lightDiffuseUniform[0] = glGetUniformLocation(shaderProgramObject_PF, "ulightDiffuse[0]");

	 lightSpecularUniform[0] = glGetUniformLocation(shaderProgramObject_PF, "ulightSpecular[0]");

	 lightPositionUniform[0] = glGetUniformLocation(shaderProgramObject_PF, "ulightPosition[0]");

	 lightAmbientUniform[1] = glGetUniformLocation(shaderProgramObject_PF, "ulightAmbient[1]");

	 lightDiffuseUniform[1] = glGetUniformLocation(shaderProgramObject_PF, "ulightDiffuse[1]");

	 lightSpecularUniform[1] = glGetUniformLocation(shaderProgramObject_PF, "ulightSpecular[1]");

	 lightPositionUniform[1] = glGetUniformLocation(shaderProgramObject_PF, "ulightPosition[1]");


	 materialAmbientUniform = glGetUniformLocation(shaderProgramObject_PF, "uMaterialAmbient");

	 materialDiffuseUniform = glGetUniformLocation(shaderProgramObject_PF, "uMaterialDiffuse");


	 materialSpecularUniform = glGetUniformLocation(shaderProgramObject_PF, "uMaterialSpecular");

	 materialShininessUniform = glGetUniformLocation(shaderProgramObject_PF, "uMaterialShininess");

	 keyPressUniform = glGetUniformLocation(shaderProgramObject_PF, "uKeyPress");



	 //************ Per Vertex Lightning Shaders and Shader Program Object.. //**************************************

	 //vertex shader
	 const GLchar* vertexShaderSourceCode_PV =
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
	 glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode_PV, NULL);

	 //Step 4 : Let OpenGL engine Compile the vertex shader code
	 glCompileShader(vertexShaderObject);

	  status = 0;
	  infoLogLength = 0;
	  szInfoLog = NULL;

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
	 const GLchar* fragmentShaderSourceCode_PV =
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

	 glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode_PV, NULL);

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

	 shaderProgramObject_PV = glCreateProgram();

	 //step 8 : Attach both shaders into this program            also know as prelinking binding  should be done before linking and after attachment
	 glAttachShader(shaderProgramObject_PV, vertexShaderObject);
	 glAttachShader(shaderProgramObject_PV, fragmentShaderObject);

	 //step 9: bind attribute location to the shader program object

	 glBindAttribLocation(shaderProgramObject_PV, AMC_ATTRIBUTE_POSITION, "aPosition");

	 glBindAttribLocation(shaderProgramObject_PV, AMC_ATTRIBUTE_COLOR, "aColor");

	 glBindAttribLocation(shaderProgramObject_PV, AMC_ATTRIBUTE_NORMAL, "aNormal");

	 //step 10: Link the shader program

	 glLinkProgram(shaderProgramObject_PV);

	 //step 11: do linking error check fpr shader program

	 status = 0;
	 infoLogLength = 0;
	 szInfoLog = NULL;

	 glGetProgramiv(shaderProgramObject_PV, GL_LINK_STATUS, &status);

	 if (status == GL_FALSE)
	 {
		 glGetProgramiv(shaderProgramObject_PV, GL_INFO_LOG_LENGTH, &infoLogLength);

		 if (infoLogLength > 0)
		 {
			 szInfoLog = (GLchar*)malloc(infoLogLength);


			 if (szInfoLog != NULL)
			 {
				 glGetProgramInfoLog(shaderProgramObject_PV, infoLogLength, NULL, szInfoLog);
				 fprintf(gpFile, "shader program linking error log : %s\n", szInfoLog);
				 free(szInfoLog);
				 szInfoLog = NULL;


			 }



		 }

		 uninitialize();
	 }

	 // Get shader uniform location

	 modelMatrixUniform = glGetUniformLocation(shaderProgramObject_PV, "uModelMatrix");

	 viewMatrixUniform = glGetUniformLocation(shaderProgramObject_PV, "uViewMatrix");

	 projectionMatrixUniform = glGetUniformLocation(shaderProgramObject_PV, "uProjectionMatrix");

	 lightAmbientUniform[0] = glGetUniformLocation(shaderProgramObject_PV, "ulightAmbient[0]");

	 lightDiffuseUniform[0] = glGetUniformLocation(shaderProgramObject_PV, "ulightDiffuse[0]");

	 lightSpecularUniform[0] = glGetUniformLocation(shaderProgramObject_PV, "ulightSpecular[0]");

	 lightPositionUniform[0] = glGetUniformLocation(shaderProgramObject_PV, "ulightPosition[0]");

	 lightAmbientUniform[1] = glGetUniformLocation(shaderProgramObject_PV, "ulightAmbient[1]");

	 lightDiffuseUniform[1] = glGetUniformLocation(shaderProgramObject_PV, "ulightDiffuse[1]");

	 lightSpecularUniform[1] = glGetUniformLocation(shaderProgramObject_PV, "ulightSpecular[1]");

	 lightPositionUniform[1] = glGetUniformLocation(shaderProgramObject_PV, "ulightPosition[1]");


	 materialAmbientUniform = glGetUniformLocation(shaderProgramObject_PV, "uMaterialAmbient");

	 materialDiffuseUniform = glGetUniformLocation(shaderProgramObject_PV, "uMaterialDiffuse");


	 materialSpecularUniform = glGetUniformLocation(shaderProgramObject_PV, "uMaterialSpecular");

	 materialShininessUniform = glGetUniformLocation(shaderProgramObject_PV, "uMaterialShininess");

	 keyPressUniform = glGetUniformLocation(shaderProgramObject_PV, "uKeyPress");


	 ///// for sphere//////


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

	// For Per Fragment
	light[0].ambient = vec3(0.0, 0.0, 0.0);
	light[1].ambient = vec3(0.0, 0.0, 0.0);
	light[0].diffuse = vec3(1.0, 0.5, 0.0);
	light[1].diffuse = vec3(0.5, 0.5, 0.5);
	light[0].specular = vec3(1.0, 0.5, 0.0);
	light[1].specular = vec3(0.5, 0.5, 0.5);
	light[0].position = vec4(-2.0, 0.0, 0.0, 1.0);
	light[1].position = vec4(2.0, 0.0, 1.0, 1.0);

	// For Per Vertex
	lightTwo[0].ambient = vec3(0.0, 0.0, 0.0);
	lightTwo[1].ambient = vec3(0.0, 0.0, 0.0);
	lightTwo[0].diffuse = vec3(1.0, 0.0, 0.0);
	lightTwo[1].diffuse = vec3(0.0, 1.0, 0.0);
	lightTwo[0].specular = vec3(1.0, 0.0, 0.0);
	lightTwo[1].specular = vec3(0.0, 1.0, 0.0);
	lightTwo[0].position = vec4(-2.0, 0.0, 0.0, 1.0);
	lightTwo[1].position = vec4(2.0, 0.0, 1.0, 1.0);

	perspectiveProjectionMatrix = vmath::mat4::identity();

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

	
	
	glUseProgram(shaderProgramObject_PF);
	
    if (enablePFLighting == True)
	{
		glUseProgram(shaderProgramObject_PF);
	}
	else if(enablePVLighting == True)
	{
		glUseProgram(shaderProgramObject_PV);
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
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelViewMatrix);

	// Pass viewMatrix to the vertex shader's uniform
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

	// Pass perspectiveProjectionMatrix to the vertex shader's uniform
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	

	if (bLightingEnable == True)
	{
		if (enablePVLighting == False && enablePFLighting == False)
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
		else if (enablePVLighting == True)
		{
			glUniform1i(keyPressUniform, 1);

			glUniform3fv(lightAmbientUniform[0], 1, lightTwo[0].ambient);

			glUniform3fv(lightDiffuseUniform[0], 1, lightTwo[0].diffuse);

			glUniform3fv(lightSpecularUniform[0], 1, lightTwo[0].specular);

			glUniform4fv(lightPositionUniform[0], 1, lightTwo[0].position);

			glUniform3fv(lightAmbientUniform[1], 1, lightTwo[1].ambient);

			glUniform3fv(lightDiffuseUniform[1], 1, lightTwo[1].diffuse);

			glUniform3fv(lightSpecularUniform[1], 1, lightTwo[1].specular);

			glUniform4fv(lightPositionUniform[1], 1, lightTwo[1].position);

			glUniform3fv(materialAmbientUniform, 1, materialAmbient);

			glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

			glUniform3fv(materialSpecularUniform, 1, materialSpecular);

			glUniform1f(materialShininessUniform, materialShininess);
		}
		else if (enablePFLighting == True)
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

	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}



	// *** bind vao ***
	glBindVertexArray(gVao);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);

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


}

void uninitialize(void)
{
  //code

//code
   
	//steps for uninitialize function in PP

	if (shaderProgramObject_PF)
	{
		//step 1 : use shader program object

		glUseProgram(shaderProgramObject_PF);

		GLint numShaders = 0;

		//step 2 : get number of attached shaders
		glGetProgramiv(shaderProgramObject_PF, GL_ATTACHED_SHADERS, &numShaders);



		if (numShaders > 0)
		{
			//step 3 : Create and allocate memory of shader objects according to number of attached shaders
			GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint));

			if (pShaders != NULL)
			{
				//step 4 : Get shader object into the allocated array.

				glGetAttachedShaders(shaderProgramObject_PF, numShaders, NULL, pShaders);

				//step 5 : Detach and delete shader objects iteratively

				for (GLint i = 0; i < numShaders; i++)
				{
					glDetachShader(shaderProgramObject_PF, pShaders[i]);
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

		glDeleteProgram(shaderProgramObject_PF);
		shaderProgramObject_PF = 0;

		
		



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













