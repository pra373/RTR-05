//
//  Window.m
//  
//
//  Created by user258815 on 6/16/24.
//

#import <Foundation/Foundation.h>

#import <Cocoa/Cocoa.h>

#import <QuartzCore/CVDisplayLink.h>

#import <OpenGL/gl3.h>

#import <OpenGL/gl3ext.h>

#import"vmath.h"

using namespace vmath;

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
//global function declaration

CVReturn MyDisplayLinkCallback(CVDisplayLinkRef,const CVTimeStamp *,const CVTimeStamp *,CVOptionFlags,CVOptionFlags *,void *);

//global variable declarations

FILE *gpFile = NULL;

enum
{
    AMC_ATTRIBUTE_POSITION = 0,
    AMC_ATTRIBUTE_TEXTURE = 1
};

//OpenGL related global variables



@interface AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>

@end

@interface GLview : NSOpenGLView
@end

//main function

int main(int argc, char* argv[])
{
    //code
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    NSApp = [NSApplication sharedApplication];
    
    [NSApp setDelegate : [[AppDelegate alloc] init]];
    
    [NSApp run];
    
    [pool release];
    
    return 0;
}

//implementation of custom AppDelegate interface

@implementation AppDelegate

    {
        
        
        NSWindow *window;
        
        GLview *glView;
        
    }

    
    -(void)applicationDidFinishLaunching:(NSNotification *)notification
    {
        //code
        
        //log file creation code
        
        NSBundle *appBundle = [NSBundle mainBundle];
        
        NSString *appDirPath = [appBundle bundlePath];
        
        NSString *parentDirPath = [appDirPath stringByDeletingLastPathComponent];
        
        NSString *logFileNameWithPath = [NSString stringWithFormat:@"%@/Log.txt",parentDirPath];
        
        const char *pszLogFileNameWithPath = [logFileNameWithPath cStringUsingEncoding:NSASCIIStringEncoding];
        
        gpFile = fopen(pszLogFileNameWithPath,"w");
        
        if(gpFile == NULL)
        {
            NSLog(@"Log File Cannot Be Created");
            
            [self release];
            
            [NSApp terminate:self];
        }
        
        fprintf(gpFile,"Program Started Successfully\n");
        
        // Painting window background black
        [window setBackgroundColor : [NSColor blackColor]];
        
        //declare rectangle for frame of our window
        
        NSRect win_rect = NSMakeRect(0.0,0.0,WIN_WIDTH,WIN_HEIGHT);
        
        //create the window
        
        window = [[NSWindow alloc]initWithContentRect:win_rect
                                             styleMask: NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable
                                              backing:NSBackingStoreBuffered
                                                defer:NO];
        
        
        //give title to the window
        
        [window setTitle:@"Prathamesh Paropkari: Smiley Texture"];
        
        //centre the window
        
        [window center];
        
        //create the custom view
        
        glView = [[GLview alloc]initWithFrame:win_rect];
        
        //set this newly created custom as view of our custom Window
        
        [window setContentView: glView];
        
        [window setDelegate:self];
        
        //now actually show the window, give it keyboard focus and make it top in the z order
        
        [window makeKeyAndOrderFront:self];
    }
    
    -(void)applicationWillTerminate:(NSNotification *)notification
    {
        //log file closing code
        
        if(gpFile)
        {
            fprintf(gpFile,"Program Finished Successfully");
            
            fclose(gpFile);
            
            gpFile = NULL;
        }
    }
    
    -(void)windowWillClose:(NSNotification *)notification
    {
        //code
        
        [NSApp terminate:self];
    }
    
    -(void)dealloc
    {
        [super dealloc];
        
        [glView release];
        
        [window release];
        
        
        
    }
    
    

    

@end

//implementation of custome view interface

