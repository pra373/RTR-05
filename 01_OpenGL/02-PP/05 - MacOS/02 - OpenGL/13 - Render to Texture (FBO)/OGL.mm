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

#import "sphere.h"

#import"vmath.h"

using namespace vmath;

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define FBO_WIDTH 512
#define FBO_HEIGHT 512
//global function declaration

CVReturn MyDisplayLinkCallback(CVDisplayLinkRef,const CVTimeStamp *,const CVTimeStamp *,CVOptionFlags,CVOptionFlags *,void *);

//global variable declarations

FILE *gpFile = NULL;

/*enum
{
    AMC_ATTRIBUTE_POSITION = 0,
    AMC_ATTRIBUTE_TEXTURE = 1,
    AMC_ATTRIBUTE_NORMAL = 2
};*/

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
        
        [window setTitle:@"Prathamesh Paropkari: Render To Texture (Frame Buffer Object)"];
        
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

  //Cube related variables

    GLuint shaderProgramObject_Cube;

    GLuint vao_Cube;
    GLuint vbo_Position_Cube;
    GLuint vbo_TexCoord_Cube;

    GLfloat AngleCube = 0.0f;
    
    GLint winWidth;
    	GLint winHeight;

    //uniforms

    GLuint textureSamplerUniform_Cube = 0;

    GLuint mvpMatrixUniform_Cube;


   

    mat4 perspectiveProjectionMatrix_Cube;

    //sphere related variables



   GLuint shaderProgramObject_Sphere;
   GLuint shaderProgramObject_Sphere_PF;

   mesh sphere;

   GLuint mvpMatrixUniform_Sphere;

   mat4 perspectiveProjectionMatrix_Sphere;

   // Uniforms for per vertex

   GLuint modelMatrixUniform_Sphere = 0;
   GLuint viewMatrixUniform_Sphere = 0;
   GLuint projectionMatrixUniform_Sphere = 0;

   GLuint lightDiffuseUniform_Sphere[3];
   GLuint lightAmbientUniform_Sphere[3];
   GLuint lightSpecularUniform_Sphere[3];
   GLuint lightPositionUniform_Sphere[3];

   GLuint materialDiffuseUniform_Sphere = 0;
   GLuint materialAmbientUniform_Sphere = 0;
   GLuint materialSpecularUniform_Sphere = 0;
   GLuint materialShininessUniform_Sphere = 0;

   GLuint keyPressUniform_Sphere = 0;

   // Uniforms for per vertex

   GLuint modelMatrixUniform_Sphere_PF = 0;
   GLuint viewMatrixUniform_Sphere_PF = 0;
   GLuint projectionMatrixUniform_Sphere_PF = 0;

   GLuint lightDiffuseUniform_Sphere_PF[3];
   GLuint lightAmbientUniform_Sphere_PF[3];
   GLuint lightSpecularUniform_Sphere_PF[3];
   GLuint lightPositionUniform_Sphere_PF[3];

   GLuint materialDiffuseUniform_Sphere_PF = 0;
   GLuint materialAmbientUniform_Sphere_PF = 0;
   GLuint materialSpecularUniform_Sphere_PF = 0;
   GLuint materialShininessUniform_Sphere_PF = 0;

   GLuint keyPressUniform_Sphere_PF = 0;

   bool bLightingEnable = false;

   bool bAnimationEnable = false;

   bool bEnableVertexLighting = false;
   bool bEnableFragmentLighting = false;

    //texture variable
    GLuint texture_FBO;
   
   // Light Variables

   GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // white diffuse light
   GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };

   GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
   GLfloat materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
   GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
   GLfloat materialShininess = 128.0f;

   //per fragment lights

   GLfloat light1Ambient[3] = {0.0, 0.0, 0.0};
   GLfloat light2Ambient[3] = {0.0, 0.0, 0.0};
   GLfloat light1Diffuse[3] = {1.0, 0.0, 0.0};
   GLfloat light2Diffuse[3] = {0.0, 0.0, 1.0};
   GLfloat light1Specular[3] = {1.0, 0.0, 0.0};
   GLfloat light2Specular[3] = {0.0, 0.0, 1.0};
   GLfloat light1Position[4] = {-2.0, 0.0, 1.0, 1.0};
   GLfloat light2Position[4] = {2.0, 0.0, 1.0, 1.0};

   GLfloat light3Ambient[3] = {0.0, 0.0, 0.0};
   GLfloat light3Diffuse[3] = {0.0, 1.0, 0.0};
   GLfloat light3Specular[3] = {0.0, 1.0, 0.0};
   GLfloat light3Position[4] = {0.0, 0.0, 1.0, 1.0};

   //per fragment lights
   
   GLfloat light1Ambient_F[3] = {0.0, 0.0, 0.0};

   GLfloat light2Ambient_F[3] = {0.0, 0.0, 0.0};

   GLfloat light3Ambient_F[3] = {0.0, 0.0, 0.0};



   GLfloat light1Diffuse_F[3] = {1.0, 0.0, 0.0};

   GLfloat light2Diffuse_F[3] = {0.0, 1.0, 0.0};

   GLfloat light3Diffuse_F[3] = {0.0, 0.0, 1.0};



   GLfloat light1Specular_F[3] = {1.0, 0.0, 0.0};

   GLfloat light2Specular_F[3] = {0.0, 1.0, 0.0};

   GLfloat light3Specular_F[3] = {0.0, 0.0, 1.0};



   GLfloat light1Position_F[4] = {-2.0, 0.0, 1.0, 1.0};

   GLfloat light2Position_F[4] = {2.0, 0.0, 1.0, 1.0};

   GLfloat light3Position_F[4] = {0.0, 0.0, 1.0, 1.0};

   //light movement variable

   GLfloat vLight0 = 0.0f;
   GLfloat vLight1 = 0.0f;
   GLfloat vLight2 = 0.0f;

   GLfloat fLight0 = 0.0f;
   GLfloat fLight1 = 0.0f;
   GLfloat fLight2 = 0.0f;  

    // FBO related variables
    
    GLuint fbo;
    GLuint rbo;
    GLuint fbo_Texture;
    bool bFBOResult = false;

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
                [self uninitialize_Cube];
                [self release];
                [NSApp terminate:self];
            }
            //step3: Create OpenGL Context using above Pixel Format
            
            NSOpenGLContext *glContext = [[[NSOpenGLContext alloc]initWithFormat:pixelFormat shareContext:nil]autorelease];
            
            if(glContext == nil)
            {
                fprintf(gpFile,"glContext Creation Failed");
                [self uninitialize_Cube];
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
        
        int result = [self initialize_Cube];
        
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
        
        [self resize_Cube :width:height];
        
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
        
        [self display_Cube];
        
        CGLFlushDrawable((CGLContextObj)[[self openGLContext]CGLContextObj]);
        
        CGLUnlockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
        
        
    }

    //below function gets called per frame using auto release pool

