// standard header file
#include<stdio.h>
#include<stdlib.h> // for exit
#include<memory.h> // for memset


#define _USE_MATH_DEFINES
#include<math.h>
#define M_PI 3.14159265358979323846

//OpenGL Header files
#include<GL/glx.h>
#include<GL/gl.h>
#include<GL/glu.h>

// X-11 header files
#include<X11/Xlib.h> // for all X-Window API
#include<X11/Xutil.h>  // for XVisualInfo and related API
#include<X11/XKBlib.h> 

// OpenAL Related header files

#include<AL/alut.h>

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global Variable Declarations

Display *display = NULL;
Colormap colormap;
Window window; // X-window la represent karnara structure...
XVisualInfo *visualInfo = NULL;

//OpenGL Global Variables

GLXContext glxContext = NULL;

Bool bFullscreen = False;
Bool bActiveWindow = False;


GLfloat Opacity = 1.0f;

//Letter translation variables

GLfloat Move_Letter_B = -8.0f;
GLfloat Move_Letter_H_X = -8.675f;
GLfloat Move_Letter_H_Y = 7.0f;
GLfloat Move_Letter_A_X = -8.44f;
GLfloat Move_Letter_A_Y = -9.0f;
GLfloat Move_Letter_R_X = 11.1f;
GLfloat Move_Letter_R_Y = 9.0f;
GLfloat Move_Letter_A2_X = 14.1f;
GLfloat Move_Letter_A2_Y = -9.0f;
GLfloat Move_Letter_T_X = 13.5f;

//Plane translation variables

GLfloat Move_Middle_Plane_X = -15.0f;

GLfloat Move_Upper_Plane_X = -4.5f;
GLfloat Move_Upper_Plane_Y = 0.4f;

GLfloat Move_Lower_Plane_X = -4.5f;
GLfloat Move_Lower_Plane_Y = -0.4f;

//Shading variables

//letter T variables

GLfloat Red_T_Orange = 128.0f;
GLfloat Green_T_Orange = 128.0f;
GLfloat Blue_T_Orange = 128.0f;

GLfloat Red_T_White = 128.0f;
GLfloat Green_T_White = 128.0f;
GLfloat Blue_T_White = 128.0f;

GLfloat Red_T_Green = 128.0f;
GLfloat Green_T_Green = 128.0f;
GLfloat Blue_T_Green = 128.0f;

//letter A3 second variables

GLfloat Red_A3_Second_Orange = 128.0f;
GLfloat Green_A3_Second_Orange = 128.0f;
GLfloat Blue_A3_Second_Orange = 128.0f;

GLfloat Red_A3_Second_White = 128.0f;
GLfloat Green_A3_Second_White = 128.0f;
GLfloat Blue_A3_Second_White = 128.0f;

GLfloat Red_A3_Second_Green = 128.0f;
GLfloat Green_A3_Second_Green = 128.0f;
GLfloat Blue_A3_Second_Green = 128.0f;

//letter A3 first variable

GLfloat Red_A3_First_Orange = 128.0f;
GLfloat Green_A3_First_Orange = 128.0f;
GLfloat Blue_A3_First_Orange = 128.0f;

GLfloat Red_A3_First_White = 128.0f;
GLfloat Green_A3_First_White = 128.0f;
GLfloat Blue_A3_First_White = 128.0f;

GLfloat Red_A3_First_Green = 128.0f;
GLfloat Green_A3_First_Green = 128.0f;
GLfloat Blue_A3_First_Green = 128.0f;


//letter R variables

GLfloat Red_R_Orange = 128.0f;
GLfloat Green_R_Orange = 128.0f;
GLfloat Blue_R_Orange = 128.0f;

GLfloat Red_R_White = 128.0f;
GLfloat Green_R_White = 128.0f;
GLfloat Blue_R_White = 128.0f;

GLfloat Red_R_Green = 128.0f;
GLfloat Green_R_Green = 128.0f;
GLfloat Blue_R_Green = 128.0f;

//letter H variables

GLfloat Red_H_Orange = 128.0f;
GLfloat Green_H_Orange = 128.0f;
GLfloat Blue_H_Orange = 128.0f;

GLfloat Red_H_White = 128.0f;
GLfloat Green_H_White = 128.0f;
GLfloat Blue_H_White = 128.0f;

GLfloat Red_H_Green = 128.0f;
GLfloat Green_H_Green = 128.0f;
GLfloat Blue_H_Green = 128.0f;

//Letter B Shading variables

GLfloat Red_B_Orange = 128.0f;
GLfloat Green_B_Orange = 128.0f;
GLfloat Blue_B_Orange = 128.0f;

GLfloat Red_B_White = 128.0f;
GLfloat Green_B_White = 128.0f;
GLfloat Blue_B_White = 128.0f;

GLfloat Red_B_Green = 128.0f;
GLfloat Green_B_Green = 128.0f;
GLfloat Blue_B_Green = 128.0f;

//plane rotation variables

GLfloat Rotate_Plane_Two = 0.0f;
GLfloat Rotate_Plane_Three = 0.0f;

//OpenAL Related variable declarations

 ALCenum error;
 ALuint audioBuffer;  //handle to audio buffer
 ALuint audioSource;  

