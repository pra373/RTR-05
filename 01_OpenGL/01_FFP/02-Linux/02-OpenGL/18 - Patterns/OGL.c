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
    XStoreName(display, window, "Prathamesh Paropkari - Patterns Assignment");

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
    //local variables

    GLint squareWidth = WIN_WIDTH / 3;
	GLint squareHeight = WIN_HEIGHT / 2;

    GLint fullScreenwidth;
	GLint fullScreenHeight;

    GLfloat numRows = 5;
	GLfloat numColumns = 5;

	GLfloat i = 0;
	GLfloat j = 0;

	GLfloat xCoordinate = -0.5f;
	GLfloat yCoordinate = -1.0f;

    GLint countLines = 0;


    //code
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glTranslatef(0.0f, 0.0f, -3.0f);

	if (bFullscreen == False)
    {
        glViewport(0, squareHeight, (GLsizei)squareWidth, (GLsizei)squareHeight);
    }

    if (bFullscreen == True)
    {
        XWindowAttributes windowAttributes;
        XGetWindowAttributes(display, window, &windowAttributes);
        fullScreenwidth = windowAttributes.width;
        fullScreenHeight = windowAttributes.height;

        squareWidth = fullScreenwidth / 3;
        squareHeight = fullScreenHeight / 2;

        glViewport(0, squareHeight, (GLsizei)squareWidth, (GLsizei)squareHeight);
    }

    glPointSize(4.0f);
	
	glBegin(GL_POINTS);

	

	glColor3f(1.0f, 1.0f, 1.0f);

	
	
	for (i = 0; i < numRows; i++)
	{
		

		for (j = 0; j < numColumns; j++)
		{
			glVertex3f(xCoordinate, yCoordinate, 0.0f);

			xCoordinate += 0.5f;
		}

		yCoordinate += 0.25f;

		xCoordinate = -0.5f;
	}

	

	glEnd();

	glPointSize(1.0f);

    //first row third figure

	if (bFullscreen == False)
	{
		

		glViewport(2 * squareWidth, squareHeight, (GLsizei)squareWidth, (GLsizei)squareHeight);
	}

	if (bFullscreen == True)
	{
		XWindowAttributes windowAttributes;
        XGetWindowAttributes(display, window, &windowAttributes);
        fullScreenwidth = windowAttributes.width;
        fullScreenHeight = windowAttributes.height;

        squareWidth = fullScreenwidth / 3;
        squareHeight = fullScreenHeight / 2;

        
		glViewport(2* squareWidth, squareHeight, (GLsizei)(squareWidth), (GLsizei)(squareHeight));
	}

	

	//re initialize local variables

	numColumns = 4;
	numRows = 4;
	//left bottom

	GLfloat leftBottomX = -1.0f;
	GLfloat leftBottomY = -1.0f;

	//right bottom

	GLfloat rightBottomX = -0.5f;
	GLfloat rightBottomY = -1.0f;

	//right top

	GLfloat rightTopX = -0.5f;
	GLfloat rightTopY = -0.5f;

	//left top

	GLfloat leftTopX = -1.0f;
	GLfloat leftTopY = -0.5f;


	glBegin(GL_LINES);

	glColor3f(1.0f, 1.0f, 1.0f);

	glPushMatrix();
	{
		glTranslatef(0.0f, -0.1f, 0.0f);
	}
	glPopMatrix();




	for (i = 0; i < numRows-1; i++)
	{


		for (j = 0; j < numColumns; j++)
		{
			//bottom line

			glVertex3f(leftBottomX, leftBottomY, 0.0f);
			glVertex3f(rightBottomX, rightBottomY, 0.0f);

			//right line

			glVertex3f(rightBottomX, rightBottomY, 0.0f);
			glVertex3f(rightTopX, rightTopY, 0.0f);

			//top line

			glVertex3f(rightTopX, rightTopY, 0.0f);
			glVertex3f(leftTopX, leftTopY, 0.0f);

			//left line

			glVertex3f(leftTopX, leftTopY, 0.0f);
			glVertex3f(leftBottomX, leftBottomY, 0.0f);

			leftBottomX = leftBottomX + 0.5f;

			rightBottomX = rightBottomX + 0.5f;

			rightTopX = rightTopX + 0.5f;

			leftTopX = leftTopX + 0.5f;
		}

		//reset X

		leftBottomX = -1.0f;
		
		rightBottomX = -0.5f;
		
		rightTopX = -0.5f;
	
		leftTopX = -1.0f;
		

		leftBottomY = leftBottomY + 0.25f;

		rightBottomY = rightBottomY + 0.25f;

		rightTopY = rightTopY + 0.25f;

		leftTopY = leftTopY + 0.25f;
	}

	glEnd();

    //fourth shape 

    if (bFullscreen == False)
	{
		

		glViewport(0.0, 0.0, (GLsizei)squareWidth, (GLsizei)squareHeight);
	}

	if (bFullscreen == True)
	{
		XWindowAttributes windowAttributes;
        XGetWindowAttributes(display, window, &windowAttributes);
        fullScreenwidth = windowAttributes.width;
        fullScreenHeight = windowAttributes.height;

        squareWidth = fullScreenwidth / 3;
        squareHeight = fullScreenHeight / 2;

        
		glViewport(0.0, 0.0, (GLsizei)(squareWidth), (GLsizei)(squareHeight));
	}

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glPushMatrix();
	{
		glTranslatef(0.45f, 0.45f, 0.0f);
	

	
		 leftTopX = -1.0f;
		 leftTopY = -0.75f;

		 leftBottomX = -1.0f;
		 leftBottomY = -1.0f;

		 rightTopX = -0.5f;
		 rightTopY = -0.75f;

		 rightBottomX = -0.5f;
		 rightBottomY = -1.0f;
	
	
		 for (i = 0; i < 4; i++)
		 {
			 for (j = 0; j < 4; j++)
			 {
				 glBegin(GL_QUADS);

				 glVertex3f(rightTopX, rightTopY, 0.0f);       //right top
				 glVertex3f(leftTopX, leftTopY, 0.0f);         //left top
				 glVertex3f(leftBottomX, leftBottomY, 0.0f);   // left bottom
				 glVertex3f(rightBottomX, rightBottomY, 0.0f); //right bottom

				 glEnd();

				 glBegin(GL_LINES);

				 glVertex3f(leftBottomX, leftBottomY, 0.0f);   // left bottom

				 glVertex3f(rightTopX, rightTopY, 0.0f);       //right top

				 glEnd();

				 leftTopX = rightTopX;

				 leftBottomX = rightBottomX;

				 rightBottomX = leftBottomX + 0.5f;

				 rightTopX = leftTopX + 0.5f;

				 glEnd();

			 

			 }

			 //reset X coordinate

			 leftTopX = -1.0f;
			 leftBottomX = -1.0f;
			 rightTopX = -0.5f;
			 rightBottomX = -0.5f;


			 //increase Y

			 leftTopY += 0.25f;

			 leftBottomY += 0.25f;

			 rightBottomY += 0.25f;

			 rightTopY += 0.25f;
		 }
	 
	
	}
	glPopMatrix();

    //second shape

    if (bFullscreen == False)
	{
		

		glViewport(squareWidth, squareHeight, (GLsizei)squareWidth, (GLsizei)squareHeight);
	}

	if (bFullscreen == True)
	{
		XWindowAttributes windowAttributes;
        XGetWindowAttributes(display, window, &windowAttributes);
        fullScreenwidth = windowAttributes.width;
        fullScreenHeight = windowAttributes.height;

        squareWidth = fullScreenwidth / 3;
        squareHeight = fullScreenHeight / 2;

        
		glViewport(squareWidth, squareHeight, (GLsizei)(squareWidth), (GLsizei)(squareHeight));
	}

    leftTopX = -1.0f;
	leftTopY = -0.75f;

	leftBottomX = -1.0f;
	leftBottomY = -1.0f;

	rightTopX = -0.5f;
	rightTopY = -0.75f;

	/*rightBottomX = -0.5f;
	rightBottomY = -1.0f;*/

	glPushMatrix();
	{
		glTranslatef(0.2f, 0.0f, 0.0f);

		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				glBegin(GL_TRIANGLES);

				glVertex3f(rightTopX, rightTopY, 0.0f);       //right top
				glVertex3f(leftTopX, leftTopY, 0.0f);         //left top
				glVertex3f(leftBottomX, leftBottomY, 0.0f);   // left bottom
				//glVertex3f(rightBottomX, rightBottomY, 0.0f); //right bottom

				glEnd();

				leftTopX += 0.5f;

				leftBottomX += 0.5f;

				rightTopX += 0.5f;

			}

			//reset X Coordinate

			leftTopX = -1.0f;


			leftBottomX = -1.0f;


			rightTopX = -0.5f;



			//Increase Y Coordinate;
			leftTopY += 0.25;

			leftBottomY += 0.25f;

			rightTopY += 0.25f;
		}

	}

	glPopMatrix();

    //shape 5

    if (bFullscreen == False)
	{
		

		glViewport(squareWidth, (squareHeight * 0), (GLsizei)squareWidth, (GLsizei)squareHeight);
	}

	if (bFullscreen == True)
	{
		XWindowAttributes windowAttributes;
        XGetWindowAttributes(display, window, &windowAttributes);
        fullScreenwidth = windowAttributes.width;
        fullScreenHeight = windowAttributes.height;

        squareWidth = fullScreenwidth / 3;
        squareHeight = fullScreenHeight / 2;

        
		glViewport(squareWidth, (squareHeight * 0), (GLsizei)(squareWidth), (GLsizei)(squareHeight));
	}

    glBegin(GL_QUADS);


	glVertex3f(1.0f, 0.65f, 0.0f);       //right top
	glVertex3f(-1.0f, 0.65f, 0.0f);         //left top
	glVertex3f(-1.0f, -0.65f, 0.0f);   // left bottom
	glVertex3f(1.0f, -0.65f, 0.0f); //right bottom

	glEnd();

	

	glBegin(GL_LINES);


	glVertex3f(-1.0f, 0.65f, 0.0f);         //left top
	glVertex3f(-0.5f, -0.65f, 0.0f);

	glVertex3f(-1.0f, 0.65f, 0.0f);         //left top
	glVertex3f(-0.0f, -0.65f, 0.0f);

	glVertex3f(-1.0f, 0.65f, 0.0f);         //left top
	glVertex3f(0.5f, -0.65f, 0.0f);

	glVertex3f(-1.0f, 0.65f, 0.0f);         //left top
	glVertex3f(1.0f, -0.65f, 0.0f);

	glVertex3f(-1.0f, 0.65f, 0.0f);         //left top
	glVertex3f(1.0f, -0.325f, 0.0f);

	glVertex3f(-1.0f, 0.65f, 0.0f);         //left top
	glVertex3f(1.0f, 0.0f, 0.0f);

	glVertex3f(-1.0f, 0.65f, 0.0f);         //left top
	glVertex3f(1.0f, 0.325f, 0.0f);



	glEnd();

    //shape 6


    if (bFullscreen == False)
	{
		

		glViewport(squareWidth * 2, (squareHeight * 0), (GLsizei)squareWidth, (GLsizei)squareHeight);
	}

	if (bFullscreen == True)
	{
		XWindowAttributes windowAttributes;
        XGetWindowAttributes(display, window, &windowAttributes);
        fullScreenwidth = windowAttributes.width;
        fullScreenHeight = windowAttributes.height;

        squareWidth = fullScreenwidth / 3;
        squareHeight = fullScreenHeight / 2;

        
		glViewport(squareWidth * 2, (squareHeight * 0), (GLsizei)(squareWidth), (GLsizei)(squareHeight));
	}

    leftTopX = -1.0f;
	leftTopY = -0.216;

	leftBottomX = -1.0f;
	leftBottomY = -0.65f;

	rightTopX = -0.333;
	rightTopY = -0.216;

	rightBottomX = -0.333;
	rightBottomY = -0.65f;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			if (j == 0)
			{
				glColor3f(1.0f, 0.0f, 0.0f);
			}

			else if (j == 1)
			{
				glColor3f(0.0f, 1.0f, 0.0f);
			}

			else
			{
				glColor3f(0.0f, 0.0f, 1.0f);

			}
			glBegin(GL_QUADS);


			glVertex3f(rightTopX, rightTopY, 0.0f);       //right top
			glVertex3f(leftTopX, leftTopY, 0.0f);         //left top
			glVertex3f(leftBottomX, leftBottomY, 0.0f);   // left bottom
			glVertex3f(rightBottomX, rightBottomY, 0.0f); //right bottom

			glEnd();

			glColor3f(1.0f, 1.0f, 1.0f);

			glBegin(GL_LINES);

			glVertex3f(rightTopX, rightTopY, 0.0f);       //right top
			glVertex3f(leftTopX, leftTopY, 0.0f);         //left top

			glVertex3f(leftTopX, leftTopY, 0.0f);         //left top
			glVertex3f(leftBottomX, leftBottomY, 0.0f);   // left bottom

			glVertex3f(leftBottomX, leftBottomY, 0.0f);   // left bottom
			glVertex3f(rightBottomX, rightBottomY, 0.0f); //right bottom

			glVertex3f(rightBottomX, rightBottomY, 0.0f); //right bottom
			glVertex3f(rightTopX, rightTopY, 0.0f);       //right top

			glEnd();


			// increment X

			leftTopX += 0.666f;


			leftBottomX += 0.666f;


			rightTopX += 0.666f;


			rightBottomX += 0.666f;



		}

		//reset X

		leftTopX = -1.0f;
		

		leftBottomX = -1.0f;
		

		rightTopX = -0.333;
		

		rightBottomX = -0.333;
		

		//increment Y

		leftTopY += 0.433;

		
		leftBottomY += 0.433;

		
		rightTopY += 0.433;

		
		rightBottomY += 0.433;
	}



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


}