-(CVReturn)getFrameForTime:(const CVTimeStamp *) outputTime
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    //call rendering function here too
    
    [self drawView];
    
    [self myUpdate_Cube];
    
    [pool release];
    
    return(kCVReturnSuccess);
    
}

    -(int)initialize_Cube
    {
        //code
        
        [self printGLInfo];   // copy paste point
        
        //vertex shader

        const GLchar* vertexShaderSourceCode_Cube =
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

        GLuint vertexShaderObject_Cube = glCreateShader(GL_VERTEX_SHADER);

        //step 3 : Give vertex shader code to OpenGL
        glShaderSource(vertexShaderObject_Cube, 1, (const GLchar**)&vertexShaderSourceCode_Cube, NULL);

        //Step 4 : Let OpenGL engine Compile the vertex shader code
        glCompileShader(vertexShaderObject_Cube);

        GLint status = 0;
        GLint infoLogLength = 0;
        GLchar* szInfoLog = NULL;

        //step 5 : Check for vertex shader compilation
        //step 5 (a) :  get compile status in status variable
        glGetShaderiv(vertexShaderObject_Cube, GL_COMPILE_STATUS, &status);

        //step 5 (b) : if error occur i.e status == GL_FALSE,  get length of error message into infoLogLength variable
        if (status == GL_FALSE)
        {
             
            glGetShaderiv(vertexShaderObject_Cube, GL_INFO_LOG_LENGTH, &infoLogLength);

            if (infoLogLength > 0)
            {
                //step 5 (c) : declare a string variable allocate memory equal to above length i.e length of error message
                szInfoLog = (GLchar*)malloc(infoLogLength);


                if (szInfoLog != NULL)
                {
                    // step 5 (d) : get the compilation error log info into memory allocated string
                    glGetShaderInfoLog(vertexShaderObject_Cube, infoLogLength, NULL, szInfoLog);

                    //step 5 (e) : print error message into log file
                    fprintf(gpFile, "Vertex shader compilation error log : %s\n", szInfoLog);


                    //step 5 (f) : free allocated memory
                    free(szInfoLog);
                    szInfoLog = NULL;


                    //step 5 (g) : uninitialize and exit.
                    

                }

            }

            [self uninitialize_Cube];
            [self release];
            [NSApp terminate:self];
        }

        //follow all steps of vertex shader for fragment shader

        //step 6  : Write the source code for fragment shader

        //fragment shader

        const GLchar* fragmentShaderSourceCode_Cube =
            "#version 410 core\n"
            "\n"
            "out vec4 FragColor;\n"
            "in vec2 oTexCoord;\n"
            "uniform sampler2D uTextureSampler;\n"
            "void main(void)\n"
            "{\n"
               "FragColor = texture(uTextureSampler, oTexCoord);\n"
            "}";

        GLuint fragmentShaderObject_Cube = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(fragmentShaderObject_Cube, 1, (const GLchar**)&fragmentShaderSourceCode_Cube, NULL);

        glCompileShader(fragmentShaderObject_Cube);

         status = 0;
         infoLogLength = 0;
         szInfoLog = NULL;

         glGetShaderiv(fragmentShaderObject_Cube, GL_COMPILE_STATUS, &status);

         if (status == GL_FALSE)
         {
            
             glGetShaderiv(fragmentShaderObject_Cube, GL_INFO_LOG_LENGTH, &infoLogLength);

             if (infoLogLength > 0)
             {
                 szInfoLog = (GLchar*)malloc(infoLogLength);


                 if (szInfoLog != NULL)
                 {
                     glGetShaderInfoLog(fragmentShaderObject_Cube, infoLogLength, NULL, szInfoLog);
                     fprintf(gpFile, "Fragment shader compilation error log : %s\n", szInfoLog);
                     free(szInfoLog);
                     szInfoLog = NULL;
                     

                 }

                 

             }

             [self uninitialize_Cube];
             [self release];
             [NSApp terminate:self];
         }

         //linking process for shader program.

         // shader program

         //step 7 : create shader program object

         shaderProgramObject_Cube = glCreateProgram();

         //step 8 : Attach both shaders into this program            also know as prelinking binding  should be done before linking and after attachment
         glAttachShader(shaderProgramObject_Cube, vertexShaderObject_Cube);
         glAttachShader(shaderProgramObject_Cube, fragmentShaderObject_Cube);

         //step 9: bind attribute location to the shader program object

         glBindAttribLocation(shaderProgramObject_Cube, AMC_ATTRIBUTE_POSITION, "aPosition");
        
        glBindAttribLocation(shaderProgramObject_Cube, AMC_ATTRIBUTE_TEXTURE, "aTexCoord");

         //step 10: Link the shader program

         glLinkProgram(shaderProgramObject_Cube);

         //step 11: do linking error check fpr shader program

         status = 0;
         infoLogLength = 0;
         szInfoLog = NULL;

         glGetProgramiv(shaderProgramObject_Cube, GL_LINK_STATUS, &status);

         if (status == GL_FALSE)
         {
             glGetProgramiv(shaderProgramObject_Cube, GL_INFO_LOG_LENGTH, &infoLogLength);

             if (infoLogLength > 0)
             {
                 szInfoLog = (GLchar*)malloc(infoLogLength);


                 if (szInfoLog != NULL)
                 {
                     glGetProgramInfoLog(shaderProgramObject_Cube, infoLogLength, NULL, szInfoLog);
                     fprintf(gpFile, "shader program linking error log : %s\n", szInfoLog);
                     free(szInfoLog);
                     szInfoLog = NULL;
                    

                 }



             }

             [self uninitialize_Cube];
             [self release];
             [NSApp terminate:self];
         }

         //step 12: Declare position and color arrays

         //get Shader uniform Locations

         mvpMatrixUniform_Cube = glGetUniformLocation(shaderProgramObject_Cube, "uMVPMatrix");  //must be done after linkage of shader program
        
        textureSamplerUniform_Cube = glGetUniformLocation(shaderProgramObject_Cube, "uTextureSampler");
        
        if (textureSamplerUniform_Cube == -1)
        {
            fprintf(gpFile, "Loading of uniform variable failed !! Exiting now\n");
            return(-7);
        }
        
       
        
        
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

         glGenVertexArrays(1, &vao_Cube);

         //step 14: Bind with VAO

         glBindVertexArray(vao_Cube);

         //step 15: Create vertex buffer object (VBO) for position
         
         //VBO for position

         glGenBuffers(1, &vbo_Position_Cube);

         //step 16: Bind with VBO of position

         glBindBuffer(GL_ARRAY_BUFFER, vbo_Position_Cube);

         glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices),cubeVertices, GL_STATIC_DRAW);

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
        
        glEnable(GL_TEXTURE_2D);
        
        //clear color
        
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        
        perspectiveProjectionMatrix_Cube = vmath::mat4::identity();
        
        // Load texture
        
        /*kundali_Texture = [self loadGLTexture:"Kundali.bmp"];
        
        if (kundali_Texture == 0)
        {
            fprintf(gpFile, "Failed to load texture\n");
            [self uninitialize_Cube];
            [self release];
            [NSApp terminate:self];
        }*/
        
        //FBO related code
        
        //FBO related code

        if ([self createFBO:FBO_WIDTH:FBO_HEIGHT] == true)
        {
            bFBOResult = [self initialize_Sphere:FBO_WIDTH:FBO_HEIGHT];
        }
        
        [self resize_Cube:WIN_WIDTH:WIN_HEIGHT];
        
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

    -(bool)initialize_Sphere : (GLint)textureWidth : (GLint)textureHeight
    {
       //code
       
       
       
       //vertex shader
       
       const GLchar* vertexShaderSourceCode =
       "#version 410 core\n"             // (version of GLSL * 100) core means you are using core profile i.e PP
       "\n"
       "in vec4 aPosition;\n"
       "in vec3 aNormal;\n"
       "uniform mat4 uModelMatrix;\n"
       "uniform mat4 uViewMatrix;\n"
       "uniform mat4 uProjectionMatrix;\n"
       "uniform vec3 ulightAmbient[3];\n"
       "uniform vec3 ulightDiffuse[3];\n"
       "uniform vec3 ulightSpecular[3];\n"
       "uniform vec4 ulightPosition[3];\n"
       "uniform vec3 uMaterialAmbient;\n"
       "uniform vec3 uMaterialDiffuse;\n"
       "uniform vec3 uMaterialSpecular;\n"
       "uniform float uMaterialShininess;\n"
       "uniform int uKeyPress;\n"
       "out vec3 oPhongADSLight;\n"
       "void main(void)\n"
       "{\n"
       "oPhongADSLight = vec3(0.0, 0.0, 0.0);\n"
       "if(uKeyPress == 1)\n"
       "{\n"
       "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;\n"
       "vec3 transformedNormals = normalize(mat3(uViewMatrix * uModelMatrix) * aNormal);\n"
       "vec3 lightAmbient[3];\n"
       "vec3 lightDirection[3];\n"
       "vec3 lightDiffuse[3];\n"
       "vec3 reflectionVector[3];\n"
       "vec3 lightSpecular[3];\n"
       "for(int i = 0; i < 3; i++)\n"
       "{\n"
       "lightAmbient[i] = ulightAmbient[i] * uMaterialAmbient;\n"
       "lightDirection[i] = normalize(vec3 (ulightPosition[i] - eyeCoordinates));\n"
       "lightDiffuse[i] = ulightDiffuse[i] * uMaterialDiffuse * max(dot(lightDirection[i], transformedNormals),0.0);\n"
       "reflectionVector[i] = reflect(-lightDirection[i], transformedNormals);\n"
       "vec3 viewerVector = normalize (- eyeCoordinates.xyz);\n"
       "lightSpecular[i] = ulightSpecular[i] * uMaterialSpecular * pow(max(dot(reflectionVector[i], viewerVector), 0.0),uMaterialShininess);\n"
       "oPhongADSLight = oPhongADSLight + lightAmbient[i] + lightDiffuse[i] + lightSpecular[i];\n"
       "}\n"
       "}\n"
       "else\n"
       "{\n"
       "oPhongADSLight = vec3( 0.0, 0.0, 0.0);\n"
       "}\n"
       "gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;\n"
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
                   
                   
                   //step 5 (g) : uninitialize_Sphere and exit.
                   
                   
               }
               
           }
           
           [self uninitialize_Sphere];
           [self release];
           [NSApp terminate:self];
       }
       
       //follow all steps of vertex shader for fragment shader
       
       //step 6  : Write the source code for fragment shader
       
       //fragment shader
       
       const GLchar* fragmentShaderSourceCode =
       "#version 410 core\n"
       "\n"
       "in vec3 oPhongADSLight;\n"
       "uniform int uKeyPress;\n"
       "out vec4 FragColor;\n"
       "void main(void)\n"
       "{\n"
       "if(uKeyPress == 1)\n"
       "{\n"
       "  FragColor = vec4 (oPhongADSLight, 1.0);\n"
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
           
           [self uninitialize_Sphere];
           [self release];
           [NSApp terminate:self];
       }
       
       //linking process for shader program.
       
       // shader program
       
       //step 7 : create shader program object
       
       shaderProgramObject_Sphere = glCreateProgram();
       
       //step 8 : Attach both shaders into this program            also know as prelinking binding  should be done before linking and after attachment
       glAttachShader(shaderProgramObject_Sphere, vertexShaderObject);
       glAttachShader(shaderProgramObject_Sphere, fragmentShaderObject);
       
       //step 9: bind attribute location to the shader program object
       
       glBindAttribLocation(shaderProgramObject_Sphere, AMC_ATTRIBUTE_POSITION, "aPosition");

       
       glBindAttribLocation(shaderProgramObject_Sphere, AMC_ATTRIBUTE_NORMAL , "aNormal");
       
       //step 10: Link the shader program
       
       glLinkProgram(shaderProgramObject_Sphere);
       
       //step 11: do linking error check fpr shader program
       
       status = 0;
       infoLogLength = 0;
       szInfoLog = NULL;
       
       glGetProgramiv(shaderProgramObject_Sphere, GL_LINK_STATUS, &status);
       
       if (status == GL_FALSE)
       {
           glGetProgramiv(shaderProgramObject_Sphere, GL_INFO_LOG_LENGTH, &infoLogLength);
           
           if (infoLogLength > 0)
           {
               szInfoLog = (GLchar*)malloc(infoLogLength);
               
               
               if (szInfoLog != NULL)
               {
                   glGetProgramInfoLog(shaderProgramObject_Sphere, infoLogLength, NULL, szInfoLog);
                   fprintf(gpFile, "shader program linking error log : %s\n", szInfoLog);
                   free(szInfoLog);
                   szInfoLog = NULL;
                   
                   
               }
               
               
               
           }
           
           [self uninitialize_Sphere];
           [self release];
           [NSApp terminate:self];
       }
       
       //step 12: Declare position and color arrays
       
       //get Shader uniform Locations
       
       // mvpMatrixUniform_Sphere = glGetUniformLocation(shaderProgramObject_Sphere, "uMVPMatrix");  //must be done after linkage of shader program
       
       modelMatrixUniform_Sphere = glGetUniformLocation(shaderProgramObject_Sphere, "uModelMatrix");
       
       viewMatrixUniform_Sphere = glGetUniformLocation(shaderProgramObject_Sphere, "uViewMatrix");
       
       projectionMatrixUniform_Sphere = glGetUniformLocation(shaderProgramObject_Sphere, "uProjectionMatrix");
       
       lightAmbientUniform_Sphere[0] = glGetUniformLocation(shaderProgramObject_Sphere, "ulightAmbient[0]");
       
       lightDiffuseUniform_Sphere[0] = glGetUniformLocation(shaderProgramObject_Sphere, "ulightDiffuse[0]");
       
       lightSpecularUniform_Sphere[0] = glGetUniformLocation(shaderProgramObject_Sphere, "ulightSpecular[0]");
       
       lightPositionUniform_Sphere[0] = glGetUniformLocation(shaderProgramObject_Sphere, "ulightPosition[0]");
       
       
       lightAmbientUniform_Sphere[1] = glGetUniformLocation(shaderProgramObject_Sphere, "ulightAmbient[1]");
       
       lightDiffuseUniform_Sphere[1] = glGetUniformLocation(shaderProgramObject_Sphere, "ulightDiffuse[1]");
       
       lightSpecularUniform_Sphere[1] = glGetUniformLocation(shaderProgramObject_Sphere, "ulightSpecular[1]");
       
       lightPositionUniform_Sphere[1] = glGetUniformLocation(shaderProgramObject_Sphere, "ulightPosition[1]");
       
       lightAmbientUniform_Sphere[2] = glGetUniformLocation(shaderProgramObject_Sphere, "ulightAmbient[2]");
       
       lightDiffuseUniform_Sphere[2] = glGetUniformLocation(shaderProgramObject_Sphere, "ulightDiffuse[2]");
       
       lightSpecularUniform_Sphere[2] = glGetUniformLocation(shaderProgramObject_Sphere, "ulightSpecular[2]");
       
       lightPositionUniform_Sphere[2] = glGetUniformLocation(shaderProgramObject_Sphere, "ulightPosition[2]");
       
       
       materialAmbientUniform_Sphere = glGetUniformLocation(shaderProgramObject_Sphere, "uMaterialAmbient");
       
       materialDiffuseUniform_Sphere = glGetUniformLocation(shaderProgramObject_Sphere, "uMaterialDiffuse");
       
       
       materialSpecularUniform_Sphere = glGetUniformLocation(shaderProgramObject_Sphere, "uMaterialSpecular");
       
       materialShininessUniform_Sphere = glGetUniformLocation(shaderProgramObject_Sphere, "uMaterialShininess");
       
       keyPressUniform_Sphere = glGetUniformLocation(shaderProgramObject_Sphere, "uKeyPress");
       
       // ****************************** shader program for per fragment lighting ****************
       
       //vertex shader
       
       const GLchar* vertexShaderSourceCode_PF =
               "#version 410 core\n"             // (version of GLSL * 100) core means you are using core profile i.e PP
               "\n"
               "in vec4 aPosition;\n"
               "in vec3 aNormal;\n"
               "uniform mat4 uModelMatrix;\n"
               "uniform mat4 uViewMatrix;\n"
               "uniform mat4 uProjectionMatrix;\n"
               "uniform vec4 ulightPosition[3];\n"
               "uniform int uKeyPress;\n"
               "out vec3 otransformedNormals;\n"
               "out vec3 olightDirection[3];\n"
               "out vec3 oviewerVector;\n"
               "void main(void)\n"
               "{\n"
               "if(uKeyPress == 1)\n"
               "{\n"
               "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;\n"
               "otransformedNormals = (mat3(uViewMatrix * uModelMatrix) * aNormal);\n"
               "oviewerVector = -eyeCoordinates.xyz;\n"
               "for(int i = 0; i < 3; i++)\n"
               "{\n"
               "olightDirection[i] = vec3(ulightPosition[i] - eyeCoordinates);\n"
               "}\n"
               "}\n"
               "else\n"
               "{\n"
               "for(int i = 0; i < 3; i++)\n"
               "{\n"
               "olightDirection[i] = vec3(0.0, 0.0, 0.0);\n"
               "}\n"
               "otransformedNormals =  vec3(0.0, 0.0, 0.0);\n"
               "oviewerVector = vec3(0.0, 0.0, 0.0);\n"
               "}\n"
               "gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;\n"
               "}\n";
       
       //step 2: Create vertex shader object
       
       GLuint vertexShaderObject_PF = glCreateShader(GL_VERTEX_SHADER);
       
       //step 3 : Give vertex shader code to OpenGL
       glShaderSource(vertexShaderObject_PF, 1, (const GLchar**)&vertexShaderSourceCode_PF, NULL);
       
       //Step 4 : Let OpenGL engine Compile the vertex shader code
       glCompileShader(vertexShaderObject_PF);
       
        status = 0;
        infoLogLength = 0;
       szInfoLog = NULL;
       
       //step 5 : Check for vertex shader compilation
       //step 5 (a) :  get compile status in status variable
       glGetShaderiv(vertexShaderObject_PF, GL_COMPILE_STATUS, &status);
       
       //step 5 (b) : if error occur i.e status == GL_FALSE,  get length of error message into infoLogLength variable
       if (status == GL_FALSE)
       {
           
           glGetShaderiv(vertexShaderObject_PF, GL_INFO_LOG_LENGTH, &infoLogLength);
           
           if (infoLogLength > 0)
           {
               //step 5 (c) : declare a string variable allocate memory equal to above length i.e length of error message
               szInfoLog = (GLchar*)malloc(infoLogLength);
               
               
               if (szInfoLog != NULL)
               {
                   // step 5 (d) : get the compilation error log info into memory allocated string
                   glGetShaderInfoLog(vertexShaderObject_PF, infoLogLength, NULL, szInfoLog);
                   
                   //step 5 (e) : print error message into log file
                   fprintf(gpFile, "Vertex shader compilation error log : %s\n", szInfoLog);
                   
                   
                   //step 5 (f) : free allocated memory
                   free(szInfoLog);
                   szInfoLog = NULL;
                   
                   
                   //step 5 (g) : uninitialize_Sphere and exit.
                   
                   
               }
               
           }
           
           [self uninitialize_Sphere];
           [self release];
           [NSApp terminate:self];
       }
       
       //fragment shader

       const GLchar* fragmentShaderSourceCode_PF =
               "#version 410 core\n"
               "\n"
               "in vec3 PhongADSLight;\n"
               "in vec3 otransformedNormals;\n"
               "in vec3 olightDirection[3];\n"
               "in vec3 oviewerVector;\n"
               "uniform mat4 uModelMatrix;\n"
               "uniform mat4 uViewMatrix;\n"
               "uniform mat4 uProjectionMatrix;\n"
               "uniform vec3 ulightAmbient[3];\n"
               "uniform vec3 ulightDiffuse[3];\n"
               "uniform vec3 ulightSpecular[3];\n"
               "uniform vec3 uMaterialAmbient;\n"
               "uniform vec3 uMaterialDiffuse;\n"
               "uniform vec3 uMaterialSpecular;\n"
               "uniform float uMaterialShininess;\n"
               "uniform int uKeyPress;\n"
               "out vec4 FragColor;\n"
               "void main(void)\n"
               "{\n"
               "vec3 PhongADSLight = vec3(0.0, 0.0, 0.0);\n" // local variable
               "if(uKeyPress == 1)\n"
               "{\n"
               "vec3 normalizedLightDirection[3];\n"
               "vec3 normalizedTransformedNormals = normalize(otransformedNormals);\n"
               "vec3 normalizedViwerVector = normalize(oviewerVector);\n"
               "vec3 lightAmbient[3];\n"
               "vec3 lightDiffuse[3];\n"
               "vec3 reflectionVector[3];\n"
               "vec3 lightSpecular[3];\n"
               "for(int i = 0; i < 3; i++)\n"
               "{\n"
               "lightAmbient[i] = ulightAmbient[i] * uMaterialAmbient;\n"
               "normalizedLightDirection[i] = normalize(olightDirection[i]);\n"
               "lightDiffuse[i] = ulightDiffuse[i] * uMaterialDiffuse * max(dot(normalizedLightDirection[i], normalizedTransformedNormals),0.0);\n"
               "reflectionVector[i] = reflect(-normalizedLightDirection[i], normalizedTransformedNormals);\n"
               "lightSpecular[i] = ulightSpecular[i] * uMaterialSpecular * pow(max(dot(reflectionVector[i], normalizedViwerVector), 0.0),uMaterialShininess);\n"
               "PhongADSLight = PhongADSLight + lightAmbient[i] + lightDiffuse[i] + lightSpecular[i];\n "
               "}\n"
               "}\n"
               "else\n"
               "{\n"
               "PhongADSLight = vec3(1.0, 1.0, 1.0);\n"
               "}\n"
               "FragColor =  vec4 (PhongADSLight, 1.0);\n"
               "}";
       
       GLuint fragmentShaderObject_PF = glCreateShader(GL_FRAGMENT_SHADER);

       glShaderSource(fragmentShaderObject_PF, 1, (const GLchar**)&fragmentShaderSourceCode_PF, NULL);

       glCompileShader(fragmentShaderObject_PF);

       status = 0;
       infoLogLength = 0;
       szInfoLog = NULL;

       glGetShaderiv(fragmentShaderObject_PF, GL_COMPILE_STATUS, &status);

       if (status == GL_FALSE)
       {
           
           glGetShaderiv(fragmentShaderObject_PF, GL_INFO_LOG_LENGTH, &infoLogLength);
           
           if (infoLogLength > 0)
           {
               szInfoLog = (GLchar*)malloc(infoLogLength);
               
               
               if (szInfoLog != NULL)
               {
                   glGetShaderInfoLog(fragmentShaderObject_PF, infoLogLength, NULL, szInfoLog);
                   fprintf(gpFile, "Fragment shader compilation error log : %s\n", szInfoLog);
                   free(szInfoLog);
                   szInfoLog = NULL;
                   
                   
               }
               
               
               
           }
           
           [self uninitialize_Sphere];
           [self release];
           [NSApp terminate:self];
       }
       
       // shader program

       //step 7 : create shader program object

       shaderProgramObject_Sphere_PF = glCreateProgram();

       //step 8 : Attach both shaders into this program            also know as prelinking binding  should be done before linking and after attachment
       glAttachShader(shaderProgramObject_Sphere_PF, vertexShaderObject_PF);
       glAttachShader(shaderProgramObject_Sphere_PF, fragmentShaderObject_PF);

       //step 9: bind attribute location to the shader program object

       glBindAttribLocation(shaderProgramObject_Sphere_PF, AMC_ATTRIBUTE_POSITION, "aPosition");

       glBindAttribLocation(shaderProgramObject_Sphere_PF, AMC_ATTRIBUTE_NORMAL , "aNormal");

       //step 10: Link the shader program

       glLinkProgram(shaderProgramObject_Sphere_PF);

       //step 11: do linking error check fpr shader program

       status = 0;
       infoLogLength = 0;
       szInfoLog = NULL;

       glGetProgramiv(shaderProgramObject_Sphere_PF, GL_LINK_STATUS, &status);

       if (status == GL_FALSE)
       {
           glGetProgramiv(shaderProgramObject_Sphere_PF, GL_INFO_LOG_LENGTH, &infoLogLength);
           
           if (infoLogLength > 0)
           {
               szInfoLog = (GLchar*)malloc(infoLogLength);
               
               
               if (szInfoLog != NULL)
               {
                   glGetProgramInfoLog(shaderProgramObject_Sphere, infoLogLength, NULL, szInfoLog);
                   fprintf(gpFile, "shader program linking error log : %s\n", szInfoLog);
                   free(szInfoLog);
                   szInfoLog = NULL;
                   
                   
               }
               
               
               
           }
           
           [self uninitialize_Sphere];
           [self release];
           [NSApp terminate:self];
       }
       
       modelMatrixUniform_Sphere_PF = glGetUniformLocation(shaderProgramObject_Sphere_PF, "uModelMatrix");

       viewMatrixUniform_Sphere_PF = glGetUniformLocation(shaderProgramObject_Sphere_PF, "uViewMatrix");

       projectionMatrixUniform_Sphere_PF = glGetUniformLocation(shaderProgramObject_Sphere_PF, "uProjectionMatrix");

       lightAmbientUniform_Sphere_PF[0] = glGetUniformLocation(shaderProgramObject_Sphere_PF, "ulightAmbient[0]");

       lightDiffuseUniform_Sphere_PF[0] = glGetUniformLocation(shaderProgramObject_Sphere_PF, "ulightDiffuse[0]");

       lightSpecularUniform_Sphere_PF[0] = glGetUniformLocation(shaderProgramObject_Sphere_PF, "ulightSpecular[0]");

       lightPositionUniform_Sphere_PF[0] = glGetUniformLocation(shaderProgramObject_Sphere_PF, "ulightPosition[0]");

       lightAmbientUniform_Sphere_PF[1] = glGetUniformLocation(shaderProgramObject_Sphere_PF, "ulightAmbient[1]");

       lightDiffuseUniform_Sphere_PF[1] = glGetUniformLocation(shaderProgramObject_Sphere_PF, "ulightDiffuse[1]");

       lightSpecularUniform_Sphere_PF[1] = glGetUniformLocation(shaderProgramObject_Sphere_PF, "ulightSpecular[1]");

       lightPositionUniform_Sphere_PF[1] = glGetUniformLocation(shaderProgramObject_Sphere_PF, "ulightPosition[1]");
       
       lightAmbientUniform_Sphere_PF[2] = glGetUniformLocation(shaderProgramObject_Sphere_PF, "ulightAmbient[2]");

       lightDiffuseUniform_Sphere_PF[2] = glGetUniformLocation(shaderProgramObject_Sphere_PF, "ulightDiffuse[2]");

       lightSpecularUniform_Sphere_PF[2] = glGetUniformLocation(shaderProgramObject_Sphere_PF, "ulightSpecular[2]");

       lightPositionUniform_Sphere_PF[2] = glGetUniformLocation(shaderProgramObject_Sphere_PF, "ulightPosition[2]");


       materialAmbientUniform_Sphere_PF = glGetUniformLocation(shaderProgramObject_Sphere_PF, "uMaterialAmbient");

       materialDiffuseUniform_Sphere_PF = glGetUniformLocation(shaderProgramObject_Sphere_PF, "uMaterialDiffuse");


       materialSpecularUniform_Sphere_PF = glGetUniformLocation(shaderProgramObject_Sphere_PF, "uMaterialSpecular");

       materialShininessUniform_Sphere_PF = glGetUniformLocation(shaderProgramObject_Sphere_PF, "uMaterialShininess");
       
       keyPressUniform_Sphere_PF = glGetUniformLocation(shaderProgramObject_Sphere_PF, "uKeyPress");
           
       
       
       makeSphere(sphere, 1.4, 30, 30);
      
       
       

       // depth setting
       
       glClearDepth(1.0f); // Compulsory
       glEnable(GL_DEPTH_TEST); // 8 test paike ek(depth) test enable kr
       glDepthFunc(GL_LEQUAL);
       
       //clear color
       
       glClearColor(0.0f,0.0f,0.0f,1.0f);
       
       
           perspectiveProjectionMatrix_Sphere = vmath::mat4::identity();
           
           [self resize_Sphere:WIN_WIDTH:WIN_HEIGHT];
        
            //FBO related code
        
            
        
           
           return true;
       }