// Main
int main(void)
{
    // local function declarations

    void uninitialize(void);
    void toggleFullscreen(void);
    void initialize(void);
    void resize(int,int);
    void draw(void);
    void update (void);

    // local variable declarations

     int defaultScreen;
     Bool bDone = False;

     int frameBufferAttributes[]={GLX_DOUBLEBUFFER,True,
                                  GLX_RGBA,
                                  GLX_RED_SIZE,8,
                                  GLX_GREEN_SIZE,8,
                                  GLX_BLUE_SIZE,8,
                                  GLX_ALPHA_SIZE,8,
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

    // step 1 -> 
    display = XOpenDisplay(NULL);

    if(display == NULL)
    {
        printf("XOpenDisplay Failed\n");

        uninitialize();
        exit(1);  // abortive
    }

    // 2nd Step -> Get default screen from above display...
    defaultScreen = XDefaultScreen(display);

    // 3rd Step -> Get Default Depth From Above Two...
    //defaultDepth = XDefaultDepth(display, defaultScreen); // remove this line for this code

    // 4th Step -> Get visual info from above three...
   // memset((void*)&visualInfo, 0, sizeof(XVisualInfo));   // remove this line for this code

    //status = XMatchVisualInfo(display, defaultScreen, defaultDepth, TrueColor, &visualInfo);  // remove this line for this code
    visualInfo = glXChooseVisual(display,defaultScreen,frameBufferAttributes);

    if( visualInfo == NULL)
    {
        printf("glXChooseVisual Failed\n");

        uninitialize();
        exit(1);
    }

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
    XStoreName(display, window, "Prathamesh Paropkari - Static Bharat Assignment");

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

void initialize(void)
{

    //local function declaration
    void resize(int,int);
    void uninitialize(void);
	void initializeAudio(void);
    //code

    //create OpenGL Context
    glxContext = glXCreateContext(display,visualInfo,NULL,True);

    if(glxContext==NULL)
    {
        printf("GLXCreateContext Failed !!!");
        uninitialize();
        exit(-1);

    }

    //make this context as current context

    if(glXMakeCurrent(display,window,glxContext) == False)
    {
        printf("glXMakeCurrent failed !!!");
        uninitialize();
        exit(-1);
    }

    //usual openGL code

    glClearColor(0.0f,0.0f,0.0f,1.0f);

    //warmup resize


    resize(WIN_WIDTH,WIN_HEIGHT);  

	initializeAudio();
}

void resize(int width ,int height)
{
    //code

    if(height == 0)
    {
        height = 1;
    }

    glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}


void draw(void)
{
    //local function declarations
	void Letter_B(void);
	void Letter_H(void);
	void Letter_A1(void);
	void Letter_A2(void);
	void Letter_A3_First(void);
	void Letter_A3_Second(void);
	void Letter_T(void);
	void Letter_R(void);
	void Plane1(void);
	void Plane2(void);
	void Plane3(void);
	void Parabola(void);

	//coloured letter functions

	//code
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glTranslatef(-0.39f, 0.0f, -3.0f);
	
	
	Letter_B();
	Letter_H();
	Letter_A1();
	//Letter_A3_First();
	//Letter_A3_Second();
	Letter_T();
	Letter_R();
	Letter_A2();
	Plane1();
	Plane2();
	Plane3();
	//Parabola();
	
	//Letter_A3();
	 

    glXSwapBuffers(display,window);
}


void update(void)
{
	//code
	

	if (Move_Letter_B <= -3.9f)
	{
		//Move_Letter_B = Move_Letter_B + 0.00085f;
		Move_Letter_B = Move_Letter_B + 0.009f;
	}

	if (Move_Letter_B >= -3.9f && Move_Letter_H_Y >= -0.01f)
	{
		//Move_Letter_H_X = Move_Letter_H_X + 0.0015f;
		//Move_Letter_H_Y = Move_Letter_H_Y - 0.0015f;
		//fprintf(gpFile, "Move_Letter_H_X = %f\n", Move_Letter_H_X);

		Move_Letter_H_X = Move_Letter_H_X + 0.0088f;
		Move_Letter_H_Y = Move_Letter_H_Y - 0.0088f;
	}

	if (Move_Letter_H_X >= -1.664147 && Move_Letter_A_Y <= 0.03f)
	{
		//Move_Letter_A_X = Move_Letter_A_X + 0.003f;
		//Move_Letter_A_Y = Move_Letter_A_Y + 0.003f;
		//fprintf(gpFile, "Move_Letter_A_X = %f\n", Move_Letter_A_X);

		Move_Letter_A_X = Move_Letter_A_X + 0.0095f;
		Move_Letter_A_Y = Move_Letter_A_Y + 0.0095f;
	}

	if (Move_Letter_A_X >= -1.664147f && Move_Letter_R_Y >= 0.022f)
	{
		//Move_Letter_R_X = Move_Letter_R_X - 0.0015f;
		//Move_Letter_R_Y = Move_Letter_R_Y - 0.0015f;
		//fprintf(gpFile, "Move_Letter_R_X = %f\n", Move_Letter_R_X);

		Move_Letter_R_X = Move_Letter_R_X - 0.009f;
		Move_Letter_R_Y = Move_Letter_R_Y - 0.009f;
	}

	if (Move_Letter_R_X <= 2.121174 && Move_Letter_A2_Y <= 0.022f)
	{
		//Move_Letter_A2_X = Move_Letter_A2_X - 0.003f;
		//Move_Letter_A2_Y = Move_Letter_A2_Y + 0.003f;
		//fprintf(gpFile, "Move_Letter_A2_X = %f\n", Move_Letter_A2_X);

		Move_Letter_A2_X = Move_Letter_A2_X - 0.009f;
		Move_Letter_A2_Y = Move_Letter_A2_Y + 0.009f;
	}

	if (Move_Letter_A2_Y >= 0.022f && Move_Letter_T_X >= 5.9f)
	{
		//Move_Letter_T_X = Move_Letter_T_X - 0.003f;
		
		//fprintf(gpFile, "Move_Letter_A2_X = %f\n", Move_Letter_A2_X);

		Move_Letter_T_X = Move_Letter_T_X - 0.009f;
	}
	
	//plane movement starts

	if (Move_Letter_T_X <= 5.9f && Move_Middle_Plane_X <= 19.0f)
	{
		//Move_Middle_Plane_X = Move_Middle_Plane_X + 0.004f;
		Move_Middle_Plane_X = Move_Middle_Plane_X + 0.004f;
	}

	

	if (Move_Letter_T_X <= 5.9f && Move_Upper_Plane_X <= 0.0f)
	{
		
		Move_Upper_Plane_X = Move_Upper_Plane_X + 0.003f;
		Move_Upper_Plane_Y = ((Move_Upper_Plane_X * Move_Upper_Plane_X) - 1.3f);
		
	}

	if (Move_Upper_Plane_X >= 0 && Move_Upper_Plane_X <=23.0f)
	{
		Move_Upper_Plane_X = Move_Upper_Plane_X + 0.004f;
	}

	if (Move_Upper_Plane_X >= 23.0f)
	{
		Move_Upper_Plane_X = Move_Upper_Plane_X + 0.0028f;
		Move_Upper_Plane_Y = (((Move_Upper_Plane_X - 22.999f) * (Move_Upper_Plane_X - 22.999f)) - 1.31111f);
	}
	if (Move_Letter_T_X <= 5.9f && Move_Lower_Plane_X <= 0.0f)
	{

		Move_Lower_Plane_X = Move_Lower_Plane_X + 0.003f;
		Move_Lower_Plane_Y = -((Move_Lower_Plane_X * Move_Lower_Plane_X) - 1.3f);

	}

	if (Move_Lower_Plane_X >= 0.0f && Move_Lower_Plane_X <= 23.0f)
	{
		Move_Lower_Plane_X = Move_Lower_Plane_X + 0.004f;
	}

	if (Move_Lower_Plane_X >= 23.0f)
	{
		Move_Lower_Plane_X = Move_Lower_Plane_X + 0.0028f;
		Move_Lower_Plane_Y = -(((Move_Lower_Plane_X - 22.999f) * (Move_Lower_Plane_X - 22.999f)) - 1.31111f);
	}

	//plane rotation code

	if (Rotate_Plane_Two <= 90.0f && Move_Letter_T_X <= 5.9f)
	{
		Rotate_Plane_Two = Rotate_Plane_Two + 0.06f;
	}

	if (Rotate_Plane_Three >= -90.0f && Move_Letter_T_X <= 5.9f)
	{
		Rotate_Plane_Three = Rotate_Plane_Three - 0.06f;
	}

	if (Move_Upper_Plane_X >= 23.0f && Rotate_Plane_Two <= 180.0f)
	{
		Rotate_Plane_Two = Rotate_Plane_Two + 0.05f;
	}

	if (Move_Lower_Plane_X >= 23.0f && Rotate_Plane_Three >= -180.0f)
	{
		Rotate_Plane_Three = Rotate_Plane_Three - 0.05f;
	}
	//letter T colouring

	if (Move_Middle_Plane_X >= 11.0f)
	{
		if(Red_T_Orange <= 255.0f)
		{
			Red_T_Orange = Red_T_Orange + 0.1f;
		}
		
		if (Green_T_Orange >= 103.0f)
		{
			Green_T_Orange = Green_T_Orange - 0.1f;
		}

		if (Blue_T_Orange >= 31.0f)
		{
			Blue_T_Orange = Blue_T_Orange - 0.1f;

		}
		
	}

	if (Move_Middle_Plane_X >= 11.0f)
	{
		if (Red_T_White <= 255.0f)
		{
			Red_T_White = Red_T_White + 0.1f;
		}

		if (Green_T_White <= 255.0f)
		{
			Green_T_White = Green_T_White + 0.1f;
		}

		if (Green_T_White <= 255.0f)
		{
			Blue_T_White = Blue_T_White + 0.1f;

		}
	}

	if (Move_Middle_Plane_X >= 11.0f)
	{
		if (Red_T_Green >= 4.0f)
		{
			Red_T_Green = Red_T_Green - 0.1f;
		}

		if (Green_T_Green >= 106.0f)
		{
			Green_T_Green = Green_T_Green - 0.1f;
		}

		if (Blue_T_Green >= 56.0f)
		{
			Blue_T_Green = Blue_T_Green - 0.1f;

		}
	}

	//Letter A3 Second Coloring

	if (Move_Middle_Plane_X >= 7.0f)
	{
		if (Red_A3_Second_Orange <= 255.0f)
		{
			Red_A3_Second_Orange = Red_A3_Second_Orange + 0.1f;
		}

		if (Green_A3_Second_Orange >= 103.0f)
		{
			Green_A3_Second_Orange = Green_A3_Second_Orange - 0.1f;
		}

		if (Blue_A3_Second_Orange >= 31.0f)
		{
			Blue_A3_Second_Orange = Blue_A3_Second_Orange - 0.1f;

		}

	}

	if (Move_Middle_Plane_X >= 7.0f)
	{
		if (Red_A3_Second_White <= 255.0f)
		{
			Red_A3_Second_White = Red_A3_Second_White + 0.1f;
		}

		if (Green_A3_Second_White <= 255.0f)
		{
			Green_A3_Second_White = Green_A3_Second_White + 0.1f;
		}

		if (Blue_A3_Second_White <= 255.0f)
		{
			Blue_A3_Second_White = Blue_A3_Second_White + 0.1f;

		}

	}

	if (Move_Middle_Plane_X >= 7.0f)
	{
		if (Red_A3_Second_Green >= 4.0f)
		{
			Red_A3_Second_Green = Red_A3_Second_Green - 0.1f;
		}

		if (Green_A3_Second_Green >= 106.0f)
		{
			Green_A3_Second_Green = Green_A3_Second_Green - 0.1f;
		}

		if (Blue_A3_Second_Green >= 56.0f)
		{
			Blue_A3_Second_Green = Blue_A3_Second_Green - 0.1f;

		}

	}

	//Letter A3 First Coloring

	if (Move_Middle_Plane_X >= 0.5f)
	{
		if (Red_A3_First_Orange <= 255.0f)
		{
			Red_A3_First_Orange = Red_A3_First_Orange + 0.1f;
		}

		if (Green_A3_First_Orange >= 103.0f)
		{
			Green_A3_First_Orange = Green_A3_First_Orange - 0.1f;
		}

		if (Blue_A3_First_Orange >= 31.0f)
		{
			Blue_A3_First_Orange = Blue_A3_First_Orange - 0.1f;

		}

	}

	if (Move_Middle_Plane_X >= 0.5f)
	{
		if (Red_A3_First_White <= 255.0f)
		{
			Red_A3_First_White = Red_A3_First_White + 0.1f;
		}

		if (Green_A3_First_White <= 255.0f)
		{
			Green_A3_First_White = Green_A3_First_White + 0.1f;
		}

		if (Blue_A3_First_White <= 255.0f)
		{
			Blue_A3_First_White = Blue_A3_First_White + 0.1f;

		}

	}

	if (Move_Middle_Plane_X >= 0.5f)
	{
		if (Red_A3_First_Green >= 4.0f)
		{
			Red_A3_First_Green = Red_A3_First_Green - 0.1f;
		}

		if (Green_A3_First_Green >= 106.0f)
		{
			Green_A3_First_Green = Green_A3_First_Green - 0.1f;
		}

		if (Blue_A3_First_Green >= 56.0f)
		{
			Blue_A3_First_Green = Blue_A3_First_Green - 0.1f;

		}

	}


	//Letter R colouring

	if (Move_Middle_Plane_X >= 4.5f)
	{
		if (Red_R_Orange <= 255.0f)
		{
			Red_R_Orange = Red_R_Orange + 0.1f;
		}

		if (Green_R_Orange >= 103.0f)
		{
			Green_R_Orange = Green_R_Orange - 0.1f;
		}

		if (Blue_R_Orange >= 31.0f)
		{
			Blue_R_Orange = Blue_R_Orange - 0.1f;

		}

	}

	if (Move_Middle_Plane_X >= 4.5f)
	{
		if (Red_R_White <= 255.0f)
		{
			Red_R_White = Red_R_White + 0.1f;
		}

		if (Green_R_White <= 255.0f)
		{
			Green_R_White = Green_R_White + 0.1f;
		}

		if (Blue_R_White <= 255.0f)
		{
			Blue_R_White = Blue_R_White + 0.1f;

		}

	}

	if (Move_Middle_Plane_X >= 4.5f)
	{
		if (Red_R_Green >= 4.0f)
		{
			Red_R_Green = Red_R_Green - 0.1f;
		}

		if (Green_R_Green >= 106.0f)
		{
			Green_R_Green = Green_R_Green - 0.1f;
		}

		if (Blue_R_Green >= 56.0f)
		{
			Blue_R_Green = Blue_R_Green - 0.1f;

		}

	}

	//letter H Colouring

	if (Move_Middle_Plane_X >= -3.5f)
	{
		if (Red_H_Orange <= 255.0f)
		{
			Red_H_Orange = Red_H_Orange + 0.1f;
		}

		if (Green_H_Orange >= 103.0f)
		{
			Green_H_Orange = Green_H_Orange - 0.1f;
		}

		if (Blue_H_Orange >= 31.0f)
		{
			Blue_H_Orange = Blue_H_Orange - 0.1f;

		}

	}

	if (Move_Middle_Plane_X >= -3.5f)
	{
		if (Red_H_White <= 255.0f)
		{
			Red_H_White = Red_H_White + 0.1f;
		}

		if (Green_H_White <= 255.0f)
		{
			Green_H_White = Green_H_White + 0.1f;
		}

		if (Blue_H_White <= 255.0f)
		{
			Blue_H_White = Blue_H_White + 0.1f;

		}

	}

	if (Move_Middle_Plane_X >= -3.5f)
	{
		if (Red_H_Green >= 4.0f)
		{
			Red_H_Green = Red_H_Green - 0.1f;
		}

		if (Green_H_Green >= 106.0f)
		{
			Green_H_Green = Green_H_Green - 0.1f;
		}

		if (Blue_H_Green >= 56.0f)
		{
			Blue_H_Green = Blue_H_Green - 0.1f;

		}

	}
	//letter B Colouring

	if (Move_Middle_Plane_X >= -7.5f)
	{
		if (Red_B_Orange <= 255.0f)
		{
			Red_B_Orange = Red_B_Orange + 0.1f;
		}

		if (Green_B_Orange >= 103.0f)
		{
			Green_B_Orange = Green_B_Orange - 0.1f;
		}

		if (Blue_B_Orange >= 31.0f)
		{
			Blue_B_Orange = Blue_B_Orange - 0.1f;

		}

	}

	if (Move_Middle_Plane_X >= -7.5f)
	{
		if (Red_B_White <= 255.0f)
		{
			Red_B_White = Red_B_White + 0.1f;
		}

		if (Green_B_White <= 255.0f)
		{
			Green_B_White = Green_B_White + 0.1f;
		}

		if (Blue_B_White <= 255.0f)
		{
			Blue_B_White = Blue_B_White + 0.1f;

		}

	}

	if (Move_Middle_Plane_X >= -7.5f)
	{
		if (Red_B_Green >= 4.0f)
		{
			Red_B_Green = Red_B_Green - 0.1f;
		}

		if (Green_B_Green >= 106.0f)
		{
			Green_B_Green = Green_B_Green - 0.1f;
		}

		if (Blue_B_Green >= 56.0f)
		{
			Blue_B_Green = Blue_B_Green - 0.1f;

		}

	}





}

void uninitialize(void)
{
	//local function declaration

	void uninitializeAudio(void);


    //local variable declarations

    GLXContext currentGLXContext=NULL;

    // code
	uninitializeAudio();

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

void Letter_B(void)
{ 
	
	glPushMatrix();
	
	glScalef(0.25f, 0.25f, 0.0f);
	//glTranslatef(-4.0f, 0.0f, 0.0f);
	glTranslatef(Move_Letter_B, 0.0f, 0.0f);
	//fprintf(gpFile, "Move_Letter_B = %f\n", Move_Letter_B);
	glColor3f(0.5f, 0.5f, 0.5f);
	//glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);                 //upper left quad vertical strip
	
	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_White / 256.0f), (Green_B_White / 256.0f), (Blue_B_White / 256.0f));
	}

	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));
	glVertex3f(-0.98f, 0.22f, 0.0f);

	
	glVertex3f(-0.3f, 0.22f, 0.0f);    

	
	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_Orange / 256.0f), (Green_B_Orange / 256.0f), (Blue_B_Orange / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));
	glVertex3f(-0.3f, 0.557f, 0.0f);
	
	
	glVertex3f(-0.98f, 0.99f, 0.0f);

	glEnd();

	

	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));
	glBegin(GL_QUADS);            //middle quad vertical strip


	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_White / 256.0f), (Green_B_White / 256.0f), (Blue_B_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.3f, 0.22f, 0.0f);


	glVertex3f(-0.98f, 0.22f, 0.0f);

	
	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_White / 256.0f), (Green_B_White / 256.0f), (Blue_B_White / 256.0f));
	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(-0.98f, -0.175f, 0.0f);

	glVertex3f(-0.3f, -0.175f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);            //lower quad vertical strip

	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_White / 256.0f), (Green_B_White / 256.0f), (Blue_B_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));
	glVertex3f(-0.3f, -0.175f, 0.0f);


	glVertex3f(-0.98f, -0.175f, 0.0f);

	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_Green / 256.0f), (Green_B_Green / 256.0f), (Blue_B_Green / 256.0f));
	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));
	glVertex3f(-0.98f, -0.99f, 0.0f);

	glVertex3f(-0.3f, -0.595f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);            //upper horizontal quad 

	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_Orange / 256.0f), (Green_B_Orange / 256.0f), (Blue_B_Orange / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.54f, 0.99f, 0.0f);


	glVertex3f(-0.98f, 0.99f, 0.0f);


	glVertex3f(-0.3f, 0.557f, 0.0f);

	glVertex3f(0.17f, 0.557f, 0.0f);

	glEnd();



	glBegin(GL_QUADS);            //lower horizontal quad 

	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_Green / 256.0f), (Green_B_Green / 256.0f), (Blue_B_Green / 256.0f));
	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));
	glVertex3f(0.17f, -0.594f, 0.0f);


	glVertex3f(-0.3f, -0.594f, 0.0f);


	glVertex3f(-0.98f, -0.99f, 0.0f);

	glVertex3f(0.54f, -0.99f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);            //upper slant quad 

	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_Orange / 256.0f), (Green_B_Orange / 256.0f), (Blue_B_Orange / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));
	glVertex3f(0.98f, 0.7f, 0.0f);


	glVertex3f(0.54f, 0.99f, 0.0f);


	glVertex3f(0.17f, 0.557f, 0.0f);

	glVertex3f(0.3f, 0.49f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);            //upper verticle quad 

	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_Orange / 256.0f), (Green_B_Orange / 256.0f), (Blue_B_Orange / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.98f, 0.7f, 0.0f);


	glVertex3f(0.3f, 0.49f, 0.0f);


	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_White / 256.0f), (Green_B_White / 256.0f), (Blue_B_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.3f, 0.3f, 0.0f);

	glVertex3f(0.98f, 0.2f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);            //upper slant quad triangle adjecent 

	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_White / 256.0f), (Green_B_White / 256.0f), (Blue_B_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));
	glVertex3f(0.98f, 0.2f, 0.0f);


	glVertex3f(0.3f, 0.3f, 0.0f);

	

	glVertex3f(0.19f, 0.22f, 0.0f);

	glVertex3f(0.7f, 0.02f, 0.0f);

	glEnd();

	glBegin(GL_TRIANGLES);            //middle triangle

	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_White / 256.0f), (Green_B_White / 256.0f), (Blue_B_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.7f, 0.02f, 0.0f);
	glVertex3f(0.19f, 0.22f, 0.0f);
	glVertex3f(0.19f, -0.17f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);            //lower slant quad triangle adjecent

	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_White / 256.0f), (Green_B_White / 256.0f), (Blue_B_White / 256.0f));
	}

	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.7f, 0.02f, 0.0f);


	glVertex3f(0.19f, -0.17f, 0.0f);


	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_White / 256.0f), (Green_B_White / 256.0f), (Blue_B_White / 256.0f));
	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(0.3f, -0.26f, 0.0f);

	glVertex3f(0.98f, -0.16f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);            //lower vertical quad triangle adjecent 

	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_White / 256.0f), (Green_B_White / 256.0f), (Blue_B_White / 256.0f));
	}

	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.98f, -0.16f, 0.0f);


	glVertex3f(0.3f, -0.26f, 0.0f);

	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_Green / 256.0f), (Green_B_Green / 256.0f), (Blue_B_Green / 256.0f));
	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(0.3f, -0.5f, 0.0f);

	glVertex3f(0.98f, -0.72f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);            //lower slant quad


	glVertex3f(0.98f, -0.72f, 0.0f);


	glVertex3f(0.3f, -0.5f, 0.0f);


	glVertex3f(0.17f, -0.595f, 0.0f);

	glVertex3f(0.54f, -0.99f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);            //horizontal middle quad

	if (Move_Middle_Plane_X >= -7.5f)
	{
		glColor3f((Red_B_White / 256.0f), (Green_B_White / 256.0f), (Blue_B_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.19f, 0.22f, 0.0f);


	glVertex3f(-0.3f, 0.22f, 0.0f);


	glVertex3f(-0.3f, -0.175f, 0.0f);

	glVertex3f(0.19f, -0.17f, 0.0f);

	glEnd();

	glPopMatrix();
}
void Letter_H(void)
{
	glPushMatrix();
	glScalef(0.24f, 0.245f, 0.0f);
	//glTranslatef(-1.7f, -0.01f, 0.0f);
	glTranslatef(Move_Letter_H_X, Move_Letter_H_Y, 0.0f);
	//glColor3f(1.0f, 0.0f, 0.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_QUADS);                //left side upper quad

	if (Move_Middle_Plane_X >= -3.5f)
	{
		glColor3f((Red_H_Orange / 256.0f), (Green_H_Orange / 256.0f), (Blue_H_Orange / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(-0.4f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);

	if (Move_Middle_Plane_X >= -3.5f)
	{
		glColor3f((Red_H_White / 256.0f), (Green_H_White / 256.0f), (Blue_H_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-1.0f, 0.2f, 0.0f);
	glVertex3f(-0.4f, 0.2f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);                //left middle  quad

	if (Move_Middle_Plane_X >= -3.5f)
	{
		glColor3f((Red_H_White / 256.0f), (Green_H_White / 256.0f), (Blue_H_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.4f, 0.2f, 0.0f);
	glVertex3f(-1.0f, 0.2f, 0.0f);
	glVertex3f(-1.0f, -0.2f, 0.0f);
	glVertex3f(-0.4f, -0.2f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);                //left lower quad

	if (Move_Middle_Plane_X >= -3.5f)
	{
		glColor3f((Red_H_White / 256.0f), (Green_H_White / 256.0f), (Blue_H_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.4f, -0.2f, 0.0f);
	glVertex3f(-1.0f, -0.2f, 0.0f);

	if (Move_Middle_Plane_X >= -3.5f)
	{
		glColor3f((Red_H_Green / 256.0f), (Green_H_Green / 256.0f), (Blue_H_Green / 256.0f));
	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(-0.4f, -1.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);                //middle horizontal line

	if (Move_Middle_Plane_X >= -3.5f)
	{
		glColor3f((Red_H_White / 256.0f), (Green_H_White / 256.0f), (Blue_H_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.4f, 0.2f, 0.0f);
	glVertex3f(-0.4f, 0.2f, 0.0f);
	glVertex3f(-0.4f, -0.2f, 0.0f);
	glVertex3f(0.4f, -0.2f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);                //right side upper quad

	if (Move_Middle_Plane_X >= -3.5f)
	{
		glColor3f((Red_H_Orange / 256.0f), (Green_H_Orange / 256.0f), (Blue_H_Orange / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.4f, 1.0f, 0.0f);

	if (Move_Middle_Plane_X >= -3.5f)
	{
		glColor3f((Red_H_White / 256.0f), (Green_H_White / 256.0f), (Blue_H_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.4f, 0.2f, 0.0f);
	glVertex3f(1.0f, 0.2f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);                //right side middle quad
	
	if (Move_Middle_Plane_X >= -3.5f)
	{
		glColor3f((Red_H_White / 256.0f), (Green_H_White / 256.0f), (Blue_H_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(1.0f, 0.2f, 0.0f);
	glVertex3f(0.4f, 0.2f, 0.0f);
	glVertex3f(0.4f, -0.2f, 0.0f);
	glVertex3f(1.0f, -0.2f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);                //right side lower quad

	if (Move_Middle_Plane_X >= -3.5f)
	{
		glColor3f((Red_H_White / 256.0f), (Green_H_White / 256.0f), (Blue_H_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(1.0f, -0.2f, 0.0f);
	glVertex3f(0.4f, -0.2f, 0.0f);

	if (Move_Middle_Plane_X >= -3.5f)
	{
		glColor3f((Red_H_Green / 256.0f), (Green_H_Green / 256.0f), (Blue_H_Green / 256.0f));
	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(0.4f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glEnd();
	glPopMatrix();
}
void Letter_A1(void)
{
	glPushMatrix();
	//glScalef(0.24f, 0.25f, 0.0f);
	 glScalef(0.24f, 0.260f, 0.0f);
	//glTranslatef(0.6f, -0.01f, 0.0f);
	//glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(Move_Letter_A_X, Move_Letter_A_Y, 0.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_TRIANGLES);           //middle triangle

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_Orange / 256.0f), (Green_A3_First_Orange / 256.0f), (Blue_A3_First_Orange / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.25f, 1.0f, 0.0f);
	glVertex3f(-0.25f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.3f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);               //left side slant quad

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_Orange / 256.0f), (Green_A3_First_Orange / 256.0f), (Blue_A3_First_Orange / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.0f, 0.3f, 0.0f);
	glVertex3f(-0.25f, 1.0f, 0.0f);

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.68f, -0.14f, 0.0f);
	glVertex3f(-0.16f, -0.14f, 0.0f);
	
	glEnd();

	glBegin(GL_QUADS);               //left side slant quad adjecent horizontal quad 

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.16f, -0.14f, 0.0f);
	glVertex3f(-0.68f, -0.14f, 0.0f);
	glVertex3f(-0.84f, -0.56f, 0.0f);
	glVertex3f(-0.32f, -0.56f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);               //left side slant quad bottom

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.32f, -0.56f, 0.0f);
	glVertex3f(-0.84f, -0.56f, 0.0f);

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_Green / 256.0f), (Green_A3_First_Green / 256.0f), (Blue_A3_First_Green / 256.0f));

	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(-0.5f, -1.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);              //right side slant quad upper

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_Orange / 256.0f), (Green_A3_First_Orange / 256.0f), (Blue_A3_First_Orange / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.25f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.3f, 0.0f);

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.16f, -0.14f, 0.0f);
	glVertex3f(0.68f, -0.14f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);               //right side slant quad adjecent to horizontal quad

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.68f, -0.14f, 0.0f);
	glVertex3f(0.16f, -0.14f, 0.0f);
	glVertex3f(0.32f, -0.56f, 0.0f);
	glVertex3f(0.84f, -0.56f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);               //right side slant quad bottom

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.84f, -0.56f, 0.0f);
	glVertex3f(0.32f, -0.56f, 0.0f);

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_Green / 256.0f), (Green_A3_First_Green / 256.0f), (Blue_A3_First_Green / 256.0f));

	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(0.5f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);               //Middle horizontal quad

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.16f, -0.14f, 0.0f);
	glVertex3f(-0.16f, -0.14f, 0.0f);
	glVertex3f(-0.32f, -0.56f, 0.0f);
	glVertex3f(0.32f, -0.56f, 0.0f);
	glEnd();
	glPopMatrix();
}
void Letter_A2(void)
{
	glPushMatrix();
	//glScalef(0.24f, 0.25f, 0.0f);
	glScalef(0.24f, 0.260f, 0.0f);
	//glTranslatef(5.0f, 0.0f, 0.0f);
	//glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(Move_Letter_A2_X, Move_Letter_A2_Y, 0.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_TRIANGLES);           //middle triangle

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_Orange / 256.0f), (Green_A3_Second_Orange / 256.0f), (Blue_A3_Second_Orange / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.25f, 1.0f, 0.0f);
	glVertex3f(-0.25f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.3f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);               //left side slant quad

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_Orange / 256.0f), (Green_A3_Second_Orange / 256.0f), (Blue_A3_Second_Orange / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.0f, 0.3f, 0.0f);
	glVertex3f(-0.25f, 1.0f, 0.0f);

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.68f, -0.14f, 0.0f);
	glVertex3f(-0.16f, -0.14f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);               //left side slant quad adjecent horizontal quad 

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.16f, -0.14f, 0.0f);
	glVertex3f(-0.68f, -0.14f, 0.0f);
	glVertex3f(-0.84f, -0.56f, 0.0f);
	glVertex3f(-0.32f, -0.56f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);               //left side slant quad bottom

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));
	}

	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.32f, -0.56f, 0.0f);
	glVertex3f(-0.84f, -0.56f, 0.0f);

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_Green / 256.0f), (Green_A3_Second_Green / 256.0f), (Blue_A3_Second_Green / 256.0f));
	}

	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(-0.5f, -1.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);              //right side slant quad upper


	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_Orange / 256.0f), (Green_A3_Second_Orange / 256.0f), (Blue_A3_Second_Orange / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.25f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.3f, 0.0f);

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.16f, -0.14f, 0.0f);
	glVertex3f(0.68f, -0.14f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);               //right side slant quad adjecent to horizontal quad

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.68f, -0.14f, 0.0f);
	glVertex3f(0.16f, -0.14f, 0.0f);
	glVertex3f(0.32f, -0.56f, 0.0f);
	glVertex3f(0.84f, -0.56f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);               //right side slant quad bottom

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.84f, -0.56f, 0.0f);
	glVertex3f(0.32f, -0.56f, 0.0f);

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_Green / 256.0f), (Green_A3_Second_Green / 256.0f), (Blue_A3_Second_Green / 256.0f));
	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(0.5f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);               //Middle horizontal quad

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.16f, -0.14f, 0.0f);
	glVertex3f(-0.16f, -0.14f, 0.0f);
	glVertex3f(-0.32f, -0.56f, 0.0f);
	glVertex3f(0.32f, -0.56f, 0.0f);
	glEnd();
	glPopMatrix();
}
void Letter_T(void)
{
	glPushMatrix();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glScalef(0.3f, 0.249f, 0.0f);
	//glTranslatef(5.89f, -0.01f, 0.0f);
	glTranslatef(Move_Letter_T_X, -0.01f, 0.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_QUADS);           //Horizontal quad

	if (Move_Middle_Plane_X >= 11.0f)
	{
		glColor3f((Red_T_Orange / 256.0f), (Green_T_Orange / 256.0f), (Blue_T_Orange / 256.0f));

	}

	glVertex3f(0.95f, 1.0f, 0.0f);
	glVertex3f(-0.95f, 1.0f, 0.0f);
	glVertex3f(-0.95f, 0.6f, 0.0f);
	glVertex3f(0.95f, 0.6f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);           //Vertical quad upper

	if (Move_Middle_Plane_X >= 11.0f)
	{
		glColor3f((Red_T_Orange / 256.0f), (Green_T_Orange / 256.0f), (Blue_T_Orange / 256.0f));

	}

	glVertex3f(0.2f, 0.6f, 0.0f);
	glVertex3f(-0.2f, 0.6f, 0.0f);

	if (Move_Middle_Plane_X >= 11.0f)
	{
		glColor3f((Red_T_White / 256.0f), (Green_T_White / 256.0f), (Blue_T_White / 256.0f));

	}

	glVertex3f(-0.2f, 0.2f, 0.0f);
	glVertex3f(0.2f, 0.2f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	
	if (Move_Middle_Plane_X >= 11.0f)
	{
		glColor3f((Red_T_White / 256.0f), (Green_T_White / 256.0f), (Blue_T_White / 256.0f));

	}

	glVertex3f(0.2f, 0.2f, 0.0f);
	glVertex3f(-0.2f, 0.2f, 0.0f);
	glVertex3f(-0.2f, -0.22f, 0.0f);
	glVertex3f(0.2f, -0.22f, 0.0f);
	glEnd();
	glBegin(GL_QUADS);           //Vertical quad bottom

	if (Move_Middle_Plane_X >= 11.0f)
	{
		glColor3f((Red_T_White / 256.0f), (Green_T_White / 256.0f), (Blue_T_White / 256.0f));

	}

	glVertex3f(0.2f, -0.22f, 0.0f);
	glVertex3f(-0.2f, -0.22f, 0.0f);
	
	if (Move_Middle_Plane_X >= 11.0f)
	{
		glColor3f((Red_T_Green / 256.0f), (Green_T_Green / 256.0f), (Blue_T_Green / 256.0f));

	}

	glVertex3f(-0.2f, -1.0f, 0.0f);
	glVertex3f(0.2f, -1.0f, 0.0f);
	glEnd();

	glDisable(GL_BLEND);
	glPopMatrix();
}
void Letter_R(void)
{
	glPushMatrix();

	glScalef(0.32f, 0.29f, 0.0f);
	//glTranslatef(2.1f, -0.01f, 0.0f);
	glTranslatef(Move_Letter_R_X, Move_Letter_R_Y, 0.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_QUADS);           //left side upper vertical quad

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_Orange / 256.0f), (Green_R_Orange / 256.0f), (Blue_R_Orange / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(-0.35f, 0.9f, 0.0f);
	glVertex3f(-0.7f, 0.9f, 0.0f);

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_White / 256.0f), (Green_R_White / 256.0f), (Blue_R_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.7f, 0.15f, 0.0f);
	glVertex3f(-0.35f, 0.15f, 0.0f);
	glEnd();
	//glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);           //left side middle vertical quad


	/*glVertex3f(-0.35f, 0.15f, 0.0f);
	glVertex3f(-0.7f, 0.15f, 0.0f);
	glVertex3f(-0.7f, -0.1f, 0.0f);
	glVertex3f(-0.35f, -0.1f, 0.0f);*/

	glVertex3f(-0.35f, 0.18f, 0.0f);
	glVertex3f(-0.7f, 0.18f, 0.0f);
	glVertex3f(-0.7f, -0.13f, 0.0f);
	glVertex3f(-0.35f, -0.13f, 0.0f);
	glEnd();
	//glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);           //left side bottom vertical quad

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_White / 256.0f), (Green_R_White / 256.0f), (Blue_R_White / 256.0f));
	}

	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.35f, -0.13f, 0.0f);
	glVertex3f(-0.7f, -0.13f, 0.0f);

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_Green / 256.0f), (Green_R_Green / 256.0f), (Blue_R_Green / 256.0f));
	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(-0.7f, -0.9f, 0.0f);
	glVertex3f(-0.35f, -0.9f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);             // upper horizontal quad

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_Orange / 256.0f), (Green_R_Orange / 256.0f), (Blue_R_Orange / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.4f, 0.9f, 0.0f);
	glVertex3f(-0.35f, 0.9f, 0.0f);
	glVertex3f(-0.35f, 0.65f, 0.0f);
	glVertex3f(0.2f, 0.65f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);             // right side slant quad upper

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_Orange / 256.0f), (Green_R_Orange / 256.0f), (Blue_R_Orange / 256.0f));
	}
