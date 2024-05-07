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



float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];


//GLfloat AngleCube = 0.0f;


unsigned int gNumVertices = 0;
unsigned int gNumElements = 0;


enum
{
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_ELEMENT,
	AMC_ATTRIBUTE_NORMAL
};

GLuint vaoSphere = 0;
//GLuint vaoCube = 0;

GLuint vbo_Position_Sphere = 0;
GLuint vbo_Normal_Sphere = 0;
GLuint vbo_Element_Sphere = 0;

//GLuint vbo_Position_Cube = 0;
//GLuint vbo_Color_Cube = 0;


//GLuint uMVPMatrix;

mat4 perspectiveProjectionMatrix;   //using 4*4 matrix.
                                   //mat4 - represents 4*4 matrix.   
								   //mat4 is not c++ datattype, it is defined in vmath.h.

GLuint mvpMatrixUniform = 0;
GLuint colorUniform = 0;

//rotation related variables

int day = 0;
int year = 0;

int moon_day;
int moon_year;



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
    XStoreName(display, window, "Prathamesh Paropkari - Solar System With Moon\n");

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

                    case 'D':
                        day = (day + 6) % 360;
                        break;
                    case 'd':
                        day = (day - 6) % 360;
                        break;
                    case 'Y':
                        year = (year + 3) % 360;
                        break;
                    case 'y':
                        year = (year - 3) % 360;
                        break;

                    case 'M':
                        moon_day = (moon_day + 6) % 360;
                        break;

                    case 'm':
                        moon_day = (moon_day - 6) % 360;
                        break;

                    case 'N':
                        moon_year = (moon_year + 3) % 360;
                        break;

                    case 'n':
                        moon_year = (moon_year - 3) % 360;
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
		"uniform vec4 colorUniform;"
		"void main(void)\n"
		"{\n"
		"FragColor = colorUniform;\n"
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



	 mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "uMVPMatrix");  //must be done after linkage of shader program

	 colorUniform = glGetUniformLocation(shaderProgramObject, "colorUniform");
	 //step 12: Declare position and color arrays 

	 

	 getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
	 gNumVertices = getNumberOfSphereVertices();
	 gNumElements = getNumberOfSphereElements();

	 


	

	 ////step 13: Create vertex array object
	 //VAO

	 // ********************************************** for Pyramid ***********************************************

	 glGenVertexArrays(1, &vaoSphere);

	 //step 14: Bind with VAO

	 glBindVertexArray(vaoSphere);

	 //step 15: Create vertex buffer object (VBO) for position
	 
	 //VBO for position

	 glGenBuffers(1, &vbo_Position_Sphere);

	 //step 16: Bind with VBO of position

	 glBindBuffer(GL_ARRAY_BUFFER, vbo_Position_Sphere);

	 glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	 glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);


	
	 //VBO for Normal

	 glGenBuffers(1, &vbo_Normal_Sphere);

	 glBindBuffer(GL_ARRAY_BUFFER, vbo_Normal_Sphere);

	 glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	 glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);

	 // element vbo
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

	



	//transformations for the sun

	mat4 modelMatrix = mat4::identity();

	mat4 viewMatrix = mat4::identity();

	mat4 translationMatrix = mat4::identity();

	mat4 madelViewProjectionMatrix = mat4::identity();

	//translationMatrix = vmath::translate(0.0f, 0.0f, -5.0f);

	viewMatrix = vmath::lookat(vec3(0.0f, 0.0f, 4.5f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));


	//madelViewProjectionMatrix = perspectiveProjectionMatrix * modelMatrix * viewMatrix;  //order  is not commutativ. i.e mat1 * mat2 != mat2 * mat1

	//glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, madelViewProjectionMatrix);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	pushMatrix(modelMatrix);
	{

		madelViewProjectionMatrix = perspectiveProjectionMatrix * viewMatrix * modelMatrix;  //order  is not commutativ. i.e mat1 * mat2 != mat2 * mat1

		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, madelViewProjectionMatrix);

		glUniform4f(colorUniform, 1.0f, 0.5f, 0.0f, 1.0);

		glBindVertexArray(vaoSphere);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_Element_Sphere);
		glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

		glBindVertexArray(0);

		//************************************************************

		//earth related transformations



		translationMatrix = vmath::translate(1.3f, 0.0f, 0.0f);

		mat4 eyRotationMatrix = mat4::identity();
		eyRotationMatrix = vmath::rotate((GLfloat)year, 0.0f, 1.0f, 0.0f);

		mat4 edRotationMatrix = mat4::identity();
		edRotationMatrix = vmath::rotate((GLfloat)day, 0.0f, 1.0f, 0.0f);

		modelMatrix = modelMatrix * eyRotationMatrix * translationMatrix * edRotationMatrix;  //revolve 

		

		pushMatrix(modelMatrix);
		{
			mat4 scaleMatrix = mat4::identity();

			scaleMatrix = vmath::scale(0.55f, 0.55f, 0.55f);

			modelMatrix = modelMatrix * scaleMatrix;

			madelViewProjectionMatrix = perspectiveProjectionMatrix * viewMatrix * modelMatrix;

			glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, madelViewProjectionMatrix);

			glUniform4f(colorUniform,0.4f, 0.9f, 1.0f, 1.0f);

			glBindVertexArray(vaoSphere);


			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_Element_Sphere);
			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

			glBindVertexArray(0);
		}
		modelMatrix = popMatrix();

		//moon code

		translationMatrix = vmath::translate(0.6f, 0.0f, 0.0f);



		mat4 myRotationMatrix = mat4::identity();
		myRotationMatrix = vmath::rotate((GLfloat)moon_year, 0.0f, 1.0f, 0.0f);

		mat4 mdRotationMatrix = mat4::identity();
		mdRotationMatrix = vmath::rotate((GLfloat)moon_day, 0.0f, 1.0f, 0.0f);

		modelMatrix = modelMatrix * myRotationMatrix * translationMatrix * mdRotationMatrix;  //revolve 



		pushMatrix(modelMatrix);
		{
			mat4 scaleMatrix = mat4::identity();

			scaleMatrix = vmath::scale(0.30f, 0.30f, 0.30f);

			modelMatrix = modelMatrix * scaleMatrix;

			madelViewProjectionMatrix = perspectiveProjectionMatrix * viewMatrix * modelMatrix;

			glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, madelViewProjectionMatrix);

			glUniform4f(colorUniform, 1.0f, 1.0f, 1.0f, 1.0f);

			glBindVertexArray(vaoSphere);


			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_Element_Sphere);
			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

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