-(bool) createFBO:(GLint)textureWidth : (GLint)textureHeight
{
    //local variables
    GLint maxRenderBufferSize = 0;

    //check capacity of render buffer

    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderBufferSize);

    if (maxRenderBufferSize < textureWidth || maxRenderBufferSize < textureHeight)
    {
        fprintf(gpFile, "Texture size overflow\n");
        return(false);
    }

    //create custom frame buffer

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    //create texture for FBO in which we are going to render Scene 2 (Sphere)

    glGenTextures(1, &texture_FBO);
    glBindTexture(GL_TEXTURE_2D,texture_FBO);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);

    // Attach above texture to framebuffer at default color attachment 0

    //no need to create render buffer for color because second parameter will ensure to fill default color buffer of our newly created FBO

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_FBO, 0);

    //create render buffer for depth

    glGenRenderbuffers(1, &rbo);
    
    glBindRenderbuffer(GL_RENDERBUFFER,rbo);  //GL_DRAW_RENDERBUFFER also can be used

    //set the storage of above render buffer for texture size for depth

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, textureWidth, textureHeight);

    //attach above depth related render buffer to FBO

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    //check the frame buffer status weather successfull or not

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        fprintf(gpFile, "Framebuffer creation status is not complete\n\n");
        return(false);
    }

    //unbind with frame buffer

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return(true);
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
           [self uninitialize_Cube];
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



 -(void)resize_Cube:(int)width :(int)height
    {
        // code
        if(height <= 0)
            height = 1;


        winWidth = width;
        winHeight = height;

        glViewport(0, 0, (GLsizei)width, (GLsizei)height);

        //set Perspective projection matrix

        perspectiveProjectionMatrix_Cube = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    }

