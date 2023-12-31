// standard header file
#include<stdio.h>
#include<stdlib.h> // for exit
#include<memory.h> // for memset

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
XVisualInfo visualInfo;

Bool bFullscreen = False;

// Main
int main(void)
{
    // local function declarations
    void uninitialize(void);
    void toggleFullscreen(void);


    // local variable declarations
     int defaultScreen;
     int defaultDepth;
     //XVisualInfo visualInfo;
     Status status;
     XSetWindowAttributes windowAttributes;
     int styleMask;
     Atom windowManagerDelete;
     XEvent event;
     KeySym keySym;

     char keys[26];

     // variable for centering 
     int screenWidth, screenHeight;



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
    defaultDepth = XDefaultDepth(display, defaultScreen);

    // 4th Step -> Get visual info from above three...
    memset((void*)&visualInfo, 0, sizeof(XVisualInfo));

    status = XMatchVisualInfo(display, defaultScreen, defaultDepth, TrueColor, &visualInfo);
    if(status == 0)
    {
        printf("XMatchVisualInfo Failed\n");

        uninitialize();
        exit(1);
    }

    // Step 5 -> Set Window Attributes/Properties...
    memset((void*)&windowAttributes, 0, sizeof(XSetWindowAttributes));

    windowAttributes.border_pixel = 0; // hya window chya border la color asnar ahe ka.. 0 means mala dyaycha nhi tu default dey..
    windowAttributes.background_pixel = XBlackPixel(display, visualInfo.screen);
    windowAttributes.background_pixmap = 0;
    windowAttributes.colormap = XCreateColormap(display,
                                                XRootWindow(display, visualInfo.screen),
                                                visualInfo.visual,
                                                AllocNone);
    
    // Step 6 -> Assign this colormap to global Colormap
    colormap = windowAttributes.colormap;

    // Set the style of window
    styleMask = CWBorderPixel | CWBackPixel | CWColormap | CWEventMask;

    // Step 8 -> Now Finally Create The Window
    window = XCreateWindow(display,
                           XRootWindow(display, visualInfo.screen), 
                           0, 
                           0,
                           WIN_WIDTH,
                           WIN_HEIGHT,
                           0,
                           visualInfo.depth,
                           InputOutput,
                           visualInfo.visual,
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
    XSelectInput(display,
                 window,
                 ExposureMask | VisibilityChangeMask | StructureNotifyMask | KeyPressMask | ButtonPressMask | PointerMotionMask
                  );

    // Step 10 -> Specify window manager delete atom...
    windowManagerDelete = XInternAtom(display, "WM_DELETE_WINDOW", True);

    // Step 11 -> Add/Set above atom as protocol for window manager
    XSetWMProtocols(display, window, &windowManagerDelete, 1);

    // Step 12 -> Give caption to the window
    XStoreName(display, window, "Prathamesh Paropkari - FullScreen Live Code 24Dec2023");

    // Step 13 -> show/map the window
    XMapWindow(display, window);

    // Center the window...

    screenWidth = XWidthOfScreen(XScreenOfDisplay(display, visualInfo.screen));
    screenHeight = XHeightOfScreen(XScreenOfDisplay(display, visualInfo.screen));
    XMoveWindow(display, window, (screenWidth - WIN_WIDTH)/2, (screenHeight - WIN_HEIGHT)/2);

    // Step 14 -> Event Loop
    while(1)
    {
       XNextEvent(display, &event);
      switch(event.type)
      {
          case KeyPress:
               keySym = XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0);

               switch(keySym)
               {
                   case XK_Escape:
                   uninitialize();
                   exit(0);
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


               case 33:
               uninitialize();
               exit(0);
               break;

               default:
               break;
        }
    }

   printf("End of the program\n");

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
    XSendEvent(display,
                XRootWindow(display, visualInfo.screen),
                False,
                SubstructureNotifyMask,
                &event);


}

void uninitialize(void)
{
    // code
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












