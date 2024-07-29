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

#import<iostream>

#import<vector>

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
    AMC_ATTRIBUTE_COLOR = 1
    
};

enum
{
    AMC_ATTRIBUTE_POSITION_SHAPES = 0,
    AMC_ATTRIBUTE_COLOR_SHAPES

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
        
        [window setTitle:@"Prathamesh Paropkari : All shapes on Graph Paper"];
        
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
    GLuint shaderProgramObject_Shapes = 0;

    std::vector<GLfloat>linePositions;
    std::vector<GLfloat>circlePositions;
    std::vector<GLfloat>CircleColor;

    //GLuint vao;
    //GLuint vbo_Position;
    GLuint vbo_Color;

    GLuint vao = 0;
    GLuint vbo_Position = 0;

    GLuint vbo_Position_Triangle = 0;
    GLuint vbo_Color_Triangle = 0;

    GLuint vbo_Position_Square = 0;
    GLuint vbo_Color_Square = 0;

    GLuint vbo_Position_Circle = 0;
    GLuint vbo_Color_Circle = 0;

    GLuint vao_Triangle = 0;
    GLuint vao_Square = 0;
    GLuint vao_Circle = 0;

    GLuint mvpMatrixUniform;

    GLuint mvpMatrixUniform_Shapes = 0;

    mat4 perspectiveProjectionMatrix;

    bool enableGraph = false;
    bool enableTriangle = false;
    bool enableSquare = false;
    bool enableCircle = false;

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
        
        //variable declaration

        GLfloat upperX = 0.0f;
        GLfloat lowerX = 0.0f;
        GLfloat upperY = 1.0f;
        GLfloat lowerY = -1.0f;

        [self printGLInfo];   // copy paste point
        
        //vertex shader

        const GLchar* vertexShaderSourceCode =
            "#version 410 core\n"             // (version of GLSL * 100) core means you are using core profile i.e PP
            "\n"
            "in vec4 aPosition;\n"
            "out vec4 oColor;\n"
            "vec2 position = aPosition.xy;"
            "uniform mat4 uMVPMatrix;\n"          //u for uniform datatype
            "void main(void)\n"
            "{\n"
               "gl_Position=uMVPMatrix * vec4(position,0.0,1.0);\n"
               "if(gl_VertexID < 2)\n"
               "{\n"
                    "oColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
               "}\n"
               "else if(gl_VertexID >= 2 && gl_VertexID <= 3 )\n"
               "{\n"
                   "oColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
               "}\n"
               "else\n"
               "{\n"
                   "oColor = vec4(0.0, 0.0, 1.0, 1.0);\n"
               "}\n"
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
            "in vec4 oColor;"
            "out vec4 FragColor;\n"
            "void main(void)\n"
            "{\n"
            "FragColor = oColor;\n"                       //vec4 is working as a constructor here not as a datatype
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
        
        //glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_COLOR, "aColor");
        


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
        
        
        GLfloat lineArray[8] = {0.0, 1.0, 0.0, -1.0, -1.0, 0.0, 1.0, 0.0};
        
        for(int i=0; i<=7; i++)
        {
            linePositions.push_back(lineArray[i]);
        }
         
        
        for (int i = 0; i <= 39; i++)   //loop for +x vertical lines
         {
             upperX = upperX + 0.025f;
             lowerX = lowerX + 0.025f;

             linePositions.push_back(upperX);

             linePositions.push_back(upperY);

             linePositions.push_back(lowerX);

             linePositions.push_back(lowerY);

             
         }
        
        // ********************************** //
        upperX = 0.0f;
         lowerX = 0.0f;

         for (int i = 0; i <= 39; i++)   //loop for -X vertical lines
         {
             upperX = upperX - 0.025f;
             lowerX = lowerX - 0.025f;

             linePositions.push_back(upperX);

             linePositions.push_back(upperY);

             linePositions.push_back(lowerX);

             linePositions.push_back(lowerY);


         }
        
        // *********************************//
        GLfloat leftX = -1.0f;
         GLfloat rightX = 1.0f;
         
         GLfloat leftY = 0.0f;
         GLfloat rightY = 0.0f;

         for (int i = 0; i <= 39; i++)    //loop for +y vertical lines
         {
             leftY = leftY + 0.025f;
             rightY = rightY + 0.025f;

             linePositions.push_back(leftX);

             linePositions.push_back(leftY);

             linePositions.push_back(rightX);

             linePositions.push_back(rightY);


         }
        
        // ***************************************//
         leftX = -1.0f;
         rightX = 1.0f;

