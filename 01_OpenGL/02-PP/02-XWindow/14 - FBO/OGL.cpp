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

#define FBO_WIDTH 512   //width of my Frame buffer
#define FBO_HEIGHT 512  //Height of my frame buffer

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

GLuint shaderProgramObject_Cube = 0;


GLfloat Angle_Cube = 0.0f;

enum
{
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_TEXCOORD,
	AMC_ATTRIBUTE_COLOR,
	AMC_ATTRIBUTE_NORMAL
};

GLuint vao_Cube = 0;
GLuint vbo_Position_Cube = 0;
GLuint vbo_Texcoord_Cube = 0;

//uniforms

GLuint mvpMatrixUniform_Cube = 0;
GLuint textureSamplerUniform_Cube = 0;


//GLuint uMVPMatrix;

mat4 perspectiveProjectionMatrix_Cube;   //using 4*4 matrix.
                                   //mat4 - represents 4*4 matrix.   
								   //mat4 is not c++ datattype, it is defined in vmath.h.

//sphere related global variables

GLuint shaderProgramObjectPerVertex_Sphere = 0;
GLuint shaderProgramObjectPerFragment_Sphere = 0;

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

Bool bLightingEnable = False;

Bool bAnimationEnable = False;

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



Bool enablePerFragmentLighting = False;
Bool enablePerVertexLighting = False;

//light movement variable

GLfloat vLight0 = 0.0f;
GLfloat vLight1 = 0.0f;
GLfloat vLight2 = 0.0f;

GLfloat fLight0 = 0.0f;
GLfloat fLight1 = 0.0f;
GLfloat fLight2 = 0.0f;

//FBO related Global variable

GLint winWidth;
GLint winHeight;

GLuint fbo = 0;
GLuint rbo = 0;  //render buffer object
GLuint texture_FBO = 0;
Bool bFBOResult = False;






// Main
int main(void)
{
    // local function declarations

    void uninitialize_Cube(void);
    void toggleFullscreen(void);
    int initialize_Cube(void);
    void resize_Cube(int,int);
    void draw_Cube(void);
    void update_Cube (void);

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

        uninitialize_Cube();
        exit(1);  // abortive
    }

    // 2nd Step -> Get default screen from above display...
    defaultScreen = XDefaultScreen(display);


    //get available FB Configs from Xwindows

    glxFBConfigs = glXChooseFBConfig(display,DefaultScreen(display),frameBufferAttributes,&numFBConfigs);

    if(glxFBConfigs == NULL)
    {
        fprintf(gpFile,"Matching glxFBconfigs cannot be found\n\n");
        uninitialize_Cube();
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

        uninitialize_Cube();
        exit(1);
    }

    // Step 9 -> Specify to which events this window should respond...
    XSelectInput(display,window,ExposureMask | VisibilityChangeMask | StructureNotifyMask | KeyPressMask | ButtonPressMask | PointerMotionMask | FocusChangeMask);

    // Step 10 -> Specify window manager delete atom...
    windowManagerDelete = XInternAtom(display, "WM_DELETE_WINDOW", True);

    // Step 11 -> Add/Set above atom as protocol for window manager
    XSetWMProtocols(display, window, &windowManagerDelete, 1);

    // Step 12 -> Give caption to the window
    XStoreName(display, window, "Prathamesh Paropkari - Frame Buffer Object \n");

    //step 13 - show map window

    XMapWindow(display, window);

    screenWidth = XWidthOfScreen(XScreenOfDisplay(display, visualInfo->screen));
    screenHeight = XHeightOfScreen(XScreenOfDisplay(display, visualInfo->screen));
    XMoveWindow(display, window, (screenWidth - WIN_WIDTH)/2, (screenHeight - WIN_HEIGHT)/2);

    

    //OpenGL Initialization

    initialize_Cube();

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
                   //bDone = True;
                   break;

                   default:
                   break;
                    
               }

                XLookupString(&event.xkey, keys, sizeof(keys), NULL, NULL);
                switch(keys[0])
                {
                    case 'F':
                    case 'f':

                    enablePerVertexLighting = False;

                    if (enablePerFragmentLighting == False)
                    {
                        enablePerFragmentLighting = True;
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

                    case 'V':
                    case 'v':
                        enablePerFragmentLighting = False;

                        if (enablePerVertexLighting == False)
                        {
                            enablePerVertexLighting = True;
                        }

                        break;

                    case 'Q':
                    case 'q':
                        bDone = True;

                        break;

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
               resize_Cube(event.xconfigure.width,event.xconfigure.height);
               
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
            draw_Cube();

            //update function

            update_Cube();
        }


    }

    
    

    printf("End of the program\n");

    uninitialize_Cube();

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

