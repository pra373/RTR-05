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
        
        [window setTitle:@"Prathamesh Paropkari: Tweaked Smiley"];
        
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

    GLuint vao;
    GLuint vbo_Position;
    GLuint vbo_TexCoord;

    

    //uniforms

    GLuint textureSamplerUniform = 0;

    GLuint mvpMatrixUniform;

    GLuint keyPressUniform;

    int iKeyPress = 0;

    //texture variable

    GLuint smiley_Texture;

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
            "uniform int uKeyPress;\n"
            "void main(void)\n"
            "{\n"
               "   if(uKeyPress == 0)\n"
               "   {\n"
               "        FragColor = vec4(1.0, 1.0, 1.0, 1.0);"
               "   }\n"
               "   else\n"
               "   {\n"
               "        FragColor = texture(uTextureSampler, oTexCoord);\n"
               "   }\n"            
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
        
        keyPressUniform = glGetUniformLocation(shaderProgramObject, "uKeyPress");
        
        
        const GLfloat rectanglePosition[] = { 1.0f, 1.0f, 0.0f,
                                             -1.0f,1.0f,0.0f,
                                              -1.0f,-1.0f,0.0f,
                                            -1.0f,-1.0f,0.0f,
                                            1.0f,-1.0f,0.0f,
                                            1.0f,1.0f,0.0f
                                            };
        
        /*const GLfloat rectangleTexCoord[] =
        {
            1.0f,1.0f,
            0.0f,1.0f,
            0.0f,0.0f,
            0.0f,0.0f,
            1.0f,0.0f,
            1.0f,1.0f

        };*/
         

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

         glBufferData(GL_ARRAY_BUFFER, sizeof(rectanglePosition), rectanglePosition, GL_STATIC_DRAW);

         glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

         glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

         glBindBuffer(GL_ARRAY_BUFFER, 0);

        //VBO for TEXCOORD

        glGenBuffers(1, &vbo_TexCoord);

        //step 16: Bind with VBO of TEXCOORD

        glBindBuffer(GL_ARRAY_BUFFER, vbo_TexCoord);

        //glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleTexCoord), rectangleTexCoord, GL_STATIC_DRAW);
        
        glBufferData(GL_ARRAY_BUFFER, (4 * 2 * sizeof(GL_FLOAT)), NULL, GL_DYNAMIC_DRAW);
        
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
        
        // Load texture
        
        smiley_Texture = [self loadGLTexture:"Smiley.bmp"];
        
        if (smiley_Texture == 0)
        {
            fprintf(gpFile, "Failed to load texture\n");
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
        //local variable
        
        GLfloat squareTexcord[12];
        
        //code
        
        
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //step 1: Use shader program object

        glUseProgram(shaderProgramObject);

        

        //transformations

        mat4 modelViewMatrix = vmath::translate(0.0f, 0.0f, -3.5f);

        mat4 madelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;  //order of multiplication is not commutativ. i.e mat1 * mat2 != mat2 * mat1

        //push above mvp into vertex shader's mvp uniform

        glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, madelViewProjectionMatrix);
        
        //for texture

        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, smiley_Texture);

        glUniform1i(textureSamplerUniform, 0);
        
        glBindVertexArray(vao);
        
        //texture coordinates

        if (iKeyPress == 1)
        {
            squareTexcord[0] = 1.0f;  //right top
            squareTexcord[1] = 1.0f;
            squareTexcord[2] = 0.0f;  //left top
            squareTexcord[3] = 1.0f;
            squareTexcord[4] = 0.0f;  //left bottom
            squareTexcord[5] = 0.0f;

            squareTexcord[6] = 0.0f;  //left bottom
            squareTexcord[7] = 0.0f;

            squareTexcord[8] = 1.0f;  //right bottom
            squareTexcord[9] = 0.0f;

            squareTexcord[10] = 1.0f;  //right top
            squareTexcord[11] = 1.0f;

            glUniform1i(keyPressUniform, 1);
        }

        else if (iKeyPress == 2)
        {
            squareTexcord[0] = 0.5f; //right top
            squareTexcord[1] = 0.5f;
            squareTexcord[2] = 0.0f; //left top
            squareTexcord[3] = 0.5f;
            squareTexcord[4] = 0.0f; //left bottom
            squareTexcord[5] = 0.0f;

            squareTexcord[6] = 0.0f; //left bottom
            squareTexcord[7] = 0.0f;

            squareTexcord[8] = 0.5f; //right bottom
            squareTexcord[9] = 0.0f;

            squareTexcord[10] = 0.5f; //right top
            squareTexcord[11] = 0.5f;

            glUniform1i(keyPressUniform, 1);
        }

        else if (iKeyPress == 3)
        {
            squareTexcord[0] = 2.0f; //right top
            squareTexcord[1] = 2.0f;

            squareTexcord[2] = 0.0f; //eft top
            squareTexcord[3] = 2.0f;

            squareTexcord[4] = 0.0f; //left bottom
            squareTexcord[5] = 0.0f;

            squareTexcord[6] = 0.0f; //left bottom
            squareTexcord[7] = 0.0f;

            squareTexcord[8] = 2.0f; //right bottom
            squareTexcord[9] = 0.0f;

            squareTexcord[10] = 2.0f; //right top
            squareTexcord[11] = 2.0f;


            glUniform1i(keyPressUniform, 1);
        }

        else if (iKeyPress == 4)
        {
            squareTexcord[0] = 0.5f;  //right top
            squareTexcord[1] = 0.5f;

            squareTexcord[2] = 0.5f; //lefttop
            squareTexcord[3] = 0.5f;

            squareTexcord[4] = 0.5f; //left bottom
            squareTexcord[5] = 0.5f;

            squareTexcord[6] = 0.5f; //left bottom
            squareTexcord[7] = 0.5f;


            squareTexcord[8] = 0.5f; //right bottom
            squareTexcord[9] = 0.5f;

            squareTexcord[10] = 0.5f;  //right top
            squareTexcord[11] = 0.5f;


            glUniform1i(keyPressUniform, 1);
        }

        else
        {
            glUniform1i(keyPressUniform, 0);
        }


        glBindBuffer(GL_ARRAY_BUFFER, vbo_TexCoord);
        glBufferData(GL_ARRAY_BUFFER, sizeof(squareTexcord), squareTexcord, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        
        //step 3: Draw the geometry

        glDrawArrays(GL_TRIANGLES, 0, 6);


        //step 4: unBind with (VAO)

        glBindVertexArray(0);

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
        
        //step 10 : delete vbo for texture

        if (vbo_TexCoord)
        {
            glDeleteBuffers(1, &vbo_TexCoord);
            vbo_TexCoord= 0;
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
                
            case 49:     // 1 is pressed
            
                iKeyPress = 1;
                glEnable(GL_TEXTURE_2D);
                break;
                
            case 50:     //2 is pressed
            
                iKeyPress = 2;
                glEnable(GL_TEXTURE_2D);
                break;
                
            case 51:     //3 is pressed
            
                iKeyPress = 3;
                glEnable(GL_TEXTURE_2D);
                break;  
                
            case 52:     //4 is pressed
           
                iKeyPress = 4;
                glEnable(GL_TEXTURE_2D);
                break;
                
            default:
                iKeyPress = -1;
                glDisable(TextureSmiley);
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




    
