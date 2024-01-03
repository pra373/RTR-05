// standard header file
#include<stdio.h>
#include<stdlib.h> // for exit
#include<memory.h> // for memset

//OpenGL Header files
#include<GL/glx.h>
#include<GL/gl.h>
#include<GL/glu.h>

// X-11 header files
#include<X11/Xlib.h> // for all X-Window API
#include<X11/Xutil.h>  // for XVisualInfo and related API
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

GLUquadric* quadric = NULL;

// For SpotLight
GLfloat gfLightAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat gfLightDiffuse[] = { 1.0f, 1.0f, 1.0f, 0.0f }; 
GLfloat gfLightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

GLfloat gfLightPosition[] = { 0.0f, 10.0f, -6.0f, 1.0f }; 
GLfloat gfLightSpotLightDirection[] = { 0.0f, -1.0f, 0.0f, 1.0f };

GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat materialDiffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialShininess[] = { 120.0f };

const GLfloat angleCutOff = 10.0f;
const GLfloat angleExponent = 2.0f;

Bool bLight = False;




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
    XStoreName(display, window, "Prathamesh Paropkari - Spot Light On Sphere");

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
		            if (bLight == False)
		            {
			            glEnable(GL_LIGHTING);
			            bLight = True;
		            }

		            else
		            {
			            glDisable(GL_LIGHTING);
			            bLight = False;
		            }

		            break;


                    case 'W':
	                case 'w':
		            if (gfLightPosition[2] >= -15.0f)
			                gfLightPosition[2] -= 0.2f;
		            break;
	                case 'S':
	                case 's':
		            if (gfLightPosition[2] <= -5.0f)
			                gfLightPosition[2] += 0.2f;
		                break;
	                case 'A':
	                case 'a':
		            if (gfLightPosition[0] >= -4.0f);
		                    gfLightPosition[0] -= 0.2f;
		                break;
	                case'D':
	                case 'd':
		                if (gfLightPosition[0] <= 4.0f)
			                gfLightPosition[0] = gfLightPosition[0] + 0.2f;
		                break;
	                case 'R':
	                case 'r':
		                gfLightPosition[0] = 0.0f;
		                gfLightPosition[2] = -6.0f;
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


    quadric = gluNewQuadric();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//enabling light0

	// Light related initialization
	glLightfv(GL_LIGHT1, GL_AMBIENT, gfLightAmbient); //f= sangto float property ahe v= sangto vector prperty ahe means array ahe
	glLightfv(GL_LIGHT1, GL_DIFFUSE, gfLightDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, gfLightSpecular);
	glLightfv(GL_LIGHT1, GL_POSITION, gfLightPosition);
	
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, gfLightSpotLightDirection); //f= sangto float property ahe v= sangto vector prperty ahe means array ahe
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, angleCutOff);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, angleExponent);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5f);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0f);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0f);

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);

	glEnable(GL_LIGHT1);


	// Initialize quadric
	


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
    // function prototype
	void drawSphere(void);

	//code
	glLightfv(GL_LIGHT1, GL_POSITION, gfLightPosition);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt(0.0f, 1.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	glTranslatef(0.0f, -3.0f, -4.0f);
    
	drawSphere();

    glXSwapBuffers(display,window);
}