@implementation GLview



    CVDisplayLinkRef displayLink;

  //OpenGL Related Field variables

    GLuint shaderProgramObject;

    //pyramid
    GLuint vao_Pyramid;
    GLuint vbo_Position_Pyramid;
    GLuint vbo_TexCoord_Pyramid;

    GLfloat AnglePyramid = 0.0f;

    //cube

    GLuint vao_Cube;
    GLuint vbo_Position_Cube;
    GLuint vbo_TexCoord_Cube;

    GLfloat AngleCube = 0.0f;

    //uniforms

    GLuint textureSamplerUniform = 0;

    GLuint mvpMatrixUniform;

    

    

    //texture variable

    GLuint stone_Texture;

    GLuint kundali_Texture;

    mat4 perspectiveProjectionMatrix;

    -(id)initWithFrame:(NSRect)frame
    {
        //code
        
        self = [super initWithFrame:frame];
        
        if(self)
        {
            //steps for code in view constructor
            
            
            
            //step 1: Declare OpenGL Attributes
            
            NSOpenGLPixelFormatAttribute attributes[]={
                
                NSOpenGLPFAOpenGLProfile,NSOpenGLProfileVersion4_1Core,
                NSOpenGLPFAScreenMask,CGDisplayIDToOpenGLDisplayMask(kCGDirectMainDisplay),
                NSOpenGLPFAColorSize,24,
                NSOpenGLPFADepthSize,32,
                NSOpenGLPFAAlphaSize,8,
                NSOpenGLPFANoRecovery,
                NSOpenGLPFAAccelerated,
                NSOpenGLPFADoubleBuffer,
                0
                
            };
            
            //step2 : Create OpenGL Pixel Format using above attributes
            
            NSOpenGLPixelFormat *pixelFormat=[[[NSOpenGLPixelFormat alloc]initWithAttributes:attributes]autorelease];
            
            if(pixelFormat == nil)
            {
                fprintf(gpFile,"OpenGL Pixel Format Failed");
                [self uninitialize];
                [self release];
                [NSApp terminate:self];
            }
            //step3: Create OpenGL Context using above Pixel Format
            
            NSOpenGLContext *glContext = [[[NSOpenGLContext alloc]initWithFormat:pixelFormat shareContext:nil]autorelease];
            
            if(glContext == nil)
            {
                fprintf(gpFile,"glContext Creation Failed");
                [self uninitialize];
                [self release];
                [NSApp terminate:self];
            }
            //step4: Set Pixel Format
            
            [self setPixelFormat:pixelFormat];
            
            //step 5:Set OpenGL Context
            
            [self setOpenGLContext:glContext];
        }
        
        
        
        return(self);
    }

    -(void)prepareOpenGL
    {
        [super prepareOpenGL];
        
        [[self openGLContext]makeCurrentContext];
        
        //matching monitor retracing with double buffer swaping
        
        GLint swapInterval=1;
        
        [[self openGLContext]setValues:&swapInterval forParameter:NSOpenGLCPSwapInterval];
        
        int result = [self initialize];
        
        //Create and start display link
        
        //steps of creating display link
        
        //step 1: Create Display Link According to the current active display
        
        CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
        
        //step 2: set the call back method for OpenGL Thread
        
        CVDisplayLinkSetOutputCallback(displayLink,&MyDisplayLinkCallback,self);
        
        //step 3:Convert NSOpenGLPixelFormat to CGL pixel format.
        
        CGLPixelFormatObj cglPixelFormat = (CGLPixelFormatObj)[[self pixelFormat]CGLPixelFormatObj];
        
        //step 4:convert NSOpenGL Context into CGL Context
        
        CGLContextObj cglContext = (CGLContextObj)[[self openGLContext]CGLContextObj];
        
        //step 5 : set above two for display link
        
        CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink,cglContext,cglPixelFormat);
        
        //step 6 : start display link (start OpenGL Thread)
        
        CVDisplayLinkStart(displayLink);
        
        
    }

    -(void)reshape
    {
        [super reshape];
        
        [[self openGLContext]makeCurrentContext];
        
        CGLLockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
        
        NSRect rect = [self bounds];
        
        int width = rect.size.width;
        
        int height = rect.size.height;
        
        [self resize:width :height];
        
        CGLUnlockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
    }

    
    


    
    -(void)drawRect:(NSRect)dirtyRect
    {
        // code
        
        //call rendering function here too, to avoid flikkering
        
    }

    -(void)drawView  //our rendering function
    {
        [[self openGLContext]makeCurrentContext];
        
        CGLLockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
        
        [self display];
        
        CGLFlushDrawable((CGLContextObj)[[self openGLContext]CGLContextObj]);
        
        CGLUnlockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
        
        
    }

    //below function gets called per frame using auto release pool