//	glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.4f, 0.9f, 0.0f);
	glVertex3f(0.2f, 0.65f, 0.0f);
	glVertex3f(0.35f, 0.55f, 0.0f);
	glVertex3f(0.7f, 0.62f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);             // right side verticle quad 

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_Orange / 256.0f), (Green_R_Orange / 256.0f), (Blue_R_Orange / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.7f, 0.62f, 0.0f);
	glVertex3f(0.35f, 0.55f, 0.0f);
	glVertex3f(0.35f, 0.25f, 0.0f);
	glVertex3f(0.7f, 0.25f, 0.0f);
	glEnd();
	//glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);             // right side slant quad adjecent to triangle
	/*glVertex3f(0.7f, 0.25f, 0.0f);
	glVertex3f(0.35f, 0.25f, 0.0f);
	glVertex3f(0.2f, 0.15f, 0.0f);
	glVertex3f(0.5f, 0.05f, 0.0f);*/

	glVertex3f(0.7f, 0.25f, 0.0f);
	glVertex3f(0.35f, 0.25f, 0.0f);

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_White / 256.0f), (Green_R_White / 256.0f), (Blue_R_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.2f, 0.18f, 0.0f);
	glVertex3f(0.5f, 0.05f, 0.0f);
	glEnd();
	//glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLES);             //  Central Triangle

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_White / 256.0f), (Green_R_White / 256.0f), (Blue_R_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.2f, 0.18f, 0.0f);
	glVertex3f(0.5f, 0.05f, 0.0f);
	glVertex3f(0.2f, -0.13f, 0.0f);
	
	glEnd();
	//glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);             // right side slant quad adjecent to triangle bottom

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_White / 256.0f), (Green_R_White / 256.0f), (Blue_R_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.5f, 0.05f, 0.0f);
	glVertex3f(0.2f, -0.1f, 0.0f);

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_White / 256.0f), (Green_R_White / 256.0f), (Blue_R_White / 256.0f));
	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(0.34f, -0.2f, 0.0f);
	glVertex3f(0.7f, -0.14f, 0.0f);
	glEnd();
	//glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);             // right side verticle quad bottom

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_White / 256.0f), (Green_R_White / 256.0f), (Blue_R_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.7f, -0.14f, 0.0f);
	glVertex3f(0.34f, -0.2f, 0.0f);

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_Green / 256.0f), (Green_R_Green / 256.0f), (Blue_R_Green / 256.0f));
	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(0.34f, -0.9f, 0.0f);
	glVertex3f(0.7f, -0.9f, 0.0f);
	glEnd();
	//glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);             // Central horizontal quad
	/*glVertex3f(0.2f, 0.15f, 0.0f);
	glVertex3f(-0.35f, 0.15f, 0.0f);
	glVertex3f(-0.35f, -0.1f, 0.0f);
	glVertex3f(0.2f, -0.1f, 0.0f);*/

	if (Move_Middle_Plane_X >= 4.5f)
	{
		glColor3f((Red_R_White / 256.0f), (Green_R_White / 256.0f), (Blue_R_White / 256.0f));
	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.2f, 0.18f, 0.0f);
	glVertex3f(-0.35f, 0.18f, 0.0f);
	glVertex3f(-0.35f, -0.13f, 0.0f);
	glVertex3f(0.2f, -0.13f, 0.0f);
	glEnd();
	glPopMatrix();
}
void Letter_A3_First(void)
{
	glPushMatrix();
	glScalef(0.24f, 0.260f, 0.0f);
	glTranslatef(Move_Letter_A_X, Move_Letter_A_Y, 0.0f);
	//glColor3f(1.0f, 0.0f, 0.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_QUADS);                   //left side bottom vertical quad

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.2f, -0.2f, 0.0f); //right top
	glVertex3f(-0.9f, -0.2f, 0.0f); //left top

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_Green / 256.0f), (Green_A3_First_Green / 256.0f), (Blue_A3_First_Green / 256.0f));

	}

	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));


	glVertex3f(-0.9f, -1.0f, 0.0f); //left bottom
	glVertex3f(-0.2f, -1.0f, 0.0f); //right bottom
	glEnd();

	glBegin(GL_QUADS);                //left side middle vertical quad

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.2f, 0.2f, 0.0f); //right top
	glVertex3f(-0.9f, 0.2f, 0.0f); //left top
	glVertex3f(-0.9f, -0.2f, 0.0f); //left bottom
	glVertex3f(-0.2f, -0.2f, 0.0f); //right bottom
	glEnd();
	//glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);                      //left side upper  quad

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_Orange / 256.0f), (Green_A3_First_Orange / 256.0f), (Blue_A3_First_Orange / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(-0.2f, 0.95f, 0.0f); //right top
	glVertex3f(-0.9f, 0.95f, 0.0f); //left top

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.9f, 0.2f, 0.0f); //left bottom
	glVertex3f(-0.2f, 0.2f, 0.0f); //right bottom
	glEnd();

	glBegin(GL_QUADS);                      //upper horizontal quad

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_Orange / 256.0f), (Green_A3_First_Orange / 256.0f), (Blue_A3_First_Orange / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.9f, 0.95f, 0.0f); //right top
	glVertex3f(-0.2f, 0.95f, 0.0f); //left top
	glVertex3f(-0.2f, 0.6f, 0.0f); //left bottom
	glVertex3f(0.9f, 0.6f, 0.0f); //right bottom
	glEnd();

	glBegin(GL_QUADS);                      //right side upper vertical quad

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_Orange / 256.0f), (Green_A3_First_Orange / 256.0f), (Blue_A3_First_Orange / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.9f, 0.6f, 0.0f); //right top
	glVertex3f(0.2f, 0.6f, 0.0f); //left top

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.2f, 0.2f, 0.0f); //left bottom
	glVertex3f(0.9f, 0.2f, 0.0f); //right bottom
	glEnd();

	glBegin(GL_QUADS);                      //right side middle vertical quad

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.9f, 0.2f, 0.0f); //right top
	glVertex3f(0.2f, 0.2f, 0.0f); //left top
	glVertex3f(0.2f, -0.2f, 0.0f); //left bottom
	glVertex3f(0.9f, -0.2f, 0.0f); //right bottom
	glEnd();
	//glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);                      //right side bottom vertical quad

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.9f, -0.2f, 0.0f); //right top
	glVertex3f(0.2f, -0.2f, 0.0f); //left top

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_Green / 256.0f), (Green_A3_First_Green / 256.0f), (Blue_A3_First_Green / 256.0f));

	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(0.2f, -1.0f, 0.0f); //left bottom
	glVertex3f(0.9f, -1.0f, 0.0f); //right bottom
	glEnd();
	
	glBegin(GL_QUADS);                      //middle horizontal quad

	if (Move_Middle_Plane_X >= 0.5f)
	{
		glColor3f((Red_A3_First_White / 256.0f), (Green_A3_First_White / 256.0f), (Blue_A3_First_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.2f, 0.2f, 0.0f); //right top
	glVertex3f(-0.2f, 0.2f, 0.0f); //left top
	glVertex3f(-0.2f, -0.2f, 0.0f); //left bottom
	glVertex3f(0.2f, -0.2f, 0.0f); //right bottom
	glEnd();

	glPopMatrix();
}
void Letter_A3_Second(void)
{
	glPushMatrix();
	glScalef(0.24f, 0.260f, 0.0f);
	//glTranslatef(5.0f, -0.01f, 0.0f);
	glTranslatef(Move_Letter_A2_X, Move_Letter_A2_Y, 0.0f);
	//glColor3f(1.0f, 0.0f, 0.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_QUADS);                   //left side bottom vertical quad

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.2f, -0.2f, 0.0f); //right top
	glVertex3f(-0.9f, -0.2f, 0.0f); //left top

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_Green / 256.0f), (Green_A3_Second_Green / 256.0f), (Blue_A3_Second_Green / 256.0f));

	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));


	glVertex3f(-0.9f, -1.0f, 0.0f); //left bottom
	glVertex3f(-0.2f, -1.0f, 0.0f); //right bottom
	glEnd();

	glBegin(GL_QUADS);                //left side middle vertical quad

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.2f, 0.2f, 0.0f); //right top
	glVertex3f(-0.9f, 0.2f, 0.0f); //left top
	glVertex3f(-0.9f, -0.2f, 0.0f); //left bottom
	glVertex3f(-0.2f, -0.2f, 0.0f); //right bottom
	glEnd();
	//glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);                      //left side upper  quad

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_Orange / 256.0f), (Green_A3_Second_Orange / 256.0f), (Blue_A3_Second_Orange / 256.0f));

	}

	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(-0.2f, 0.95f, 0.0f); //right top
	glVertex3f(-0.9f, 0.95f, 0.0f); //left top

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(-0.9f, 0.2f, 0.0f); //left bottom
	glVertex3f(-0.2f, 0.2f, 0.0f); //right bottom
	glEnd();

	glBegin(GL_QUADS);                      //upper horizontal quad

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_Orange / 256.0f), (Green_A3_Second_Orange / 256.0f), (Blue_A3_Second_Orange / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.9f, 0.95f, 0.0f); //right top
	glVertex3f(-0.2f, 0.95f, 0.0f); //left top
	glVertex3f(-0.2f, 0.6f, 0.0f); //left bottom
	glVertex3f(0.9f, 0.6f, 0.0f); //right bottom
	glEnd();

	glBegin(GL_QUADS);                      //right side upper vertical quad

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_Orange / 256.0f), (Green_A3_Second_Orange / 256.0f), (Blue_A3_Second_Orange / 256.0f));

	}

	//glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.9f, 0.6f, 0.0f); //right top
	glVertex3f(0.2f, 0.6f, 0.0f); //left top

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.2f, 0.2f, 0.0f); //left bottom
	glVertex3f(0.9f, 0.2f, 0.0f); //right bottom
	glEnd();

	glBegin(GL_QUADS);                      //right side middle vertical quad

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.9f, 0.2f, 0.0f); //right top
	glVertex3f(0.2f, 0.2f, 0.0f); //left top
	glVertex3f(0.2f, -0.2f, 0.0f); //left bottom
	glVertex3f(0.9f, -0.2f, 0.0f); //right bottom
	glEnd();
	//glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);                      //right side bottom vertical quad

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.9f, -0.2f, 0.0f); //right top
	glVertex3f(0.2f, -0.2f, 0.0f); //left top

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_Green / 256.0f), (Green_A3_Second_Green / 256.0f), (Blue_A3_Second_Green / 256.0f));

	}
	//glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(0.2f, -1.0f, 0.0f); //left bottom
	glVertex3f(0.9f, -1.0f, 0.0f); //right bottom
	glEnd();

	glBegin(GL_QUADS);                      //middle horizontal quad

	if (Move_Middle_Plane_X >= 7.0f)
	{
		glColor3f((Red_A3_Second_White / 256.0f), (Green_A3_Second_White / 256.0f), (Blue_A3_Second_White / 256.0f));

	}
	//glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.2f, 0.2f, 0.0f); //right top
	glVertex3f(-0.2f, 0.2f, 0.0f); //left top
	glVertex3f(-0.2f, -0.2f, 0.0f); //left bottom
	glVertex3f(0.2f, -0.2f, 0.0f); //right bottom
	glEnd();

	glPopMatrix();
	
}
void Plane1(void)
{
	glPushMatrix();
	glScalef(0.16f, 0.18f, 0.0f);
	glTranslatef(Move_Middle_Plane_X, 0.0f, 0.0f);
	glRotatef(270, 0.0f, 0.0f, 1.0f);
	glColor3f((183.0f / 260.0f), (212.0f / 260.0f), (218.0f / 260.0f));

	glBegin(GL_QUADS);                     //left tail


	glVertex3f(-0.08f, -0.36f, 0.0f);


	glVertex3f(-0.24f, -0.56f, 0.0f);


	glVertex3f(-0.22f, -0.62f, 0.0f);


	glVertex3f(-0.06f, -0.52f, 0.0f);

	//right tail

	glVertex3f(0.08f, -0.36f, 0.0f);


	glVertex3f(0.24f, -0.56f, 0.0f);


	glVertex3f(0.22f, -0.62f, 0.0f);


	glVertex3f(0.06f, -0.52f, 0.0f);

	//middle quad between tails
	glVertex3f(-0.08f, -0.36f, 0.0f);


	glVertex3f(-0.06f, -0.52f, 0.0f);


	glVertex3f(0.06f, -0.52f, 0.0f);


	glVertex3f(0.08f, -0.36f, 0.0f);

	glEnd();

	//tail middle triangle
	glColor3f((111.0f / 260.0f), (111.0f / 260.0f), (119.0f / 260.0f));

	glBegin(GL_TRIANGLES);

	glVertex3f(0.06f, -0.52f, 0.0f);
	glVertex3f(-0.06f, -0.52f, 0.0f);
	glVertex3f(0.0f, -0.6f, 0.0f);

	glEnd();

	//plane middle part quad
	//rgb(101, 170, 199)
	glColor3f((101.0f / 260.0f), (170.0f / 260.0f), (199.0f / 260.0f));
	glBegin(GL_QUADS);

	glVertex3f(0.12f, 0.46f, 0.0f);
	glVertex3f(-0.12f, 0.46f, 0.0f);
	glVertex3f(-0.08f, -0.36f, 0.0f);
	glVertex3f(0.08f, -0.36f, 0.0f);

	//left wing
	//rgb(183, 212, 218)
	glColor3f((183.0f / 260.0f), (212.0f / 260.0f), (218.0f / 260.0f));

	glVertex3f(-0.1f, 0.16f, 0.0f);
	glVertex3f(-0.45f, -0.12f, 0.0f);
	glVertex3f(-0.48f, -0.35f, 0.0f);
	glVertex3f(-0.08f, -0.29f, 0.0f);

	//right wing
	//rgb(183, 212, 218)
	glColor3f((183.0f / 260.0f), (212.0f / 260.0f), (218.0f / 260.0f));

	glVertex3f(0.1f, 0.16f, 0.0f);
	glVertex3f(0.45f, -0.12f, 0.0f);
	glVertex3f(0.48f, -0.35f, 0.0f);
	glVertex3f(0.08f, -0.29f, 0.0f);

	//left wing outer quad
	//rgb(111, 111, 119)
	glColor3f((111.0f / 260.0f), (111.0f / 260.0f), (119.0f / 260.0f));

	glVertex3f(-0.52f, 0.08f, 0.0f);
	glVertex3f(-0.52f, -0.4f, 0.0f);
	glVertex3f(-0.48f, -0.35f, 0.0f);
	glVertex3f(-0.45f, -0.12f, 0.0f);

	//right wing outer quad
	//rgb(111, 111, 119)
	glColor3f((111.0f / 260.0f), (111.0f / 260.0f), (119.0f / 260.0f));

	glVertex3f(0.52f, 0.08f, 0.0f);
	glVertex3f(0.52f, -0.4f, 0.0f);
	glVertex3f(0.48f, -0.35f, 0.0f);
	glVertex3f(0.45f, -0.12f, 0.0f);


	glEnd();

	//upper Triangle

	glBegin(GL_TRIANGLES);

	glVertex3f(-0.06f, 0.46f, 0.0f);
	glVertex3f(0.0f, 0.9f, 0.0f);
	glVertex3f(0.06f, 0.46f, 0.0f);

	glEnd();

	glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glBegin(GL_LINES);  //orange circle                                


	float Radious_Orange = 0.06f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = Radious_Orange * cos(angle) + (0.0f);
		float y = Radious_Orange * sin(angle) + (-0.25f);



		// Circle Origin


		glVertex3f(0.0, -0.25, 0.0f);



		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));
	glBegin(GL_LINES);  //white circle                                


	float Radious_White = 0.04f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = Radious_White * cos(angle) + (0.0f);
		float y = Radious_White * sin(angle) + (-0.25f);



		// Circle Origin


		glVertex3f(0.0, -0.25, 0.0f);



		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));
	glBegin(GL_LINES);  //Green circle                                


	float Radious_Green = 0.02f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = Radious_Green * cos(angle) + (0.0f);
		float y = Radious_Green * sin(angle) + (-0.25f);



		// Circle Origin


		glVertex3f(0.0, -0.25, 0.0f);



		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	//engine fire of plane

	glBegin(GL_QUADS);
	glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));

	glVertex3f(0.13f, -0.62f, 0.0f);
	glVertex3f(-0.13f, -0.62f, 0.0f);
	glVertex3f(-0.13f, -0.64f, 0.0f);
	glVertex3f(0.13f, -0.64f, 0.0f);

	glEnd();

	glBegin(GL_TRIANGLES);

	glVertex3f(-0.0434f, -0.64f, 0.0f);   //right base point
	glVertex3f(-0.13f, -0.64f, 0.0f);     //left base point
	//glVertex3f(-0.08667f, -0.68f, 0.0f);  //apex point
	glVertex3f(-0.05f, -0.70f, 0.0f);  //apex point



	glEnd();
	glBegin(GL_TRIANGLES);

	glVertex3f(0.0432f, -0.64f, 0.0f);   //right base point
	glVertex3f(-0.0434f, -0.64f, 0.0f);     //left base point
	glVertex3f(-0.0001f, -0.9f, 0.0f);  //apex point


	glEnd();

	glBegin(GL_TRIANGLES);

	glVertex3f(0.1298f, -0.64f, 0.0f);   //right base point
	glVertex3f(0.0432f, -0.64f, 0.0f);  //left base point
	//glVertex3f(0.0865, -0.68f, 0.0f);  //apex point
	glVertex3f(0.05f, -0.70f, 0.0f);

	glEnd();
	
	glPopMatrix();
}
void Plane2(void)
{
	glPushMatrix();
	glScalef(0.16f, 0.18f, 0.0f);
	
	glTranslatef(-9.0f, 2.5f, 0.0f);
	
	//glRotatef(Rotate_Plane_Two, 0.0f, 0.0f, 1.0f);
	
	
	
	glTranslatef(Move_Upper_Plane_X, Move_Upper_Plane_Y, 0.0f);
	glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(Rotate_Plane_Two, 0.0f, 0.0f, 1.0f);
	glColor3f((183.0f / 260.0f), (212.0f / 260.0f), (218.0f / 260.0f));

	glBegin(GL_QUADS);                     //left tail


	glVertex3f(-0.08f, -0.36f, 0.0f);


	glVertex3f(-0.24f, -0.56f, 0.0f);


	glVertex3f(-0.22f, -0.62f, 0.0f);


	glVertex3f(-0.06f, -0.52f, 0.0f);

	//right tail

	glVertex3f(0.08f, -0.36f, 0.0f);


	glVertex3f(0.24f, -0.56f, 0.0f);


	glVertex3f(0.22f, -0.62f, 0.0f);


	glVertex3f(0.06f, -0.52f, 0.0f);

	//middle quad between tails
	glVertex3f(-0.08f, -0.36f, 0.0f);


	glVertex3f(-0.06f, -0.52f, 0.0f);


	glVertex3f(0.06f, -0.52f, 0.0f);


	glVertex3f(0.08f, -0.36f, 0.0f);

	glEnd();

	//tail middle triangle
	glColor3f((111.0f / 260.0f), (111.0f / 260.0f), (119.0f / 260.0f));

	glBegin(GL_TRIANGLES);

	glVertex3f(0.06f, -0.52f, 0.0f);
	glVertex3f(-0.06f, -0.52f, 0.0f);
	glVertex3f(0.0f, -0.6f, 0.0f);

	glEnd();

	//plane middle part quad
	//rgb(101, 170, 199)
	glColor3f((101.0f / 260.0f), (170.0f / 260.0f), (199.0f / 260.0f));
	glBegin(GL_QUADS);

	glVertex3f(0.12f, 0.46f, 0.0f);
	glVertex3f(-0.12f, 0.46f, 0.0f);
	glVertex3f(-0.08f, -0.36f, 0.0f);
	glVertex3f(0.08f, -0.36f, 0.0f);

	//left wing
	//rgb(183, 212, 218)
	glColor3f((183.0f / 260.0f), (212.0f / 260.0f), (218.0f / 260.0f));

	glVertex3f(-0.1f, 0.16f, 0.0f);
	glVertex3f(-0.45f, -0.12f, 0.0f);
	glVertex3f(-0.48f, -0.35f, 0.0f);
	glVertex3f(-0.08f, -0.29f, 0.0f);

	//right wing
	//rgb(183, 212, 218)
	glColor3f((183.0f / 260.0f), (212.0f / 260.0f), (218.0f / 260.0f));

	glVertex3f(0.1f, 0.16f, 0.0f);
	glVertex3f(0.45f, -0.12f, 0.0f);
	glVertex3f(0.48f, -0.35f, 0.0f);
	glVertex3f(0.08f, -0.29f, 0.0f);

	//left wing outer quad
	//rgb(111, 111, 119)
	glColor3f((111.0f / 260.0f), (111.0f / 260.0f), (119.0f / 260.0f));

	glVertex3f(-0.52f, 0.08f, 0.0f);
	glVertex3f(-0.52f, -0.4f, 0.0f);
	glVertex3f(-0.48f, -0.35f, 0.0f);
	glVertex3f(-0.45f, -0.12f, 0.0f);

	//right wing outer quad
	//rgb(111, 111, 119)
	glColor3f((111.0f / 260.0f), (111.0f / 260.0f), (119.0f / 260.0f));

	glVertex3f(0.52f, 0.08f, 0.0f);
	glVertex3f(0.52f, -0.4f, 0.0f);
	glVertex3f(0.48f, -0.35f, 0.0f);
	glVertex3f(0.45f, -0.12f, 0.0f);


	glEnd();

	//upper Triangle

	glBegin(GL_TRIANGLES);

	glVertex3f(-0.06f, 0.46f, 0.0f);
	glVertex3f(0.0f, 0.9f, 0.0f);
	glVertex3f(0.06f, 0.46f, 0.0f);

	glEnd();

	glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glBegin(GL_LINES);  //orange circle                                


	float Radious_Orange = 0.06f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = Radious_Orange * cos(angle) + (0.0f);
		float y = Radious_Orange * sin(angle) + (-0.25f);



		// Circle Origin


		glVertex3f(0.0, -0.25, 0.0f);



		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));
	glBegin(GL_LINES);  //white circle                                


	float Radious_White = 0.04f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = Radious_White * cos(angle) + (0.0f);
		float y = Radious_White * sin(angle) + (-0.25f);



		// Circle Origin


		glVertex3f(0.0, -0.25, 0.0f);



		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));
	glBegin(GL_LINES);  //Green circle                                


	float Radious_Green = 0.02f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = Radious_Green * cos(angle) + (0.0f);
		float y = Radious_Green * sin(angle) + (-0.25f);



		// Circle Origin


		glVertex3f(0.0, -0.25, 0.0f);



		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	//engine fire of plane

	glBegin(GL_QUADS);
	glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glVertex3f(0.13f, -0.62f, 0.0f);
	glVertex3f(-0.13f, -0.62f, 0.0f);
	glVertex3f(-0.13f, -0.64f, 0.0f);
	glVertex3f(0.13f, -0.64f, 0.0f);

	glEnd();

	glBegin(GL_TRIANGLES);

	glVertex3f(-0.0434f, -0.64f, 0.0f);   //right base point
	glVertex3f(-0.13f, -0.64f, 0.0f);     //left base point
	//glVertex3f(-0.08667f, -0.68f, 0.0f);  //apex point
	glVertex3f(-0.05f, -0.70f, 0.0f);  //apex point



	glEnd();
	glBegin(GL_TRIANGLES);

	glVertex3f(0.0432f, -0.64f, 0.0f);   //right base point
	glVertex3f(-0.0434f, -0.64f, 0.0f);     //left base point
	glVertex3f(-0.0001f, -0.9f, 0.0f);  //apex point


	glEnd();

	glBegin(GL_TRIANGLES);

	glVertex3f(0.1298f, -0.64f, 0.0f);   //right base point
	glVertex3f(0.0432f, -0.64f, 0.0f);  //left base point
	//glVertex3f(0.0865, -0.68f, 0.0f);  //apex point
	glVertex3f(0.05f, -0.70f, 0.0f);

	glEnd();
	glPopMatrix();
}
void Plane3(void)
{
	glPushMatrix();
	glScalef(0.16f, 0.18f, 0.0f);
	glTranslatef(-9.0f, -2.5f, 0.0f);
	glTranslatef(Move_Lower_Plane_X, Move_Lower_Plane_Y, 0.0f);
	glRotatef(Rotate_Plane_Three, 0.0f, 0.0f, 1.0f);
	//glRotatef(270, 0.0f, 0.0f, 1.0f);
	glColor3f((183.0f / 260.0f), (212.0f / 260.0f), (218.0f / 260.0f));

	glBegin(GL_QUADS);                     //left tail


	glVertex3f(-0.08f, -0.36f, 0.0f);


	glVertex3f(-0.24f, -0.56f, 0.0f);


	glVertex3f(-0.22f, -0.62f, 0.0f);


	glVertex3f(-0.06f, -0.52f, 0.0f);

	//right tail

	glVertex3f(0.08f, -0.36f, 0.0f);


	glVertex3f(0.24f, -0.56f, 0.0f);


	glVertex3f(0.22f, -0.62f, 0.0f);


	glVertex3f(0.06f, -0.52f, 0.0f);

	//middle quad between tails
	glVertex3f(-0.08f, -0.36f, 0.0f);


	glVertex3f(-0.06f, -0.52f, 0.0f);


	glVertex3f(0.06f, -0.52f, 0.0f);


	glVertex3f(0.08f, -0.36f, 0.0f);

	glEnd();

	//tail middle triangle
	glColor3f((111.0f / 260.0f), (111.0f / 260.0f), (119.0f / 260.0f));

	glBegin(GL_TRIANGLES);

	glVertex3f(0.06f, -0.52f, 0.0f);
	glVertex3f(-0.06f, -0.52f, 0.0f);
	glVertex3f(0.0f, -0.6f, 0.0f);

	glEnd();

	//plane middle part quad
	//rgb(101, 170, 199)
	glColor3f((101.0f / 260.0f), (170.0f / 260.0f), (199.0f / 260.0f));
	glBegin(GL_QUADS);

	glVertex3f(0.12f, 0.46f, 0.0f);
	glVertex3f(-0.12f, 0.46f, 0.0f);
	glVertex3f(-0.08f, -0.36f, 0.0f);
	glVertex3f(0.08f, -0.36f, 0.0f);

	//left wing
	//rgb(183, 212, 218)
	glColor3f((183.0f / 260.0f), (212.0f / 260.0f), (218.0f / 260.0f));

	glVertex3f(-0.1f, 0.16f, 0.0f);
	glVertex3f(-0.45f, -0.12f, 0.0f);
	glVertex3f(-0.48f, -0.35f, 0.0f);
	glVertex3f(-0.08f, -0.29f, 0.0f);

	//right wing
	//rgb(183, 212, 218)
	glColor3f((183.0f / 260.0f), (212.0f / 260.0f), (218.0f / 260.0f));

	glVertex3f(0.1f, 0.16f, 0.0f);
	glVertex3f(0.45f, -0.12f, 0.0f);
	glVertex3f(0.48f, -0.35f, 0.0f);
	glVertex3f(0.08f, -0.29f, 0.0f);

	//left wing outer quad
	//rgb(111, 111, 119)
	glColor3f((111.0f / 260.0f), (111.0f / 260.0f), (119.0f / 260.0f));

	glVertex3f(-0.52f, 0.08f, 0.0f);
	glVertex3f(-0.52f, -0.4f, 0.0f);
	glVertex3f(-0.48f, -0.35f, 0.0f);
	glVertex3f(-0.45f, -0.12f, 0.0f);

	//right wing outer quad
	//rgb(111, 111, 119)
	glColor3f((111.0f / 260.0f), (111.0f / 260.0f), (119.0f / 260.0f));

	glVertex3f(0.52f, 0.08f, 0.0f);
	glVertex3f(0.52f, -0.4f, 0.0f);
	glVertex3f(0.48f, -0.35f, 0.0f);
	glVertex3f(0.45f, -0.12f, 0.0f);


	glEnd();

	//upper Triangle

	glBegin(GL_TRIANGLES);

	glVertex3f(-0.06f, 0.46f, 0.0f);
	glVertex3f(0.0f, 0.9f, 0.0f);
	glVertex3f(0.06f, 0.46f, 0.0f);

	glEnd();

	glColor3f((255.0f / 256.0f), (103.0f / 256.0f), (31.0f / 256.0f));

	glBegin(GL_LINES);  //orange circle                                


	float Radious_Orange = 0.06f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = Radious_Orange * cos(angle) + (0.0f);
		float y = Radious_Orange * sin(angle) + (-0.25f);



		// Circle Origin


		glVertex3f(0.0, -0.25, 0.0f);



		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glColor3f((255.0f / 256.0f), (255.0f / 256.0f), (255.0f / 256.0f));
	glBegin(GL_LINES);  //white circle                                


	float Radious_White = 0.04f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = Radious_White * cos(angle) + (0.0f);
		float y = Radious_White * sin(angle) + (-0.25f);



		// Circle Origin


		glVertex3f(0.0, -0.25, 0.0f);



		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));
	glBegin(GL_LINES);  //Green circle                                


	float Radious_Green = 0.02f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = Radious_Green * cos(angle) + (0.0f);
		float y = Radious_Green * sin(angle) + (-0.25f);



		// Circle Origin


		glVertex3f(0.0, -0.25, 0.0f);



		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	//engine fire of plane

	glBegin(GL_QUADS);
	glColor3f((4.0f / 256.0f), (106.0f / 256.0f), (56.0f / 256.0f));

	glVertex3f(0.13f, -0.62f, 0.0f);
	glVertex3f(-0.13f, -0.62f, 0.0f);
	glVertex3f(-0.13f, -0.64f, 0.0f);
	glVertex3f(0.13f, -0.64f, 0.0f);

	glEnd();

	glBegin(GL_TRIANGLES);

	glVertex3f(-0.0434f, -0.64f, 0.0f);   //right base point
	glVertex3f(-0.13f, -0.64f, 0.0f);     //left base point
	//glVertex3f(-0.08667f, -0.68f, 0.0f);  //apex point
	glVertex3f(-0.05f, -0.70f, 0.0f);  //apex point



	glEnd();
	glBegin(GL_TRIANGLES);

	glVertex3f(0.0432f, -0.64f, 0.0f);   //right base point
	glVertex3f(-0.0434f, -0.64f, 0.0f);     //left base point
	glVertex3f(-0.0001f, -0.9f, 0.0f);  //apex point


	glEnd();

	glBegin(GL_TRIANGLES);

	glVertex3f(0.1298f, -0.64f, 0.0f);   //right base point
	glVertex3f(0.0432f, -0.64f, 0.0f);  //left base point
	//glVertex3f(0.0865, -0.68f, 0.0f);  //apex point
	glVertex3f(0.05f, -0.70f, 0.0f);

	glEnd();


	glPopMatrix();
}