void update(void)
{
    
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

void drawSphere(void)
{


	// code
	glPushMatrix();
	{
		// first column
		materialDiffuse[0] = 1.0f; // r
		materialDiffuse[1] = 0.0f; // g
		materialDiffuse[2] = 0.0f; // b
		materialDiffuse[3] = 1.0f; // a
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		materialSpecular[0] = 1.0f;
		materialSpecular[1] = 0.0f;
		materialSpecular[2] = 0.0f;
		materialSpecular[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		glTranslatef(-4.0f, 0.0f, 0.0f);

		for (int i = 0; i < 10; i++)
		{
			glTranslatef(0.0f, 0.0f, -2.0f);
			gluSphere(quadric, 0.7f, 100, 100);
		}
	}
	glPopMatrix();

	glPushMatrix();
	{
		// 2nd column
		materialDiffuse[0] = 1.0f; // r
		materialDiffuse[1] = 0.0f; // g
		materialDiffuse[2] = 0.0f; // b
		materialDiffuse[3] = 1.0f; // a
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		materialSpecular[0] = 1.0f; // r
		materialSpecular[1] = 0.5f; // g
		materialSpecular[2] = 0.0f; // b
		materialSpecular[3] = 1.0f;  //a
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		glTranslatef(-3.0f, 0.0f, 0.0f);

		for (int i = 0; i < 10; i++)
		{
			glTranslatef(0.0f, 0.0f, -2.0f);
			gluSphere(quadric, 0.7f, 100, 100);
		}
	}
	glPopMatrix();

	// 3rd column
	glPushMatrix();
	{
		// 3rd column
		materialDiffuse[0] = 1.0f; // r
		materialDiffuse[1] = 1.0f; // g
		materialDiffuse[2] = 0.0f; // b
		materialDiffuse[3] = 1.0f; // a
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		materialSpecular[0] = 1.0f; // r
		materialSpecular[1] = 1.0f; // g
		materialSpecular[2] = 0.0f; // b
		materialSpecular[3] = 1.0f;  //a
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		glTranslatef(-1.5f, 0.0f, 0.0f);

		for (int i = 0; i < 10; i++)
		{
			glTranslatef(0.0f, 0.0f, -2.0f);
			gluSphere(quadric, 0.7f, 100, 100);
		}
	}
	glPopMatrix();


	glPushMatrix();
	{
		// 4th column
		materialDiffuse[0] = 0.0f; // r
		materialDiffuse[1] = 1.0f; // g
		materialDiffuse[2] = 0.0f; // b
		materialDiffuse[3] = 1.0f; // a
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		materialSpecular[0] = 0.0f; // r
		materialSpecular[1] = 1.0f; // g
		materialSpecular[2] = 0.0f; // b
		materialSpecular[3] = 1.0f;  //a
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		glTranslatef(0.0f, 0.0f, 0.0f);

		for (int i = 0; i < 10; i++)
		{
			glTranslatef(0.0f, 0.0f, -2.0f);
			gluSphere(quadric, 0.7f, 100, 100);
		}
	}
	glPopMatrix();

	glPushMatrix();
	{
		// 5th column
		materialDiffuse[0] = 0.0f; // r
		materialDiffuse[1] = 0.0f; // g
		materialDiffuse[2] = 1.0f; // b
		materialDiffuse[3] = 1.0f; // a
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		materialSpecular[0] = 0.0f; // r
		materialSpecular[1] = 0.0f; // g
		materialSpecular[2] = 1.0f; // b
		materialSpecular[3] = 1.0f;  //a
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		glTranslatef(1.5f, 0.0f, 0.0f);

		for (int i = 0; i < 10; i++)
		{
			glTranslatef(0.0f, 0.0f, -2.0f);
			gluSphere(quadric, 0.7f, 100, 100);
		}
	}
	glPopMatrix();

	glPushMatrix();
	{
		// 6th column
		materialDiffuse[0] = 0.294f; // r
		materialDiffuse[1] = 0.0f;   // g
		materialDiffuse[2] = 0.509f; // b
		materialDiffuse[3] = 1.0f;   // a
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		materialSpecular[0] = 0.294f; // r
		materialSpecular[1] = 0.0f;   // g
		materialSpecular[2] = 0.509f; // b
		materialSpecular[3] = 1.0f;  //a
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		glTranslatef(3.0f, 0.0f, 0.0f);

		for (int i = 0; i < 10; i++)
		{
			glTranslatef(0.0f, 0.0f, -2.0f);
			gluSphere(quadric, 0.7f, 100, 100);
		}
	}
	glPopMatrix();

	glPushMatrix();
	{
		// 7th column
		materialDiffuse[0] = 0.937f; // r
		materialDiffuse[1] = 0.509f;   // g
		materialDiffuse[2] = 0.937f; // b
		materialDiffuse[3] = 1.0f;   // a
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		materialSpecular[0] = 0.937f; // r
		materialSpecular[1] = 0.509f;   // g
		materialSpecular[2] = 0.937f; // b
		materialSpecular[3] = 1.0f;  //a
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

		glTranslatef(4.5f, 0.0f, 0.0f);

		for (int i = 0; i < 10; i++)
		{
			glTranslatef(0.0f, 0.0f, -2.0f);
			gluSphere(quadric, 0.7f, 100, 100);
		}
	}
	glPopMatrix();

}












