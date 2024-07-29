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
    AMC_ATTRIBUTE_COLOR = 1,
    AMC_ATTRIBUTE_NORMAL = 2
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
        
        [window setTitle:@"Prathamesh Paropkari : Diffuse Light On Pyramid"];
        
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

    GLuint vaoPyramid = 0;
    GLuint vbo_Normal = 0;


    GLuint vbo_Position_Pyramid = 0;
    GLuint vbo_Color_Pyramid = 0;

    
    // uniforms

    GLuint modelViewMatrixUniform;
    GLuint projectionMatrixUniform = 0;
    GLuint ldUniform = 0;
    GLuint kdUniform = 0;
    GLuint lightPositionUniform = 0;
    GLuint keyPressUniform = 0;

    bool bLightingEnable = false;

    bool bAnimationEnable = false;


    //rotation variable

    GLfloat AnglePyramid = 0.0f;

   
    mat4 perspectiveProjectionMatrix;

    
    // Light Variables

    GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // white diffuse light
    GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
    GLfloat materialDiffuse[] = { 1.0f, 0.5f, 0.0f, 1.0f };


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
    
    if(bAnimationEnable == true)
    {
        
        [self myUpdate];
    }
    
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
                "in vec3 aNormal;\n"
                "uniform mat4 uModelViewMatrix;\n"
                "uniform mat4 uProjectionMatrix;\n"
                "uniform vec3 uld;\n"
                "uniform vec3 ukd;\n"
                "uniform vec4 uLightPosition;\n"
                "uniform int uKeyPress;\n"
                "out vec3 oDiffuseLight;\n"
                "in vec4 aColor;\n"
                "out vec4 oColor;\n"
                "void main(void)\n"
                "{\n"
                "if(uKeyPress == 1)"
                "{\n"
                "vec4 eyePosition = uModelViewMatrix * aPosition;\n"
                "mat3 normalMatrix = mat3(transpose(inverse(uModelViewMatrix)));\n"
                "vec3 n = normalize(normalMatrix * aNormal);\n"
                "vec3 s = normalize(vec3(uLightPosition - eyePosition));\n"
                "oDiffuseLight =  uld*ukd*dot(s,n);\n"
                "}\n"
                "else\n"
                "{\n"
                "oDiffuseLight = vec3( 0.0, 0.0, 0.0);\n"
                "}\n"
                "gl_Position = uProjectionMatrix * uModelViewMatrix * aPosition;\n"
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
            "in vec4 oColor;\n"
            "in vec3 oDiffuseLight;\n"
            "uniform int uKeyPress;\n"
            "out vec4 FragColor;\n"
            "void main(void)\n"
            "{\n"
            "if(uKeyPress == 1)\n"
            "{\n"
            "  FragColor = vec4 (oDiffuseLight, 1.0);\n"
            "}\n"
            "else\n"
            "{\n"
            "  FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
            "}\n"
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
        
        glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_COLOR, "aColor");
        
        glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_NORMAL , "aNormal");
        
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

        modelViewMatrixUniform = glGetUniformLocation(shaderProgramObject, "uModelViewMatrix");  //must be done after linkage of shader program
        
        projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "uProjectionMatrix");
        
        ldUniform = glGetUniformLocation(shaderProgramObject, "uld");

        kdUniform = glGetUniformLocation(shaderProgramObject, "ukd");

        lightPositionUniform = glGetUniformLocation(shaderProgramObject, "uLightPosition");

        keyPressUniform = glGetUniformLocation(shaderProgramObject, "uKeyPress");
      
        const GLfloat pyramidPosition[] = {
                                           // front
                                            0.0f, 1.0f, 0.0f,
                                            -1.0f, -1.0f, 1.0f,
                                            1.0f, -1.0f, 1.0f,

                                            // right
                                            0.0f, 1.0f, 0.0f,
                                            1.0f, -1.0f, 1.0f,
                                            1.0f, -1.0f, -1.0f,

                                            // back
                                           0.0f, 1.0f, 0.0f,
                                           1.0f, -1.0f, -1.0f,
                                           -1.0f, -1.0f, -1.0f,

                                           // left
                                           0.0f, 1.0f, 0.0f,
                                           -1.0f, -1.0f, -1.0f,
                                           -1.0f, -1.0f, 1.0f

                                           };
        
        // normals
             GLfloat pyramidNormals[] =
             {
                 // front
                 0.000000f, 0.447214f,  0.894427f, // front-top
                 0.000000f, 0.447214f,  0.894427f, // front-left
                 0.000000f, 0.447214f,  0.894427f, // front-right

                 // right
                 0.894427f, 0.447214f,  0.000000f, // right-top
                 0.894427f, 0.447214f,  0.000000f, // right-left
                 0.894427f, 0.447214f,  0.000000f, // right-right

                 // back
                 0.000000f, 0.447214f, -0.894427f, // back-top
                 0.000000f, 0.447214f, -0.894427f, // back-left
                 0.000000f, 0.447214f, -0.894427f, // back-right

                 // left
                -0.894427f, 0.447214f,  0.000000f, // left-top
                -0.894427f, 0.447214f,  0.000000f, // left-left
                -0.894427f, 0.447214f,  0.000000f, // left-right
             };



        const GLfloat pyramidColor[] = {
                                           1.0f, 1.0f, 1.0f,
                                           1.0f, 1.0f, 1.0f,
                                           1.0f, 1.0f, 1.0f,

                                           1.0f, 1.0f, 1.0f,
                                           1.0f, 1.0f, 1.0f,
                                           1.0f, 1.0f, 1.0f,

                                           1.0f, 1.0f, 1.0f,
                                           1.0f, 1.0f, 1.0f,
                                           1.0f, 1.0f, 1.0f,

                                           1.0f, 1.0f, 1.0f,
                                           1.0f, 1.0f, 1.0f,
                                           1.0f, 1.0f, 1.0f
                                                               };


       
                                      


        // ********************************************** for Pyramid ***********************************************

        glGenVertexArrays(1, &vaoPyramid);

        //step 14: Bind with VAO

        glBindVertexArray(vaoPyramid);

        //step 15: Create vertex buffer object (VBO) for position
        
        //VBO for position

        glGenBuffers(1, &vbo_Position_Pyramid);

        //step 16: Bind with VBO of position

        glBindBuffer(GL_ARRAY_BUFFER, vbo_Position_Pyramid);

        glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPosition), pyramidPosition, GL_STATIC_DRAW);

        glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

        glBindBuffer(GL_ARRAY_BUFFER, 0);


        //step 17: Repete VBO position steps for VBO color
        //VBO for color

        glGenBuffers(1, &vbo_Color_Pyramid);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_Color_Pyramid);

        glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidColor), pyramidColor, GL_STATIC_DRAW);

        glVertexAttribPointer(AMC_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        // Vbo For Pyramid Normals ***********************************
        glGenBuffers(1, &vbo_Normal);

            glBindBuffer(GL_ARRAY_BUFFER, vbo_Normal);

            glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidNormals), pyramidNormals, GL_STATIC_DRAW);

            glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

            glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);

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

        //Pyramid


        //transformations

          mat4 modelViewMatrix = mat4::identity();

          mat4 translationMatrix = mat4::identity();

          translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

          mat4 rotationMatrix = mat4::identity();

          rotationMatrix = vmath::rotate(AnglePyramid, 0.0f, 1.0f, 0.0f);



          modelViewMatrix = translationMatrix * rotationMatrix;

          //mat4 modelViewMatrix = vmath::translate(0.0f, 0.0f, -3.0f);

          mat4 modelViewProjectionMatrix = mat4::identity();

          modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;  //order of multiplication is not commutativ. i.e mat1 * mat2 != mat2 * mat1

          //push above mvp into vertex shader's mvp uniform


          glUniformMatrix4fv(modelViewMatrixUniform, 1, GL_FALSE, modelViewMatrix);

          glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

          if (bLightingEnable == true)
          {
              glUniform1i(keyPressUniform, 1);

              glUniform3fv(ldUniform, 1, LightDiffuse);

              glUniform3fv(kdUniform, 1, materialDiffuse);

              glUniform4fv(lightPositionUniform, 1, LightPosition);

          }
          else
          {
              glUniform1i(keyPressUniform, 0);
          }



          //step 2: Bind with (VAO)

          glBindVertexArray(vaoPyramid);

          //step 3: Draw the geometry

          glDrawArrays(GL_TRIANGLES, 0, 12);


          //step 4: unBind with (VAO)

          glBindVertexArray(0);

      

        

       
        //step 5: Unuse shader program object
        glUseProgram(0);
            
    
       
    }

    -(void)myUpdate
    {
        
        AnglePyramid = AnglePyramid - 0.2f;
        
        if (AnglePyramid <= 0.0f)
        {
            AnglePyramid = 360.0f;
        }
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

        if (vbo_Position_Pyramid)
        {
            glDeleteBuffers(1, &vbo_Position_Pyramid);
            vbo_Position_Pyramid = 0;
        }
        
        
        
        //delete vbo for color
        
        if (vbo_Color_Pyramid)
        {
            glDeleteBuffers(1, &vbo_Color_Pyramid);
            vbo_Color_Pyramid = 0;
        }
        
        
        //step 11 : delete VAO

        if (vaoPyramid)
        {
            glDeleteVertexArrays(1, &vaoPyramid);
            vaoPyramid = 0;
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
            case 'A':
            case 'a':
                if (bAnimationEnable == false)
                {
                    bAnimationEnable = true;
                }
                else
                {
                    bAnimationEnable = false;
                }
                break;
            case 'L':
            case 'l':
                if (bLightingEnable == false)
                {
                    bLightingEnable = true;
                }
                else
                {
                    bLightingEnable = false;
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




    