         leftY = 0.0f;
         rightY = 0.0f;

        for (int i = 0; i <= 39; i++)    //loop for -y vertical lines
        {
             leftY = leftY - 0.025f;
             rightY = rightY - 0.025f;

             linePositions.push_back(leftX);

             linePositions.push_back(leftY);

             linePositions.push_back(rightX);

             linePositions.push_back(rightY);

        }
        
        
         ////step 13: Create vertex array object
         //VAO

         glGenVertexArrays(1, &vao);

         //step 14: Bind with VAO

         glBindVertexArray(vao);

         //step 15: Create vertex buffer object (VBO) for position
         
         //VBO for position

         glGenBuffers(1, &vbo_Position);

         //step 16: Bind with VBO of position

         glBindBuffer(GL_ARRAY_BUFFER, vbo_Position);

         glBufferData(GL_ARRAY_BUFFER, linePositions.size()*sizeof(GLfloat), linePositions.data(), GL_STATIC_DRAW);

         glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);

         glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

         glBindBuffer(GL_ARRAY_BUFFER, 0);
        
    
         //step 18 : unbind with VAO
         glBindVertexArray(0);
        
        // ************* Shader Program For Shapes ************************* //
        
        const GLchar* vertexShaderSourceCode_Shapes =
             "#version 410 core\n"
             "in vec4 inputPosition;\n"
             "in vec4 inputColor;\n"
             "out vec4 FragColor;\n"
             "vec2 position = inputPosition.xy;"
             "uniform mat4 uMVPMatrixShapes;"
             "void main(void)\n"
             "{\n"
               "gl_Position = uMVPMatrixShapes * vec4(position, 0.0, 1.0);\n"
               "FragColor = inputColor;\n"
             "}\n";
        
        //step 2: Create vertex shader object

        GLuint vertexShaderObject_Shapes = glCreateShader(GL_VERTEX_SHADER);

         //step 3 : Give vertex shader code to OpenGL
         glShaderSource(vertexShaderObject_Shapes, 1, (const GLchar**)&vertexShaderSourceCode_Shapes, NULL);

         //Step 4 : Let OpenGL engine Compile the vertex shader code
         glCompileShader(vertexShaderObject_Shapes);

         status = 0;
         infoLogLength = 0;
         szInfoLog = NULL;

         //step 5 : Check for vertex shader compilation
         //step 5 (a) :  get compile status in status variable
         glGetShaderiv(vertexShaderObject_Shapes, GL_COMPILE_STATUS, &status);

         //step 5 (b) : if error occur i.e status == GL_FALSE,  get length of error message into infoLogLength variable
         if (status == GL_FALSE)
         {

             glGetShaderiv(vertexShaderObject_Shapes, GL_INFO_LOG_LENGTH, &infoLogLength);

             if (infoLogLength > 0)
             {
                 //step 5 (c) : declare a string variable allocate memory equal to above length i.e length of error message
                 szInfoLog = (GLchar*)malloc(infoLogLength);


                 if (szInfoLog != NULL)
                 {
                     // step 5 (d) : get the compilation error log info into memory allocated string
                     glGetShaderInfoLog(vertexShaderObject_Shapes, infoLogLength, NULL, szInfoLog);

                     //step 5 (e) : print error message into log file
                     fprintf(gpFile, "Vertex shader for shapes compilation error log : %s\n", szInfoLog);


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
        
        const GLchar* fragmentShaderSourceCode_Shapes =
         "#version 410 core\n"
         "in vec4 FragColor;\n"
         "out vec4 outputColor;\n"
         "void main(void)\n"
         "{\n"
         "outputColor = FragColor;\n"
         "}\n";

        GLuint fragmentShaderObject_Shapes = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(fragmentShaderObject_Shapes, 1, (const GLchar**)&fragmentShaderSourceCode_Shapes, NULL);

        glCompileShader(fragmentShaderObject_Shapes);

        status = 0;
        infoLogLength = 0;
        szInfoLog = NULL;

        glGetShaderiv(fragmentShaderObject_Shapes, GL_COMPILE_STATUS, &status);

        if (status == GL_FALSE)
        {

         glGetShaderiv(fragmentShaderObject_Shapes, GL_INFO_LOG_LENGTH, &infoLogLength);

         if (infoLogLength > 0)
         {
             szInfoLog = (GLchar*)malloc(infoLogLength);


             if (szInfoLog != NULL)
             {
                 glGetShaderInfoLog(fragmentShaderObject_Shapes, infoLogLength, NULL, szInfoLog);
                 fprintf(gpFile, "Fragment shader for shapes compilation error log : %s\n", szInfoLog);
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

             shaderProgramObject_Shapes = glCreateProgram();

             //step 8 : Attach both shaders into this program            also know as prelinking binding  should be done before linking and after attachment
             glAttachShader(shaderProgramObject_Shapes, vertexShaderObject_Shapes);
             glAttachShader(shaderProgramObject_Shapes, fragmentShaderObject_Shapes);

             //step 9: bind attribute location to the shader program object

             glBindAttribLocation(shaderProgramObject_Shapes, AMC_ATTRIBUTE_POSITION_SHAPES, "inputPosition");
             glBindAttribLocation(shaderProgramObject_Shapes, AMC_ATTRIBUTE_COLOR_SHAPES, "inputColor");


             //step 10: Link the shader program

             glLinkProgram(shaderProgramObject_Shapes);

             //step 11: do linking error check fpr shader program

             status = 0;
             infoLogLength = 0;
             szInfoLog = NULL;

             glGetProgramiv(shaderProgramObject_Shapes, GL_LINK_STATUS, &status);

             if (status == GL_FALSE)
             {
                 glGetProgramiv(shaderProgramObject_Shapes, GL_INFO_LOG_LENGTH, &infoLogLength);

                 if (infoLogLength > 0)
                 {
                     szInfoLog = (GLchar*)malloc(infoLogLength);


                     if (szInfoLog != NULL)
                     {
                         glGetProgramInfoLog(shaderProgramObject_Shapes, infoLogLength, NULL, szInfoLog);
                         fprintf(gpFile, "shader program for shapes linking error log : %s\n", szInfoLog);
                         free(szInfoLog);
                         szInfoLog = NULL;


                     }



                 }

                 [self uninitialize];
                 [self release];
                 [NSApp terminate:self];

             }
        
        //get Shader uniform Locations

         mvpMatrixUniform_Shapes = glGetUniformLocation(shaderProgramObject_Shapes, "uMVPMatrixShapes");  //must be done after linkage of shader program
        
        
        
        const GLfloat trianglePosition[] =
         {
            0.0f,0.5f,  //top vertex
            -0.5f,-0.5f, //left vertex

            -0.5f,-0.5f, //left vertex
            0.5f,-0.5f, //right vertex

            0.5f,-0.5f, //right vertex
            0.0f,0.5f,  //top vertex

         };

         const GLfloat triangleColor[] =
         {
             1.0f,1.0f,0.0f,
             1.0f,1.0f,0.0f,
             1.0f,1.0f,0.0f,
             1.0f,1.0f,0.0f,
             1.0f,1.0f,0.0f,
             1.0f,1.0f,0.0f
         };

         const GLfloat squarePosition[] =
         {
             0.5f,0.5f, //right top
             -0.5f,0.5f, //left top

             -0.5f,0.5f, //left top
             -0.5f,-0.5f, //left bottom

              -0.5f,-0.5f, //left bottom
             0.5f,-0.5f,   //right bottom

              0.5f,-0.5f,   //right bottom
              0.5f,0.5f     //right top


         };

         const GLfloat squareColor[] =
         {
             1.0f,1.0f,0.0f,
             1.0f,1.0f,0.0f,
             1.0f,1.0f,0.0f,
             1.0f,1.0f,0.0f,
             1.0f,1.0f,0.0f,
             1.0f,1.0f,0.0f,
             1.0f,1.0f,0.0f,
             1.0f,1.0f,0.0f
         };
        
        //code to find circle positions

         GLfloat radious = 0.5f;

         for (GLfloat angle = 0.0f; angle <= 360.0f; angle = angle + 0.01f)
         {

             //float radianangle = angle * (M_PI * 180.0f);
             GLfloat x = radious * cos((angle * M_PI) / 180.0f) + (0.0f);
             GLfloat y = radious * sin((angle * M_PI) / 180.0f) + (0.0f);

             circlePositions.push_back(x);
             circlePositions.push_back(y);


         }

         for (int i = 0; i < circlePositions.size(); i++)
         {
             CircleColor.push_back(1.0f);
             CircleColor.push_back(1.0f);
             CircleColor.push_back(0.0f);
         }
        
        
        ////step 13: Create vertex array object
        //VAO

         glGenVertexArrays(1, &vao_Triangle);

         //step 14: Bind with VAO

         glBindVertexArray(vao_Triangle);

         //step 15: Create vertex buffer object (VBO) for position

         //VBO for position

         glGenBuffers(1, &vbo_Position_Triangle);

         //step 16: Bind with VBO of position

         glBindBuffer(GL_ARRAY_BUFFER, vbo_Position_Triangle);

         glBufferData(GL_ARRAY_BUFFER, sizeof(trianglePosition), trianglePosition, GL_STATIC_DRAW);

         glVertexAttribPointer(AMC_ATTRIBUTE_POSITION_SHAPES, 2, GL_FLOAT, GL_FALSE, 0, NULL);

         glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION_SHAPES);

         glBindBuffer(GL_ARRAY_BUFFER, 0);

         //vbo for color

         glGenBuffers(1, &vbo_Color_Triangle);

         //step 16: Bind with VBO of position

         glBindBuffer(GL_ARRAY_BUFFER, vbo_Color_Triangle);

         glBufferData(GL_ARRAY_BUFFER, sizeof(triangleColor), triangleColor, GL_STATIC_DRAW);

         glVertexAttribPointer(AMC_ATTRIBUTE_COLOR_SHAPES, 3, GL_FLOAT, GL_FALSE, 0, NULL);

         glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR_SHAPES);

         glBindBuffer(GL_ARRAY_BUFFER, 0);

         //step 18 : unbind with VAO
         glBindVertexArray(0);
        
        
        //vao for square
          ////step 13: Create vertex array object
        //VAO

         glGenVertexArrays(1, &vao_Square);

         //step 14: Bind with VAO

         glBindVertexArray(vao_Square);

         //step 15: Create vertex buffer object (VBO) for position

         //VBO for position

         glGenBuffers(1, &vbo_Position_Square);

         //step 16: Bind with VBO of position

         glBindBuffer(GL_ARRAY_BUFFER, vbo_Position_Square);

         glBufferData(GL_ARRAY_BUFFER, sizeof(squarePosition), squarePosition, GL_STATIC_DRAW);

         glVertexAttribPointer(AMC_ATTRIBUTE_POSITION_SHAPES, 2, GL_FLOAT, GL_FALSE, 0, NULL);

         glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION_SHAPES);

         glBindBuffer(GL_ARRAY_BUFFER, 0);

         //vbo for color

         glGenBuffers(1, &vbo_Color_Square);

         //step 16: Bind with VBO of position

         glBindBuffer(GL_ARRAY_BUFFER, vbo_Color_Square);

         glBufferData(GL_ARRAY_BUFFER, sizeof(squareColor), squareColor, GL_STATIC_DRAW);

         glVertexAttribPointer(AMC_ATTRIBUTE_COLOR_SHAPES, 3, GL_FLOAT, GL_FALSE, 0, NULL);

         glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR_SHAPES);

         glBindBuffer(GL_ARRAY_BUFFER, 0);


         //step 18 : unbind with VAO
         glBindVertexArray(0);
        
        //VAO for circle

         glGenVertexArrays(1, &vao_Circle);

         //step 14: Bind with VAO

         glBindVertexArray(vao_Circle);

         //step 15: Create vertex buffer object (VBO) for position

         //VBO for position

         glGenBuffers(1, &vbo_Position_Circle);

         //step 16: Bind with VBO of position

         glBindBuffer(GL_ARRAY_BUFFER, vbo_Position_Circle);

         glBufferData(GL_ARRAY_BUFFER, circlePositions.size()*sizeof(float), circlePositions.data(), GL_STATIC_DRAW);

         glVertexAttribPointer(AMC_ATTRIBUTE_POSITION_SHAPES, 2, GL_FLOAT, GL_FALSE, 0, NULL);

         glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION_SHAPES);

         glBindBuffer(GL_ARRAY_BUFFER, 0);

         //vbo for color

         glGenBuffers(1, &vbo_Color_Circle);

         //step 16: Bind with VBO of position

         glBindBuffer(GL_ARRAY_BUFFER, vbo_Color_Circle);

         glBufferData(GL_ARRAY_BUFFER, CircleColor.size() * sizeof(float), CircleColor.data(), GL_STATIC_DRAW);

         glVertexAttribPointer(AMC_ATTRIBUTE_COLOR_SHAPES, 3, GL_FLOAT, GL_FALSE, 0, NULL);

         glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR_SHAPES);

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
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //step 1: Use shader program object

        glUseProgram(shaderProgramObject);

        
        //transformations

        mat4 modelViewMatrix = vmath::translate(0.0f, 0.0f, -2.7f);

        mat4 madelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;  //order of multiplication is not commutativ. i.e mat1 * mat2 != mat2 * mat1

        //push above mvp into vertex shader's mvp uniform

        glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, madelViewProjectionMatrix);

        glBindVertexArray(vao);


        //step 3: Draw the geometry
        if(enableGraph == true)
        {
            glDrawArrays(GL_LINES, 0, 324);
        }


        //step 4: unBind with (VAO)

        glBindVertexArray(0);
        
        // Triangle
        
        if (enableTriangle == true)
        {
            glUseProgram(shaderProgramObject_Shapes);

            mat4 modelViewMatrixShapes = mat4::identity();

            mat4 madelViewProjectionMatrixShapes = mat4::identity();

            modelViewMatrixShapes = vmath::translate(0.0f, 0.0f, -2.5f);

            madelViewProjectionMatrixShapes = perspectiveProjectionMatrix * modelViewMatrixShapes;  //order of multiplication is not commutativ. i.e mat1 * mat2 != mat2 * mat1

            //push above mvp into vertex shader's mvp uniform

            glUniformMatrix4fv(mvpMatrixUniform_Shapes, 1, GL_FALSE, madelViewProjectionMatrixShapes);

            glBindVertexArray(vao_Triangle);

            glPointSize(2.0f);
            
            glDrawArrays(GL_LINES, 0, 6);

            glBindVertexArray(0);

            glUseProgram(0);


        }
        
        // Square
        
        if (enableSquare == true)
        {
            glUseProgram(shaderProgramObject_Shapes);

            mat4 modelViewMatrixShapes = mat4::identity();

            mat4 madelViewProjectionMatrixShapes = mat4::identity();

            modelViewMatrixShapes = vmath::translate(0.0f, 0.0f, -2.5f);

            madelViewProjectionMatrixShapes = perspectiveProjectionMatrix * modelViewMatrixShapes;  //order of multiplication is not commutativ. i.e mat1 * mat2 != mat2 * mat1

            //push above mvp into vertex shader's mvp uniform

            glUniformMatrix4fv(mvpMatrixUniform_Shapes, 1, GL_FALSE, madelViewProjectionMatrixShapes);

            glBindVertexArray(vao_Square);

            glPointSize(2.0f);
            
            glDrawArrays(GL_LINES, 0, 8);

            glBindVertexArray(0);

            glUseProgram(0);
        }
        
        
        // Circle
        
        if (enableCircle == true)
        {
            glUseProgram(shaderProgramObject_Shapes);

            mat4 modelViewMatrixShapes = mat4::identity();

            mat4 madelViewProjectionMatrixShapes = mat4::identity();

            modelViewMatrixShapes = vmath::translate(0.0f, 0.0f, -2.5f);

            madelViewProjectionMatrixShapes = perspectiveProjectionMatrix * modelViewMatrixShapes;  //order of multiplication is not commutativ. i.e mat1 * mat2 != mat2 * mat1

            //push above mvp into vertex shader's mvp uniform

            glUniformMatrix4fv(mvpMatrixUniform_Shapes, 1, GL_FALSE, madelViewProjectionMatrixShapes);

            glBindVertexArray(vao_Circle);

            glPointSize(2.0f);

            glDrawArrays(GL_POINTS, 0, (circlePositions.size()/2));

            glPointSize(1.0f);

            glBindVertexArray(0);

            glUseProgram(0);
        }
        //step 5: Unuse shader program object
        glUseProgram(0);

       
    }

    -(void)myUpdate
    {
        //code
        
        
    }

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



        //step 10 : delete vbo for position

        if (vbo_Position)
        {
            glDeleteBuffers(1, &vbo_Position);
            vbo_Position = 0;
        }

        //step 11 : delete VAO

        if (vao)
        {
            glDeleteVertexArrays(1, &vao);
            vao = 0;
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
                
            case 'g':
            case 'G':
                if (enableGraph == false)
                {
                    
                    enableGraph = true;
                    enableCircle = false;
                    enableSquare = false;
                    enableTriangle = false;
                }
                else
                {
                    
                    enableGraph = false;
                }
                break;
                
            case 'T':
            case 't':
                if (enableTriangle == false)
                {

                    enableTriangle = true;
                    enableSquare = false;
                    enableCircle = false;
                }
                else
                {

                    enableTriangle = false;
                }
                break;

            case 's':
            case 'S':
                if (enableSquare == false)
                {

                    enableSquare = true;
                    enableTriangle = false;
                    enableCircle = false;
                }
                else
                {

                    enableSquare = false;
                }
                break;

            case 'c':
            case 'C':
                if (enableCircle == false)
                {

                    enableCircle = true;
                    enableTriangle = false;
                    enableSquare = false;
                }
                else
                {

                    enableCircle = false;
                }
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




    