-(void)resize_Sphere:(int)width :(int)height
    {
        // code
        if(height <= 0)
            height = 1;


        //glMatrixMode(GL_PROJECTION); //not allowed in PP
        //glLoadIdentity();            not allowed in PP

        glViewport(0, 0, (GLsizei)width, (GLsizei)height);

        //set Perspective projection matrix

        perspectiveProjectionMatrix_Sphere = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    }
    -(void)display_Cube
    {
        //code
        // 3rd Change GL_DEPTH_BUFFER_BIT
        
        //void display_Sphere(GLint,GLint);
        //void update_Spher(void);
        //void resize_Sphere(int,int)
        
        //render FBO Scene
        
        if(bFBOResult == true)
        {
            [self display_Sphere:FBO_WIDTH:FBO_HEIGHT];
            
            if(bAnimationEnable == true)
            {
                [self myUpdate_Sphere];
            }
            
        }
        
        [self resize_Cube:WIN_WIDTH:WIN_HEIGHT];
        
        //reset the color to white to companset the change done by display sphere

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //PP steps for display function

        //step 1: Use shader program object

        glUseProgram(shaderProgramObject_Cube);
        //Cube

        //transformations

         mat4 modelViewMatrix = mat4::identity();

         mat4 translationMatrix = mat4::identity();

         translationMatrix = vmath::translate(0.0f, 0.0f, -4.0f);

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

         mat4 rotationMatrix = rotationMatrix1 * rotationMatrix2 * rotationMatrix3;

         modelViewMatrix = translationMatrix * scaleMatrix * rotationMatrix;

         

         //modelViewMatrix = mat4::identity();

        //mat4 modelViewMatrix = vmath::translate(0.0f, 0.0f, -3.0f);

        mat4 modelViewProjectionMatrix = mat4::identity();

        modelViewProjectionMatrix = perspectiveProjectionMatrix_Cube * modelViewMatrix;  //order of multiplication is not commutativ. i.e mat1 * mat2 != mat2 * mat1

        //push above mvp into vertex shader's mvp uniform

        glUniformMatrix4fv(mvpMatrixUniform_Cube, 1, GL_FALSE, modelViewProjectionMatrix);

        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, texture_FBO);

        glUniform1i(textureSamplerUniform_Cube, 0);

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