void Parabola(void)
{
	glPushMatrix();
	glTranslatef(-1.0f, 0.2f, 0.0f);
	glBegin(GL_POINTS);

	for (float x = -2.0f; x <= 0.0f; x = x + 0.001f)
	{
		float y = (x * x);
		glVertex2f(x, y);
	}
	glEnd();
	glPopMatrix();
}

void initializeAudio(void)
{
	//function declaration

	void uninitializeAudio(void);

	//code

	// follow 5 setps given below to play audio.

	// first step : initializing ALUT

	alutInit(NULL,NULL);

	error=alutGetError();

	if(error!=ALUT_ERROR_NO_ERROR)
	{
		printf("alutinit() failed !!!\n");
		uninitializeAudio();
		return;
	}

	// second step : Create audio buffer from audio file

	audioBuffer = alutCreateBufferFromFile("Sound2.wav");

	error=alutGetError();

	if(error!=ALUT_ERROR_NO_ERROR)
	{
		printf("alutCreateBufferFromFile() failed !!!\n");
		uninitializeAudio();
		return;
	}

	//step 3 : Create audio source

	alGenSources(1,&audioSource); //Create One ar multiple sources

	error = alGetError();

	if(error!=AL_NO_ERROR)
	{
		printf("Audio source generation failed !!!\n");
		uninitializeAudio();
		return;
	}

	//step 4 : Attach above created audio buffer to above audio source

	alSourcei(audioSource,AL_BUFFER,audioBuffer);

	error = alGetError();

	if(error!=AL_NO_ERROR)
	{
		printf("Attaching audio buffer to audio source failed !!!\n");
		uninitializeAudio();
		return;
	}

	// step 5 : Play audio from source

	alSourcePlay(audioSource);

	error = alGetError();

	if(error!=AL_NO_ERROR)
	{
		printf("audio source failed !!!\n");
		uninitializeAudio();
		return;
	}

	return;

}