int initialize_Cube(void)
{

    //function declaration
    void printGLInfo(void);
    void uninitialize_Cube(void);    
    void resize_Cube(int, int);
	Bool createFBO(GLint, GLint);
	Bool initialize_Sphere(GLint, GLint);
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
        uninitialize_Cube();
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
            uninitialize_Cube();
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
        uninitialize_Cube();
        exit(-1);
    }

    if (glewInit() != GLEW_OK)
	{
		fprintf(gpFile, "glewInit() failed , failed to initialize glew !!\n");
        uninitialize_Cube();
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
		"in vec2 aTexCoord;\n"
		"out vec2 oTexCoord;\n"
		"uniform mat4 uMVPMatrix;\n"          //u for uniform datatype
		"void main(void)\n"
		"{\n"
		"gl_Position=uMVPMatrix*aPosition;\n"
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

		uninitialize_Cube();
	}

	//follow all steps of vertex shader for fragment shader

	//step 6  : Write the source code for fragment shader   

	const GLchar* fragmentShaderSourceCode =
		"#version 460 core\n"
		"\n"
		"in vec2 oTexCoord;\n"
		"uniform sampler2D uTextureSampler;\n"  //sampler is a component which collects samples of image info
		"out vec4 FragColor;\n"
		"void main(void)\n"
		"{\n"
		"FragColor = texture(uTextureSampler, oTexCoord);\n"                                             
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

		 uninitialize_Cube();
	 }

	 //linking process for shader program.

	 // shader program

	 //step 7 : create shader program object

	 shaderProgramObject_Cube = glCreateProgram();

	 //step 8 : Attach both shaders into this program            also know as prelinking binding  should be done before linking and after attachment
	 glAttachShader(shaderProgramObject_Cube, vertexShaderObject);
	 glAttachShader(shaderProgramObject_Cube, fragmentShaderObject);

	 //step 9: bind attribute location to the shader program object

	 glBindAttribLocation(shaderProgramObject_Cube, AMC_ATTRIBUTE_POSITION, "aPosition");
	 glBindAttribLocation(shaderProgramObject_Cube, AMC_ATTRIBUTE_TEXCOORD, "aTexCoord");

	 //step 10: Link the shader program

	 glLinkProgram(shaderProgramObject_Cube);

	 //step 11: do linking error check fpr shader program

	 status = 0;
	 infoLogLength = 0;
	 szInfoLog = NULL;

	 glGetProgramiv(shaderProgramObject_Cube, GL_LINK_STATUS, &status);

	 if (status == GL_FALSE)
	 {
		 glGetProgramiv(shaderProgramObject_Cube, GL_INFO_LOG_LENGTH, &infoLogLength);

		 if (infoLogLength > 0)
		 {
			 szInfoLog = (GLchar*)malloc(infoLogLength);


			 if (szInfoLog != NULL)
			 {
				 glGetProgramInfoLog(shaderProgramObject_Cube, infoLogLength, NULL, szInfoLog);
				 fprintf(gpFile, "shader program linking error log : %s\n", szInfoLog);
				 free(szInfoLog);
				 szInfoLog = NULL;
				

			 }



		 }

		 uninitialize_Cube();
	 }



	 mvpMatrixUniform_Cube = glGetUniformLocation(shaderProgramObject_Cube, "uMVPMatrix");  //must be done after linkage of shader program
	                                                                     
	 //step 12: Declare position and color arrays 

	 textureSamplerUniform_Cube = glGetUniformLocation(shaderProgramObject_Cube, "uTextureSampler");

	

	 

	 


	 


	 // CUBE :