-(CVReturn)getFrameForTime:(const CVTimeStamp *) outputTime
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    //call rendering function here too
    
    [self drawView];
    
    [self myUpdate];
    
    [pool release];
    
    return(kCVReturnSuccess);
    
}

    -(int)initialize
    {
        //code
        
        [self printGLInfo];   // copy paste point
        
        //vertex shader

        const GLchar* vertexShaderSourceCode =
            "#version 410 core\n"             // (version of GLSL * 100) core means you are using core profile i.e PP
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

            [self uninitialize];
            [self release];
            [NSApp terminate:self];
        }

        //follow all steps of vertex shader for fragment shader

        //step 6  : Write the source code for fragment shader

        //fragment shader

        const GLchar* fragmentShaderSourceCode =
            "#version 410 core\n"
            "\n"
            "out vec4 FragColor;\n"
            "in vec2 oTexCoord;\n"
            "uniform sampler2D uTextureSampler;\n"
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

             [self uninitialize];
             [self release];
             [NSApp terminate:self];
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
        
        glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_TEXTURE, "aTexCoord");

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

             [self uninitialize];
             [self release];
             [NSApp terminate:self];
         }

         //step 12: Declare position and color arrays

         //get Shader uniform Locations

         mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "uMVPMatrix");  //must be done after linkage of shader program
        
        textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "uTextureSampler");
        
        if (textureSamplerUniform == -1)
        {
            fprintf(gpFile, "Loading of uniform variable failed !! Exiting now\n");
            return(-7);
        }
        
        
        
        
        
        GLfloat pyramidVertices[] =
        {
            // front
            0.0f,  1.0f,  0.0f, // front-top
           -1.0f, -1.0f,  1.0f, // front-left
            1.0f, -1.0f,  1.0f, // front-right

            // right
            0.0f,  1.0f,  0.0f, // right-top
            1.0f, -1.0f,  1.0f, // right-left
            1.0f, -1.0f, -1.0f, // right-right

            // back
            0.0f,  1.0f,  0.0f, // back-top
            1.0f, -1.0f, -1.0f, // back-left
           -1.0f, -1.0f, -1.0f, // back-right

           // left
           0.0f,  1.0f,  0.0f, // left-top
          -1.0f, -1.0f, -1.0f, // left-left
          -1.0f, -1.0f,  1.0f, // left-right
        };
        
        GLfloat pyramidTexcoords[] =
        {
            // front
            0.5, 1.0, // front-top
            0.0, 0.0, // front-left
            1.0, 0.0, // front-right

            // right
            0.5, 1.0, // right-top
            1.0, 0.0, // right-left
            0.0, 0.0, // right-right

            // back
            0.5, 1.0, // back-top
            0.0, 0.0, // back-left
            1.0, 0.0, // back-right

            // left
            0.5, 1.0, // left-top
            1.0, 0.0, // left-left
            0.0, 0.0, // left-right
        };
        
        //cube vertices
        
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
        
        //cube texcoord
        
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

           
         ////step 13: Create vertex array object
         //VAO

         glGenVertexArrays(1, &vao_Pyramid);

         //step 14: Bind with VAO

         glBindVertexArray(vao_Pyramid);

         //step 15: Create vertex buffer object (VBO) for position
         
         //VBO for position

         glGenBuffers(1, &vbo_Position_Pyramid);

         //step 16: Bind with VBO of position

         glBindBuffer(GL_ARRAY_BUFFER, vbo_Position_Pyramid);

         glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);

         glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

         glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

         glBindBuffer(GL_ARRAY_BUFFER, 0);

        //VBO for TEXCOORD

        glGenBuffers(1, &vbo_TexCoord_Pyramid);

        //step 16: Bind with VBO of TEXCOORD

        glBindBuffer(GL_ARRAY_BUFFER, vbo_TexCoord_Pyramid);

        glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidTexcoords), pyramidTexcoords, GL_STATIC_DRAW);

        glVertexAttribPointer(AMC_ATTRIBUTE_TEXTURE, 2, GL_FLOAT, GL_FALSE, 0, NULL);

        glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXTURE);

        glBindBuffer(GL_ARRAY_BUFFER, 0);


         //step 18 : unbind with VAO
         glBindVertexArray(0);


        // ********************* VAO for cube **********************
        
        ////step 13: Create vertex array object
        //VAO

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

       glGenBuffers(1, &vbo_TexCoord_Cube);

       //step 16: Bind with VBO of TEXCOORD

       glBindBuffer(GL_ARRAY_BUFFER, vbo_TexCoord_Cube);

       glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTexcoords), cubeTexcoords, GL_STATIC_DRAW);

       glVertexAttribPointer(AMC_ATTRIBUTE_TEXTURE, 2, GL_FLOAT, GL_FALSE, 0, NULL);

       glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXTURE);

       glBindBuffer(GL_ARRAY_BUFFER, 0);


        //step 18 : unbind with VAO
        glBindVertexArray(0);



        // depth setting
        
        glClearDepth(1.0f); // Compulsory
        glEnable(GL_DEPTH_TEST); // 8 test paike ek(depth) test enable kr
        glDepthFunc(GL_LEQUAL);
        
        //clear color
        
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        
        perspectiveProjectionMatrix = vmath::mat4::identity();
        
        // Load texture for stone
        
        stone_Texture = [self loadGLTexture:"Stone.bmp"];
        
        if (stone_Texture == 0)
        {
            fprintf(gpFile, "Failed to load stone texture\n");
            [self uninitialize];
            [self release];
            [NSApp terminate:self];
        }
        
        // Load texture for kundali
        
        kundali_Texture = [self loadGLTexture:"Kundali.bmp"];
        
        if (kundali_Texture == 0)
        {
            fprintf(gpFile, "Failed to load Kundali texture\n");
            [self uninitialize];
            [self release];
            [NSApp terminate:self];
        }
        
        [self resize:WIN_WIDTH:WIN_HEIGHT];
        
        return 0;
    }



    -(void)printGLInfo
    {
        //code
        
        fprintf(gpFile, "OpenGL Vendor : %s\n\n", glGetString(GL_VENDOR));

        fprintf(gpFile, "OpenGL Renderer : %s\n\n", glGetString(GL_RENDERER));

        fprintf(gpFile, "OpenGL Version : %s\n\n", glGetString(GL_VERSION));

        fprintf(gpFile, "Graphic Library Shading Language  (GLSL) Version : %s\n\n",glGetString(GL_SHADING_LANGUAGE_VERSION));

    }