void uninitializeAudio(void)
{
	// local variable declaration

	ALint state;

	//code

	//first step : stop playing audio from source

	alGetSourcei(audioSource,AL_SOURCE_STATE,&state);  //receive state of your audio source

	if(state == AL_PLAYING) //if audio is currently playing
	{
		alSourceStop(audioSource);

		error = alGetError();

		if(error!=AL_NO_ERROR)
		{
			printf("alSourceStop() failed !!!\n"); // Audio ajun suru ahe.
		
		}

	}

	//step 2 : Detach the audio buffer from the audio source

	alSourcei(audioSource,AL_BUFFER,0);

	error = alGetError();

	if(error!=AL_NO_ERROR)
	{
		printf(" Detaching the audio buffer operation failed !!!\n");
		
	}

	//step 3 : Delete the audio source

	alDeleteSources(1,&audioSource);

	error = alGetError();

	if(error!=AL_NO_ERROR)
	{
		printf(" Audio source deletion failed !!!\n");
		
	}

	else
	{
		audioSource = 0;
	}

	//step 4 : Delete the audio buffer

	alDeleteBuffers(1,&audioBuffer);

	error = alGetError();

	if(error!=AL_NO_ERROR)
	{
		printf(" Audio buffer deletion failed !!!\n");
		
	}

	else
	{
		audioBuffer = 0;
	}

	//step 5 : Uninitialize ALUT

	alutExit(); 

}