-(void)display_Sphere :(GLint)textureWidth :(GLint)textureHeight
{
    
    //bind with FBO
    
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);      //step 1: Use shader program object
    
    //call resize_Sphere()
    
    [self resize_Sphere:textureWidth:textureHeight];
    
    //set the clear color to back again to companset the change done by display_Cube()
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(shaderProgramObject_Sphere);        //transformations
    
    if(bEnableVertexLighting == true)
    {
        glUseProgram(shaderProgramObject_Sphere);
    }
    else if(bEnableFragmentLighting == true)
    {
        glUseProgram(shaderProgramObject_Sphere_PF);
    }
    
    
    mat4 modelViewMatrix = mat4::identity();
    mat4 viewMatrix = mat4::identity();
    mat4 translationMatrix = mat4::identity();
    
    
    
    // Set up translation matrix
    
    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
    
    // Combine translation and rotation
    modelViewMatrix = translationMatrix;
    
    ///push above mvp into vertex shader's mvp uniform
    
    // Pass modelViewMatrix to the vertex shader's uniform
    
    
    
    glUniformMatrix4fv(modelMatrixUniform_Sphere, 1, GL_FALSE, modelViewMatrix);
    
    // Pass viewMatrix to the vertex shader's uniform
    
    glUniformMatrix4fv(viewMatrixUniform_Sphere, 1, GL_FALSE, viewMatrix);
    
    // Pass perspectiveProjectionMatrix_Sphere to the vertex shader's uniform
    glUniformMatrix4fv(projectionMatrixUniform_Sphere, 1, GL_FALSE, perspectiveProjectionMatrix_Sphere);
    
    
    if(bEnableFragmentLighting == true)
    {
        glUniformMatrix4fv(modelMatrixUniform_Sphere_PF, 1, GL_FALSE, modelViewMatrix);
        
        // Pass viewMatrix to the vertex shader's uniform
        
        glUniformMatrix4fv(viewMatrixUniform_Sphere_PF, 1, GL_FALSE, viewMatrix);
        
        // Pass perspectiveProjectionMatrix_Sphere to the vertex shader's uniform
        glUniformMatrix4fv(projectionMatrixUniform_Sphere_PF, 1, GL_FALSE, perspectiveProjectionMatrix_Sphere);
    }
    
    
    
    
    if(bLightingEnable == true)
    {
        if(bEnableVertexLighting == false && bEnableFragmentLighting == false)
        {
            glUniform1i(keyPressUniform_Sphere, 1);
            
            glUniform3fv(lightAmbientUniform_Sphere[0], 1, light1Ambient);
            glUniform3fv(lightDiffuseUniform_Sphere[0], 1, light1Diffuse);
            glUniform3fv(lightSpecularUniform_Sphere[0], 1, light1Specular);
            glUniform4fv(lightPositionUniform_Sphere[0], 1, light1Position);
            
            glUniform3fv(lightAmbientUniform_Sphere[1], 1, light2Ambient);
            glUniform3fv(lightDiffuseUniform_Sphere[1], 1, light2Diffuse);
            glUniform3fv(lightSpecularUniform_Sphere[1], 1, light2Specular);
            glUniform4fv(lightPositionUniform_Sphere[1], 1, light2Position);
            
            glUniform3fv(lightAmbientUniform_Sphere[2], 1, light3Ambient);
            glUniform3fv(lightDiffuseUniform_Sphere[2], 1, light3Diffuse);
            glUniform3fv(lightSpecularUniform_Sphere[2], 1, light3Specular);
            glUniform4fv(lightPositionUniform_Sphere[2], 1, light3Position);
            
            glUniform3fv(materialAmbientUniform_Sphere, 1, materialAmbient);
            glUniform3fv(materialDiffuseUniform_Sphere, 1, materialDiffuse);
            glUniform3fv(materialSpecularUniform_Sphere, 1, materialSpecular);
            glUniform1f(materialShininessUniform_Sphere, materialShininess);
        }
        
        if(bEnableVertexLighting == true && bEnableFragmentLighting == false)
        {
            glUniform1i(keyPressUniform_Sphere, 1);
            
            glUniform3fv(lightAmbientUniform_Sphere[0], 1, light1Ambient);
            glUniform3fv(lightDiffuseUniform_Sphere[0], 1, light1Diffuse);
            glUniform3fv(lightSpecularUniform_Sphere[0], 1, light1Specular);
            glUniform4fv(lightPositionUniform_Sphere[0], 1, light1Position);
            
            glUniform3fv(lightAmbientUniform_Sphere[1], 1, light2Ambient);
            glUniform3fv(lightDiffuseUniform_Sphere[1], 1, light2Diffuse);
            glUniform3fv(lightSpecularUniform_Sphere[1], 1, light2Specular);
            glUniform4fv(lightPositionUniform_Sphere[1], 1, light2Position);
            
            glUniform3fv(lightAmbientUniform_Sphere[2], 1, light3Ambient);
            glUniform3fv(lightDiffuseUniform_Sphere[2], 1, light3Diffuse);
            glUniform3fv(lightSpecularUniform_Sphere[2], 1, light3Specular);
            glUniform4fv(lightPositionUniform_Sphere[2], 1, light3Position);
            
            glUniform3fv(materialAmbientUniform_Sphere, 1, materialAmbient);
            glUniform3fv(materialDiffuseUniform_Sphere, 1, materialDiffuse);
            glUniform3fv(materialSpecularUniform_Sphere, 1, materialSpecular);
            glUniform1f(materialShininessUniform_Sphere, materialShininess);
        }
        
        if(bEnableVertexLighting == false && bEnableFragmentLighting == true)
        {
            glUniform1i(keyPressUniform_Sphere_PF, 1);
            
            glUniform3fv(lightAmbientUniform_Sphere_PF[0], 1, light1Ambient_F);
            glUniform3fv(lightDiffuseUniform_Sphere_PF[0], 1, light1Diffuse_F);
            glUniform3fv(lightSpecularUniform_Sphere_PF[0], 1, light1Specular_F);
            glUniform4fv(lightPositionUniform_Sphere_PF[0], 1, light1Position_F);
            
            glUniform3fv(lightAmbientUniform_Sphere_PF[1], 1, light2Ambient_F);
            glUniform3fv(lightDiffuseUniform_Sphere_PF[1], 1, light2Diffuse_F);
            glUniform3fv(lightSpecularUniform_Sphere_PF[1], 1, light2Specular_F);
            glUniform4fv(lightPositionUniform_Sphere_PF[1], 1, light2Position_F);
            
            glUniform3fv(lightAmbientUniform_Sphere_PF[2], 1, light3Ambient_F);
            glUniform3fv(lightDiffuseUniform_Sphere_PF[2], 1, light3Diffuse_F);
            glUniform3fv(lightSpecularUniform_Sphere_PF[2], 1, light3Specular_F);
            glUniform4fv(lightPositionUniform_Sphere_PF[2], 1, light3Position_F);
            
            glUniform3fv(materialAmbientUniform_Sphere_PF, 1, materialAmbient);
            glUniform3fv(materialDiffuseUniform_Sphere_PF, 1, materialDiffuse);
            glUniform3fv(materialSpecularUniform_Sphere_PF, 1, materialSpecular);
            glUniform1f(materialShininessUniform_Sphere_PF, materialShininess);
        }
        
    }
    else
    {
        glUniform1i(keyPressUniform_Sphere, 0);
        glUniform1i(keyPressUniform_Sphere_PF,0);
    }
    
    sphere.draw();
    
    // step 5: Unuse shader program object
    glUseProgram(0);
    
    //unbind with the frame buffer
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

    -(void)myUpdate_Cube
    {
        AngleCube = AngleCube + 0.2f;

        if (AngleCube >= 360.0f)
        {
            AngleCube = 0.0f;
        }
    }