// position
	 GLfloat cubeVertices[] =
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

	
	 GLfloat cubeTexcoords[] =
	 {
		 // front
		 1.0f, 1.0f, // top-right of front
		 0.0f, 1.0f, // top-left of front
		 0.0f, 0.0f, // bottom-left of front
		 1.0f, 0.0f, // bottom-right of front

		 // right
		 1.0f, 1.0f, // top-right of right
		 0.0f, 1.0f, // top-left of right
		 0.0f, 0.0f, // bottom-left of right
		 1.0f, 0.0f, // bottom-right of right

		 // back
		 1.0f, 1.0f, // top-right of back
		 0.0f, 1.0f, // top-left of back
		 0.0f, 0.0f, // bottom-left of back
		 1.0f, 0.0f, // bottom-right of back

		 // left
		 1.0f, 1.0f, // top-right of left
		 0.0f, 1.0f, // top-left of left
		 0.0f, 0.0f, // bottom-left of left
		 1.0f, 0.0f, // bottom-right of left

		 // top
		 1.0f, 1.0f, // top-right of top
		 0.0f, 1.0f, // top-left of top
		 0.0f, 0.0f, // bottom-left of top
		 1.0f, 0.0f, // bottom-right of top

		 // bottom
		 1.0f, 1.0f, // top-right of bottom
		 0.0f, 1.0f, // top-left of bottom
		 0.0f, 0.0f, // bottom-left of bottom
		 1.0f, 0.0f, // bottom-right of bottom
	 };




	
	 

	

	 // ********************************************** for cube ***********************************************

	 

	 glGenVertexArrays(1, &vao_Cube);

	 //step 14: Bind with VAO

	 glBindVertexArray(vao_Cube);

	 //step 15: Create vertex buffer object (VBO) for position

	 //VBO for position

	 glGenBuffers(1, &vbo_Position_Cube);

	 //step 16: Bind with VBO of position

	 glBindBuffer(GL_ARRAY_BUFFER, vbo_Position_Cube);

	 glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	 glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);


	

	

	  //VBO for TEXCOORD

	 glGenBuffers(1, &vbo_Texcoord_Cube);

	 //step 16: Bind with VBO of TEXCOORD

	 glBindBuffer(GL_ARRAY_BUFFER, vbo_Texcoord_Cube);

	 glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTexcoords), cubeTexcoords, GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	 glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);
	 
	 

	 //step 18 : unbind with VAO
	 glBindVertexArray(0);



	 


	// 2nd change for depth or Enabling depth
	
	glClearDepth(1.0f); // Compulsory
	glEnable(GL_DEPTH_TEST); // 8 test paike ek(depth) test enable kr
	glDepthFunc(GL_LEQUAL); 
	


	// Set The ClearColor Of Window To Blue
	// Clear OpenGl Stars
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //Blue Color

	perspectiveProjectionMatrix_Cube = vmath::mat4::identity();

	// Create texture
	

	glEnable(GL_TEXTURE_2D);
	resize_Cube(WIN_WIDTH, WIN_HEIGHT);

	//FBO related code

	if (createFBO(FBO_WIDTH, FBO_HEIGHT) == True)
	{
		bFBOResult = initialize_Sphere(FBO_WIDTH, FBO_HEIGHT);
	}

	//code


    return(0);

}

Bool initialize_Sphere(GLint textureWidth,GLint textureHeight)
{

	//function declarations
	
	void uninitialize_Sphere(void);
	void resize_Sphere(int, int);



	//PIXELFORMATDESCRIPTOR pfd;
	//int iPixelFormateIndex = 0;

	//ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));	

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

	glBindAttribLocation(shaderProgramObjectPerVertex_Sphere, AMC_ATTRIBUTE_NORMAL, "aNormal");

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


	//FBO related code
    return(True);
}

Bool createFBO(GLint textureWidth, GLint textureHeight)
{
	//local variables
	GLint maxRenderBufferSize = 0;

	//check capacity of render buffer

	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderBufferSize);

	if (maxRenderBufferSize < textureWidth || maxRenderBufferSize < textureHeight)
	{
		fprintf(gpFile, "Texture size overflow\n");
		return(False);
	}
	
	//create custom frame buffer

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//create texture for FBO in which we are going to render Scene 2 (Sphere)

	glGenTextures(1, &texture_FBO);
	glBindTexture(GL_TEXTURE_2D,texture_FBO);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);

	// Attach above texture to framebuffer at default color attachment 0

	//no need to create render buffer for color because second parameter will ensure to fill default color buffer of our newly created FBO

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_FBO, 0);

	//create render buffer for depth

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER,rbo);  //GL_DRAW_RENDERBUFFER also can be used

	//set the storage of above render buffer for texture size for depth

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, textureWidth, textureHeight);

	//attach above depth related render buffer to FBO

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	//check the frame buffer status weather successfull or not

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(gpFile, "Framebuffer creation status is not complete\n\n");
		return(False);
	}

	//unbind with frame buffer

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return(True);


}

void resize_Cube(int width ,int height)
{
    // code
	if (height <= 0)
	{
		height = 1;
	}
		
	winWidth = width;
	winHeight = height;
	
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	//set Perspective projection matrix

	perspectiveProjectionMatrix_Cube = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

}

