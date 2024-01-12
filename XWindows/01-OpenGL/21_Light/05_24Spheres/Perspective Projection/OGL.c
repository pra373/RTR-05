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

Bool bLight = False;

GLfloat lightAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
//GLfloat lightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightPosition[] = { 0.0f,0.0f,0.0f,1.0f };



//GLfloat materialAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
//GLfloat materialDiffuse[] = { 0.5f,0.2f,0.7f,1.0f };
//GLfloat materialSpecular[] = { 0.7f, 0.7f, 0.7f, 1.0f };
//GLfloat materialShininess[] = { 128.0f };

GLUquadric *quadric = NULL;

GLfloat angleForXRotation = 0.0f;
GLfloat angleForYRotation = 0.0f;
GLfloat angleForZRotation = 0.0f;

GLuint keyPressed = 0;

GLfloat WindowWidth;
GLfloat WindowHeight;









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
    XStoreName(display, window, "Prathamesh Paropkari - 24 Sphere Perspective Projection");

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

                    case 'X':
	                case 'x':
		                keyPressed = 1;
		                angleForXRotation = 0.0f;  //reset rotation
		            break;

	                case 'Y':
	                case 'y':
		                keyPressed = 2;
		                angleForYRotation = 0.0f; //reset rotation
		            break;

	                case 'Z':
                    case 'z':
                        keyPressed = 3;
                        angleForZRotation = 0.0f; //reset rotation
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

	

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

	glEnable(GL_LIGHT0);
	

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //warmup resize

    resize(WIN_WIDTH,WIN_HEIGHT);
}

void resize(int width ,int height)
{
    //code

   // code
	if(height <= 0)
		height = 1;

	WindowHeight = height;
	WindowWidth = width;


	
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	
	gluPerspective(45.0f, ((GLfloat)width / (GLfloat)height), 0.1f, 100.0f);

}