-(void)myUpdate_Sphere
    {
        
        if (bLightingEnable == true)
        {
            if(bEnableVertexLighting == true || bAnimationEnable == true)
            {
                if (vLight0 < (2 * M_PI))
                {
                    
                    
                    //light 0    //x rotation
                    light1Position[0] = 0.0f;
                    light1Position[1] = 5.0f * (GLfloat)sin(vLight0);
                    light1Position[2] = (-5.0f) + (5.0f * (GLfloat)cos(vLight0));
                    light1Position[3] = 1.0f;
                    
                    //fprintf(gpFile, "value of position[1] = %f", light1Position[1]);
                    
                    //light 1   //y rotation
                    
                    light2Position[0] = 5.0f * (GLfloat)sin(vLight1);
                    light2Position[1] = 0.0f;
                    light2Position[2] = (-5.0f) + (5.0f * (GLfloat)cos(vLight1));
                    light2Position[3] = 1.0f;
                    
                    //light 2   //Z rotation
                    
                    light3Position[0] = 5.0f * (GLfloat)sin(vLight2);
                    light3Position[1] = 5.0f * (GLfloat)cos(vLight2);
                    light3Position[2] = 0.0f;
                    light3Position[3] = 1.0f;
                    
                    vLight0 = vLight0 + 0.01f;
                    vLight1 = vLight1 + 0.01f;
                    vLight2 = vLight2 + 0.01f;
                }
                
                else
                {
                    vLight0 = 0.0f;
                    vLight1 = 0.0f;
                    vLight2 = 0.0f;
                }
            }
            
            if(bEnableFragmentLighting == true || bAnimationEnable == true)
            {
                
                
                if (fLight0 < (2 * M_PI))
                {
                    //light 0    //x rotation
                    light1Position_F[0] = 0.0f;
                    light1Position_F[1] = 5.0f * (GLfloat)sin(fLight0);
                    light1Position_F[2] = 5.0f * (GLfloat)cos(fLight0);
                    light1Position_F[3] = 1.0f;

                    //fprintf(gpFile, "value of position[1] = %f", light[0].position[1]);

                    //light 1   //y rotation

                    light2Position_F[0] = 5.0f * (GLfloat)sin(fLight1);
                    light2Position_F[1] = 0.0f;
                    light2Position_F[2] = 5.0f * (GLfloat)cos(fLight1);
                    light2Position_F[3] = 1.0f;

                    //light 2   //Z rotation

                    light3Position_F[0] = 5.0f * (GLfloat)sin(fLight2);
                    light3Position_F[1] = 5.0f * (GLfloat)cos(fLight2);
                    light3Position_F[2] = 0.0f;
                    light3Position_F[3] = 1.0f;

                    fLight0 = fLight0 + 0.01f;
                    fLight1 = fLight1 + 0.01f;
                    fLight2 = fLight2 + 0.01f;
                }

                else
                {
                    fLight0 = 0.0f;
                    fLight1 = 0.0f;
                    fLight2 = 0.0f;
                }


            }
            
           
        }
    }

    -(void)uninitialize_Cube
    {
        //code
        
        [self uninitialize_Sphere];

        if (shaderProgramObject_Cube)
        {
            //step 1 : use shader program object

            glUseProgram(shaderProgramObject_Cube);

            GLint numShaders = 0;

            //step 2 : get number of attached shaders
            glGetProgramiv(shaderProgramObject_Cube, GL_ATTACHED_SHADERS, &numShaders);



            if (numShaders > 0)
            {
                //step 3 : Create and allocate memory of shader objects according to number of attached shaders
                GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint));

                if (pShaders != NULL)
                {
                    //step 4 : Get shader object into the allocated array.

                    glGetAttachedShaders(shaderProgramObject_Cube, numShaders, NULL, pShaders);

                    //step 5 : Detach and delete shader objects iteratively

                    for (GLint i = 0; i < numShaders; i++)
                    {
                        glDetachShader(shaderProgramObject_Cube, pShaders[i]);
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

            glDeleteProgram(shaderProgramObject_Cube);
            shaderProgramObject_Cube = 0;

            
            if (texture_FBO)
            {
                glDeleteTextures(1, &texture_FBO);
                texture_FBO = 0;
            }

            if (rbo)
            {
                glDeleteRenderbuffers(1, &rbo);
                rbo = 0;
            }


            if (fbo)
            {
                glDeleteFramebuffers(1, &fbo);
                fbo = 0;
            }


        }
        
        //step 10 : delete vbo for texture

        if (vbo_TexCoord_Cube)
        {
            glDeleteBuffers(1, &vbo_TexCoord_Cube);
            vbo_TexCoord_Cube= 0;
        }


        //step 10 : delete vbo for position

        if (vbo_Position_Cube)
        {
            glDeleteBuffers(1, &vbo_Position_Cube);
            vbo_Position_Cube = 0;
        }

        //step 11 : delete VAO

        if (vao_Cube)
        {
            glDeleteVertexArrays(1, &vao_Cube);
            vao_Cube = 0;
        }

    }

-(void)uninitialize_Sphere
  {
      //code
      
      //steps for uninitialize_Sphere function in PP

      if (shaderProgramObject_Sphere)
      {
          //step 1 : use shader program object

          glUseProgram(shaderProgramObject_Sphere);

          GLint numShaders = 0;

          //step 2 : get number of attached shaders
          glGetProgramiv(shaderProgramObject_Sphere, GL_ATTACHED_SHADERS, &numShaders);



          if (numShaders > 0)
          {
              //step 3 : Create and allocate memory of shader objects according to number of attached shaders
              GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint));

              if (pShaders != NULL)
              {
                  //step 4 : Get shader object into the allocated array.

                  glGetAttachedShaders(shaderProgramObject_Sphere, numShaders, NULL, pShaders);

                  //step 5 : Detach and delete shader objects iteratively

                  for (GLint i = 0; i < numShaders; i++)
                  {
                      glDetachShader(shaderProgramObject_Sphere, pShaders[i]);
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

          glDeleteProgram(shaderProgramObject_Sphere);
          shaderProgramObject_Sphere = 0;

          
          



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
                [self uninitialize_Cube];
                [self release];
                [NSApp terminate:self];
                
                break;
                
            case 'e':
            case'E':
                [[self window]toggleFullScreen:self];
                break;
                
            case 'F':
            case'f':
                if(bEnableFragmentLighting == false)
                {
                    bEnableFragmentLighting = true;
                    bEnableVertexLighting = false;
                }
                break;
                           
            case 'V':
            case'v':
                if(bEnableVertexLighting == false)
                {
                    bEnableVertexLighting = true;
                    bEnableFragmentLighting = false;
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
        
        [self uninitialize_Cube];
        
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




    
