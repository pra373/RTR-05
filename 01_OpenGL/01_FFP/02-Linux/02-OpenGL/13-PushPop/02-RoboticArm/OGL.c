// standard header file
#include<stdio.h>
#include<stdlib.h> // for exit
#include<memory.h> // for memset

//OpenGL Header files
#include<GL/glx.h>
#include<GL/gl.h>
#include<GL/glu.h>

// X-11 header files
#include<X11/Xlib.h> //for all X-Window API
#include<X11/Xutil.h>  //for XVisualInfo and related API
#include<X11/XKBlib.h> 

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


int shoulder = 0;
int elbow = 0;
int wrist = 0;

GLUquadric *quadric = NULL;




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
                                  GLX_DEPTH_SIZE,24,   //first change for depth
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
    XStoreName(display, window, "Prathamesh Paropkari - Robotc Arm");

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

	                case's':
		            shoulder = (shoulder - 3) % 360;
		            break;

	                case'E':
		            elbow = (elbow + 3) % 360;
		            break;

	                case'e':
		            elbow = (elbow - 3) % 360;
		            break;

	                case'W':
		            wrist = (wrist + 3) % 360;
		            break;

	                case'w':
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

void initialize(void)
{

    //local function declaration
    void resize(int,int);
    void uninitialize(void);
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

    // 2nd change for depth or Enabling depth
	glShadeModel(GL_SMOOTH); // (Its beautification line Not compulsory) jeva kheva color deu theva shade smooth thev
	glClearDepth(1.0f); // Compulsory 
	glEnable(GL_DEPTH_TEST); // 8 test paike ek(depth) test enable kr
	glDepthFunc(GL_LEQUAL); 
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // optional for beautification

    //warmup resize

    quadric = gluNewQuadric();

    resize(WIN_WIDTH,WIN_HEIGHT);
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
    //code
	// 3rd Change GL_DEPTH_BUFFER_BIT
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//glTranslatef(0.0f, 0.0f, -3.0f);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);   //default mode for shapes in OpenGL
	//do initial translation for better visibility
	
	glTranslatef(0.0f, 0.0f, -7.0f);
	//push this matrix
	glPushMatrix();
	{



		// Do transformations for arm

		glRotatef((GLfloat)shoulder, 0.0f, 0.0f, 1.0f);//because in GLU library z axis is considered in the direction of x axis 
		                                               // therefor now x axis goes in the 3D plane like z axis.
		glTranslatef(1.0f, 0.0f, 0.0f);
		glPushMatrix();
		{


			glScalef(4.0f, 0.5f, 1.0f);

			//draw arm

			glColor3f(0.8f, 0.6f, 0.4f);
			gluSphere(quadric, 0.2f, 10, 10);

		
			
		}
		glPopMatrix(); //pop the matric to come back to that point where arm ended by  translation
		
		//do transformations for forearm in CTM


		glTranslatef(0.8f, 0.0f, 0.0f);
		glRotatef((GLfloat)elbow, 0.0f, 0.0f, 1.0f);
		glTranslatef(0.8f, 0.0f, 0.0f);

		glPushMatrix();
		{

			glScalef(4.0f, 0.5f, 1.0f);

			//draw the forearm

			glColor3f(0.8f, 0.6f, 0.4f);
			gluSphere(quadric, 0.2f, 10, 10);
		}
		glPopMatrix();

		//go transformation for wrist
		glTranslatef(0.75f, 0.0f, 0.0f);
		glRotatef((GLfloat)wrist, 0.0f, 0.0f, 1.0f);
		glTranslatef(0.2f, 0.0f, 0.0f);

		glPushMatrix();
		{
			glScalef(1.2f, 1.0f, 0.0f);

			//draw wrist
			glColor3f(0.8f, 0.6f, 0.4f);
			gluSphere(quadric, 0.12f, 10, 10);

			//transformation for pinky
			glTranslatef(0.12f, 0.15f, 0.0f);
			glScalef(0.8f, 0.15f, 0.0f);
			gluSphere(quadric, 0.12f, 10, 10); //draw pinky finger
		}


		glPopMatrix();
		glPushMatrix();
		//transformation for ring finger

		glTranslatef(0.2f, 0.1f, 0.0f);
		glScalef(0.9f, 0.18f, 0.0f);
		gluSphere(quadric, 0.12f, 10, 10); //draw ring finger
		glPopMatrix();

		//transformation for middle finger 

		glPushMatrix();
		glTranslatef(0.25f, 0.04f, 0.0f);
		glScalef(0.9f, 0.18f, 0.0f);
		gluSphere(quadric, 0.12f, 10, 10); //draw middle finger
		glPopMatrix();

		//transformation for index finger
		glPushMatrix();
		glTranslatef(0.25f, -0.02f, 0.0f);
		glScalef(0.9f, 0.18f, 0.0f);
		gluSphere(quadric, 0.12f, 10, 10); //draw index finger
		glPopMatrix();

		//transformation for thumb
		glTranslatef(0.25f, -0.09f, 0.0f);
		glScalef(0.9f, 0.18f, 0.0f);
		gluSphere(quadric, 0.12f, 10, 10); //draw thumb finger


		
		



	}
	glPopMatrix();
	

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

    if (quadric)
	{
		gluDeleteQuadric(quadric);
		quadric = NULL;
	}


}