void draw(void)
{
   //code
	// 3rd Change GL_DEPTH_BUFFER_BIT

	//variable declarations

	GLfloat materialAmbient[4];
	GLfloat materialDiffuse[4];
	GLfloat materialSpecular[4];

	GLfloat Shininess;

	GLfloat horizontalDistance = WindowWidth / 6;
	GLfloat verticalDistance = WindowHeight / 7;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, (verticalDistance*5), (GLsizei)(WindowWidth/4), (GLsizei)(WindowHeight/4));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//transformation

	glTranslatef(0.0f, 0.0f, -1.0f);
	

	//animation
	
	if (keyPressed == 1)
	{
		glRotatef(angleForXRotation, 1.0f, 0.0f, 0.0f);
		lightPosition[2] = angleForXRotation;
	}

	if (keyPressed == 2)
	{
		glRotatef(angleForYRotation, 0.0f, 1.0f, 0.0f);
		lightPosition[0] = angleForYRotation;
	}

	if (keyPressed == 3)
	{
		glRotatef(angleForZRotation, 0.0f, 0.0f, 1.0f);
		lightPosition[1] = angleForZRotation;
	}

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);


	//First Column precious stones
	
	//(emerald)

	materialAmbient[0] = 0.0215;
	materialAmbient[1] = 0.1745;
	materialAmbient[2] = 0.0215;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.07568;
	materialDiffuse[1] = 0.61424;
	materialDiffuse[2] = 0.07568;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.633;
	materialSpecular[1] = 0.727811;
	materialSpecular[2] = 0.633;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.6 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5.0f);

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//Second Sphere 
	//jade

	materialAmbient[0] = 0.135;
	materialAmbient[1] = 0.2225;
	materialAmbient[2] = 0.1575; 
	materialAmbient[3] = 1.0f;   

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.54;
	materialDiffuse[1] = 0.89; 
	materialDiffuse[2] = 0.63;
	materialDiffuse[3] = 1.0f; 

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.316228;
	materialSpecular[1] = 0.316228;
	materialSpecular[2] = 0.316228;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.1 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport(0, (verticalDistance * 4), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0, -5.0f);

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//first column third sphere
	//obsidian


	materialAmbient[0] = 0.05375;
	materialAmbient[1] = 0.05;
	materialAmbient[2] = 0.06625;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.18275;
	materialDiffuse[1] = 0.17;
	materialDiffuse[2] = 0.22525;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.332741;
	materialSpecular[1] = 0.328634;
	materialSpecular[2] = 0.346435;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.3 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport(0, (verticalDistance * 3), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//first colomn fourth sphere
	//pearl

	materialAmbient[0] = 0.25;
	materialAmbient[1] = 0.20725;
	materialAmbient[2] = 0.20725;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 1.0;
	materialDiffuse[1] = 0.829;
	materialDiffuse[2] = 0.829;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.296648;
	materialSpecular[1] = 0.296648;
	materialSpecular[2] = 0.296648;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.088 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport(0, (verticalDistance * 2), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);

	
	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//first column fifth sphere
	//ruby


	materialAmbient[0] = 0.1745;
	materialAmbient[1] = 0.01175;
	materialAmbient[2] = 0.01175;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.61424;
	materialDiffuse[1] = 0.04136;
	materialDiffuse[2] = 0.04136;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.727811;
	materialSpecular[1] = 0.626959;
	materialSpecular[2] = 0.626959;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.6 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport(0, (verticalDistance), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);

	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//first column sixth sphere
	//turquoise


	materialAmbient[0] = 0.1;
	materialAmbient[1] = 0.18725;
	materialAmbient[2] = 0.1745;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.396;
	materialDiffuse[1] = 0.74151;
	materialDiffuse[2] = 0.69102;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.297254;
	materialSpecular[1] = 0.30829;
	materialSpecular[2] = 0.306678;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.1 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport(0, 0, (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);



	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	// ******************************** end of first column ****************************

	//second column first sphere
	//brass

	materialAmbient[0] = 0.329412;
	materialAmbient[1] = 0.223529;
	materialAmbient[2] = 0.027451;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.780392;
	materialDiffuse[1] = 0.568627;
	materialDiffuse[2] = 0.113725;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.992157;
	materialSpecular[1] = 0.941176;
	materialSpecular[2] = 0.807843;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.21794872 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance), (verticalDistance * 5), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);
	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//second column second sphere
	//bronze

	materialAmbient[0] = 0.2125;
	materialAmbient[1] = 0.1275;
	materialAmbient[2] = 0.054;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.714;
	materialDiffuse[1] = 0.568627;
	materialDiffuse[2] = 0.113725;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.992157;
	materialSpecular[1] = 0.941176;
	materialSpecular[2] = 0.807843;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.21794872 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance), (verticalDistance * 4), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);

	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//secong column third sphere
	//chrome


	materialAmbient[0] = 0.25;
	materialAmbient[1] = 0.25;
	materialAmbient[2] = 0.25;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.4;
	materialDiffuse[1] = 0.4;
	materialDiffuse[2] = 0.4;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.774597;
	materialSpecular[1] = 0.774597;
	materialSpecular[2] = 0.774597;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.6 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance), (verticalDistance * 3), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);
	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//second column fourth sphere
	//copper

	materialAmbient[0] = 0.19125;
	materialAmbient[1] = 0.0735;
	materialAmbient[2] = 0.0225;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.7038;
	materialDiffuse[1] = 0.27048;
	materialDiffuse[2] = 0.0828;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.256777;
	materialSpecular[1] = 0.137622;
	materialSpecular[2] = 0.086014;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.1 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance), (verticalDistance * 2), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);
	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//second column fifth sphere
	//gold


	materialAmbient[0] = 0.24725;
	materialAmbient[1] = 0.1995;
	materialAmbient[2] = 0.0745;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.75164;
	materialDiffuse[1] = 0.60648;
	materialDiffuse[2] = 0.22648;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.628281;
	materialSpecular[1] = 0.555802;
	materialSpecular[2] = 0.366065;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.4 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance), (verticalDistance), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//second column sixth sphere
	//silver


	materialAmbient[0] = 0.19225;
	materialAmbient[1] = 0.19225;
	materialAmbient[2] = 0.19225;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.50754;
	materialDiffuse[1] = 0.50754;
	materialDiffuse[2] = 0.50754;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.508273;
	materialSpecular[1] = 0.508273;
	materialSpecular[2] = 0.508273;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.4 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance), 0, (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);
	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere
	// ******************************* Second Column ends here ***************************
	
	//third column first sphere
	//black


	materialAmbient[0] = 0.0f;
	materialAmbient[1] = 0.0f;
	materialAmbient[2] = 0.0f;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.01;
	materialDiffuse[1] = 0.01;
	materialDiffuse[2] = 0.01;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.5;
	materialSpecular[1] = 0.5;
	materialSpecular[2] = 0.5;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.25 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance*2), (verticalDistance*5), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);
	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//third column second sphere
	//cyan


	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.1;
	materialAmbient[2] = 0.06;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.0;
	materialDiffuse[1] = 0.50980392;
	materialDiffuse[2] = 0.50980392;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.50196078;
	materialSpecular[1] = 0.50196078;
	materialSpecular[2] = 0.50196078;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.25 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance * 2), (verticalDistance * 4), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);
	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere
	
	//third column third sphere
	//green

	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.0;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.1;
	materialDiffuse[1] = 0.35;
	materialDiffuse[2] = 0.1;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.45;
	materialSpecular[1] = 0.55;
	materialSpecular[2] = 0.45;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.25 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance * 2), (verticalDistance * 3), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);
	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//third column fourth sphere
	//red

	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.0;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.5;
	materialDiffuse[1] = 0.0;
	materialDiffuse[2] = 0.0;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.7;
	materialSpecular[1] = 0.6;
	materialSpecular[2] = 0.6;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.25 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance * 2), (verticalDistance * 2), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//third column fifth sphere
	// white


	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.0;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.55;
	materialDiffuse[1] = 0.55;
	materialDiffuse[2] = 0.55;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.70;
	materialSpecular[1] = 0.70;
	materialSpecular[2] = 0.70;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.25 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance * 2), (verticalDistance ), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);
	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//third column sixth sphere
	//yello plastic


	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.0;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.5;
	materialDiffuse[1] = 0.5;
	materialDiffuse[2] = 0.0;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.60;
	materialSpecular[1] = 0.60;
	materialSpecular[2] = 0.50;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.25 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();
	
	glViewport((horizontalDistance * 2), (0), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	// ***************************** thied column ends here ****************

	//fourth column first sphere
	//black


	materialAmbient[0] = 0.02;
	materialAmbient[1] = 0.02;
	materialAmbient[2] = 0.02;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.01;
	materialDiffuse[1] = 0.01;
	materialDiffuse[2] = 0.01;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.4;
	materialSpecular[1] = 0.4;
	materialSpecular[2] = 0.4;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.078125 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance * 3), (verticalDistance*5), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//fourth column second sphere
	//cyan


	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.05;
	materialAmbient[2] = 0.05;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.4;
	materialDiffuse[1] = 0.5;
	materialDiffuse[2] = 0.5;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.04;
	materialSpecular[1] = 0.7;
	materialSpecular[2] = 0.7;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.078125 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance * 3), (verticalDistance * 4), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);
	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//fourth column third sphere
	//green


	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.05;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.4;
	materialDiffuse[1] = 0.5;
	materialDiffuse[2] = 0.4;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.04;
	materialSpecular[1] = 0.7;
	materialSpecular[2] = 0.04;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.078125 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();
	glViewport((horizontalDistance * 3), (verticalDistance * 3), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//fourth column fourth sphere
	//red


	materialAmbient[0] = 0.05;
	materialAmbient[1] = 0.0;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.5;
	materialDiffuse[1] = 0.4;
	materialDiffuse[2] = 0.4;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.7;
	materialSpecular[1] = 0.04;
	materialSpecular[2] = 0.04;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.078125 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance * 3), (verticalDistance * 2), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);
	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//fourth column fifth sphere
	//white


	materialAmbient[0] = 0.05;
	materialAmbient[1] = 0.05;
	materialAmbient[2] = 0.05;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.5;
	materialDiffuse[1] = 0.5;
	materialDiffuse[2] = 0.5;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.7;
	materialSpecular[1] = 0.7;
	materialSpecular[2] = 0.7;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.078125 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance * 3), (verticalDistance), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);
	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

	//fourth column sixth sphere
	//yellow rubber



	materialAmbient[0] = 0.05;
	materialAmbient[1] = 0.05;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.5;
	materialDiffuse[1] = 0.5;
	materialDiffuse[2] = 0.4;
	materialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.7;
	materialSpecular[1] = 0.7;
	materialSpecular[2] = 0.04;
	materialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	Shininess = 0.078125 * 128;

	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);


	glLoadIdentity();

	glViewport((horizontalDistance * 3), (0), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

	glTranslatef(0.0f, 0.0f, -5.0f);
	

	gluSphere(quadric, 1.0f, 30, 30);  //draw sphere

    //gluSphere(quadric, 0.2f, 50, 50);  //gluSphere call creates internally creates all needed normals for you
    
	// 4th Change use z-axis coordinate*/
	

    glXSwapBuffers(display,window);
}

void update(void)
{
    

	//code

	if (keyPressed == 1)
	{
		angleForXRotation = angleForXRotation + 0.1f;

		if (angleForXRotation >= 360.0f)
		{
			angleForXRotation = 0.0f;
			
		}
	}

	if (keyPressed == 2)
	{
		angleForYRotation = angleForYRotation + 0.1f;

		if (angleForYRotation >= 360.0f)
		{
			angleForYRotation -= 360.0f;
		}
	}

	if (keyPressed == 3)
	{
		angleForZRotation = angleForZRotation + 0.1f;

		if (angleForZRotation >= 360.0f)
		{
			angleForZRotation -= 360.0f;
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