-(GLuint)loadGLTexture:(const char *)szImageFileName
{
    //code
    
    //local variable
    
    GLuint texture;

    //step 1

     NSBundle *appBundle = [NSBundle mainBundle];
        
     NSString *appDirPath = [appBundle bundlePath];
        
     NSString *parentDirPath = [appDirPath stringByDeletingLastPathComponent];
        
     NSString *imageFileNameWithPath = [NSString stringWithFormat:@"%@/%s",parentDirPath,szImageFileName];

     //step 2

     NSImage *image = [[NSImage alloc]initWithContentsOfFile:imageFileNameWithPath];

     //do error checking here

    if (image == nil) 
    {
           fprintf(gpFile, "Image loading failed: %s\n", imageFileNameWithPath.UTF8String);
           [self uninitialize];
           [self release];
           [NSApp terminate:self];
           return 0;
       }     //step 3

     CGImageRef cgImage = [image CGImageForProposedRect:nil context:nil hints:nil];

     //step 4

     int imageWidth = (int)CGImageGetWidth(cgImage);

     int imageHeight =(int)CGImageGetHeight(cgImage);

     //step 5

     CGDataProviderRef imageDataProviderRef = CGImageGetDataProvider(cgImage);

     //step 6

     CFDataRef imageDataRef = CGDataProviderCopyData(imageDataProviderRef);

     //step 7

    const UInt8* imageData = CFDataGetBytePtr(imageDataRef);

     // step 8

     //create OpenGL Texture above image width,heigh and image data
    
    glGenTextures(1, &texture); // generate textures

    
    glBindTexture(GL_TEXTURE_2D, texture);

    

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    glGenerateMipmap(GL_TEXTURE_2D);

    
    glBindTexture(GL_TEXTURE_2D, 0);


    //step 9

    CFRelease(imageDataRef);

    return (texture);

        
}



 -(void)resize:(int)width :(int)height
    {
        // code
        if(height <= 0)
            height = 1;


        //glMatrixMode(GL_PROJECTION); //not allowed in PP
        //glLoadIdentity();            not allowed in PP

        glViewport(0, 0, (GLsizei)width, (GLsizei)height);

        //set Perspective projection matrix

        perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
        }

    -(void)display
    {
        //code
        // 3rd Change GL_DEPTH_BUFFER_BIT
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //PP steps for display function

        //step 1: Use shader program object

        glUseProgram(shaderProgramObject);

        //Pyramid




        //transformations

        mat4 modelViewMatrix = mat4::identity();

        mat4 translationMatrix = mat4::identity();

        translationMatrix = vmath::translate(-1.5f, 0.0f, -6.0f);

        mat4 rotationMatrix = mat4::identity();

        rotationMatrix = vmath::rotate(AnglePyramid, 0.0f, 1.0f, 0.0f);

        //texture lines for Pyramid

        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, stone_Texture);

        glUniform1i(textureSamplerUniform, 0);



        modelViewMatrix = translationMatrix * rotationMatrix;

        //mat4 modelViewMatrix = vmath::translate(0.0f, 0.0f, -3.0f);

        mat4 madelViewProjectionMatrix = mat4::identity();

        madelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;  //order of multiplication is not commutativ. i.e mat1 * mat2 != mat2 * mat1

        //push above mvp into vertex shader's mvp uniform

        glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, madelViewProjectionMatrix);

        //step 2: Bind with (VAO)

        glBindVertexArray(vao_Pyramid);

        //step 3: Draw the geometry

        glDrawArrays(GL_TRIANGLES, 0, 12);


        //step 4: unBind with (VAO)

        glBindVertexArray(0);

        //Cube




        //transformations

         modelViewMatrix = mat4::identity();

         translationMatrix = mat4::identity();

         translationMatrix = vmath::translate(1.5f, 0.0f, -6.0f);

         mat4 scaleMatrix = mat4::identity();

         scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

         mat4 rotationMatrix1 = mat4::identity();

         rotationMatrix1 = mat4::identity();

         rotationMatrix1 = vmath::rotate(AngleCube, 1.0f, 0.0f, 0.0f);

         mat4 rotationMatrix2 = mat4::identity();

         rotationMatrix2 = mat4::identity();

         rotationMatrix2 = vmath::rotate(AngleCube, 0.0f, 1.0f, 0.0f);

         mat4 rotationMatrix3 = mat4::identity();

         rotationMatrix3 = mat4::identity();

         rotationMatrix3 = vmath::rotate(AngleCube, 0.0f, 0.0f, 1.0f);

         rotationMatrix = rotationMatrix1 * rotationMatrix2 * rotationMatrix3;

         modelViewMatrix = translationMatrix * scaleMatrix * rotationMatrix;

         

         //modelViewMatrix = mat4::identity();

        //mat4 modelViewMatrix = vmath::translate(0.0f, 0.0f, -3.0f);

        madelViewProjectionMatrix = mat4::identity();

        madelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;  //order of multiplication is not commutativ. i.e mat1 * mat2 != mat2 * mat1

        //push above mvp into vertex shader's mvp uniform

        glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, madelViewProjectionMatrix);

        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, kundali_Texture);

        glUniform1i(textureSamplerUniform, 0);

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
    }

    -(void)myUpdate
    {
        //code
        AnglePyramid = AnglePyramid - 0.2f;

        if (AnglePyramid <= 0.0f)
        {
            AnglePyramid = 360.0f;
        }
        
        AngleCube = AngleCube + 0.2f;

        if (AngleCube >= 360.0f)
        {
            AngleCube = 0.0f;
        }    }

    -(void)uninitialize
    {
        //code
        
        //steps for uninitialize function in PP

        if (shaderProgramObject)
        {
            //step 1 : use shader program object

            glUseProgram(shaderProgramObject);

            GLint numShaders = 0;

            //step 2 : get number of attached shaders
            glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &numShaders);



            if (numShaders > 0)
            {
                //step 3 : Create and allocate memory of shader objects according to number of attached shaders
                GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint));

                if (pShaders != NULL)
                {
                    //step 4 : Get shader object into the allocated array.

                    glGetAttachedShaders(shaderProgramObject, numShaders, NULL, pShaders);

                    //step 5 : Detach and delete shader objects iteratively

                    for (GLint i = 0; i < numShaders; i++)
                    {
                        glDetachShader(shaderProgramObject, pShaders[i]);
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

            glDeleteProgram(shaderProgramObject);
            shaderProgramObject = 0;

            
            



        }
        
        //step 10 : delete vbo for texture

        if (vbo_TexCoord_Pyramid)
        {
            glDeleteBuffers(1, &vbo_TexCoord_Pyramid);
            vbo_TexCoord_Pyramid= 0;
        }
        
        if (vbo_TexCoord_Cube)
        {
            glDeleteBuffers(1, &vbo_TexCoord_Cube);
            vbo_TexCoord_Cube= 0;
        }


        //step 10 : delete vbo for position

        if (vbo_Position_Pyramid)
        {
            glDeleteBuffers(1, &vbo_Position_Pyramid);
            vbo_Position_Pyramid = 0;
        }
        
        if (vbo_Position_Cube)
        {
            glDeleteBuffers(1, &vbo_Position_Cube);
            vbo_Position_Cube = 0;
        }
        //step 11 : delete VAO

        if (vao_Pyramid)
        {
            glDeleteVertexArrays(1, &vao_Pyramid);
            vao_Pyramid = 0;
        }
        
        
        if (vao_Cube)
        {
            glDeleteVertexArrays(1, &vao_Cube);
            vao_Cube = 0;
        }

    }
    
    -(BOOL)acceptsFirstResponder
    {
        //code
        
        [[self window]makeFirstResponder:self];
        
        return(YES);
    }
    
    -(void)keyDown:(NSEvent *)event
    {
        //code
        
        int key = (int)[[event characters]characterAtIndex:0];
        
        switch(key)
        {
            case 27:
                [self uninitialize];
                [self release];
                [NSApp terminate:self];
                
                break;
                
            case 'f':
            case'F':
                [[self window]toggleFullScreen:self];
                break;
                
        
            default:
                break;
        }
    }
    
    -(void)mouseDown:(NSEvent *)event
    {

        
    }
    
    -(void)rightMouseDown:(NSEvent *)event
    {
       
        
    }
    
    -(void)dealloc
    {
        [super dealloc];
        
        [self uninitialize];
        
        if(displayLink)
        {
            CVDisplayLinkStop(displayLink);
            CVDisplayLinkRelease(displayLink);
            displayLink = nil;
        }
        
        
        
        
        
        
        
        
    }


@end

//definition of global callback function

CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink,const CVTimeStamp* now,const CVTimeStamp* outputTime,CVOptionFlags flagsIn,CVOptionFlags *flagsOut,void*displayLinkContext)
{
    //code
    
    CVReturn result = [ (GLview *) displayLinkContext getFrameForTime:outputTime];
    
    return(result);
}




    