void resize_Sphere(int width, int height)
{
	// code
	if (height <= 0)
		height = 1;


	//glMatrixMode(GL_PROJECTION); //not allowed in PP 
	//glLoadIdentity();            not allowed in PP 

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	//set Perspective projection matrix

	perspectiveProjectionMatrix_Sphere = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	//gluPerspective(45.0f,((GLfloat)width / (GLfloat)height),0.1f,100.0f);  //not allowed in PP 


}

void draw_Cube(void)
{

	//local functions
	
	void draw_Sphere(GLint, GLint);
	void update_Sphere(void);
	void resize_Cube(int width, int height);

	//render FBO scene

	if (bFBOResult == True)
	{
		draw_Sphere(FBO_WIDTH, FBO_HEIGHT);
		update_Sphere();
	}

	//call resize cube again to companset the change done by displaySphere()

	resize_Cube(winWidth, winHeight);

	//reset the color to white to companset the change done by display sphere

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);



	//code
	// 3rd Change GL_DEPTH_BUFFER_BIT
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	//PP steps for display function

	//step 1: Use shader program object

	glUseProgram(shaderProgramObject_Cube);

	//Cube
	
	//transformations

	mat4 modelViewMatrix = mat4::identity();

	 mat4 translationMatrix = mat4::identity();

	 translationMatrix = vmath::translate(0.0f, 0.0f, -4.1f);

	 mat4 scaleMatrix = mat4::identity();

	 scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	 mat4 rotationMatrix1 = mat4::identity();

	 rotationMatrix1 = mat4::identity();

	 rotationMatrix1 = vmath::rotate(Angle_Cube, 1.0f, 0.0f, 0.0f);

	 mat4 rotationMatrix2 = mat4::identity();

	 rotationMatrix2 = mat4::identity();

	 rotationMatrix2 = vmath::rotate(Angle_Cube, 0.0f, 1.0f, 0.0f);

	 mat4 rotationMatrix3 = mat4::identity();

	 rotationMatrix3 = mat4::identity();

	 rotationMatrix3 = vmath::rotate(Angle_Cube, 0.0f, 0.0f, 1.0f);

	mat4 rotationMatrix = rotationMatrix1 * rotationMatrix2 * rotationMatrix3;

	 modelViewMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	 

	 //modelViewMatrix = mat4::identity();

	//mat4 modelViewMatrix = vmath::translate(0.0f, 0.0f, -3.0f);

	mat4 madelViewProjectionMatrix = mat4::identity();

	madelViewProjectionMatrix = perspectiveProjectionMatrix_Cube * modelViewMatrix;  //order of multiplication is not commutativ. i.e mat1 * mat2 != mat2 * mat1

	//push above mvp into vertex shader's mvp uniform

	glUniformMatrix4fv(mvpMatrixUniform_Cube, 1, GL_FALSE, madelViewProjectionMatrix);

	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, texture_FBO);

	glUniform1i(textureSamplerUniform_Cube, 0);

	//step 2: Bind with (VAO)

	glBindVertexArray(vao_Cube);

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
	
	
	glXSwapBuffers(display,window);

}

void draw_Sphere(GLint textureWidth , GLint textureHeight)
{
    void resize_Sphere(int, int);
	//code
	// 3rd Change GL_DEPTH_BUFFER_BIT

	//bind with FBO
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//call resize_Sphere()

	resize_Sphere(textureWidth, textureHeight);

	//set the clear color to back again to companset the change done by display_sphere()

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	//PP steps for display function

	//step 1: Use shader program object

	

	if (enablePerFragmentLighting == True)
	{
		glUseProgram(shaderProgramObjectPerFragment_Sphere);
	}
	else if(enablePerVertexLighting == True)
	{
		glUseProgram(shaderProgramObjectPerVertex_Sphere);
	}
    else
    {
        glUseProgram(shaderProgramObjectPerVertex_Sphere);
    }


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


	if (bLightingEnable == True)
	{
		if (enablePerVertexLighting == False && enablePerFragmentLighting == False)
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

		else if (enablePerVertexLighting == True)
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

		else if (enablePerFragmentLighting == True)
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

	//unbind with the frame buffer

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void update_Cube(void)
{
    //code

    //code
	//only at time of change animation here code will come

	Angle_Cube = Angle_Cube + 0.2f;

	if (Angle_Cube >= 360.0f)
	{
		Angle_Cube = 0.0f;
	}


}

void update_Sphere(void)
{
	//code
	//only at time of change animation here code will come

	if (bLightingEnable == True)
	{
		if (enablePerVertexLighting == True)
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

		if (enablePerFragmentLighting == True)
		{
			if (fLight0 < (2 * M_PI))
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

void uninitialize_Cube(void)
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

void uninitialize_Sphere(void)
{
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













