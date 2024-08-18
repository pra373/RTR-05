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
    AMC_ATTRIBUTE_TEXCOORD = 1,
    AMC_ATTRIBUTE_NORMAL = 2,
    AMC_ATTRIBUTE_COLOR = 3
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
        
        [window setTitle:@"Prathamesh Paropkari : Interleaved Program"];
        
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
    GLuint vbo_PCNT;
    

    GLfloat AngleCube = 0.0f;

    //uniforms

    GLuint textureSamplerUniform = 0;

    GLuint mvpMatrixUniform;

    



    mat4 perspectiveProjectionMatrix;

    // Uniforms

    GLuint modelMatrixUniform = 0;
    GLuint viewMatrixUniform = 0;
    GLuint projectionMatrixUniform = 0;

    GLuint lightDiffuseUniform = 0;
    GLuint lightAmbientUniform = 0;
    GLuint lightSpecularUniform = 0;
    GLuint lightPositionUniform = 0;

    GLuint materialDiffuseUniform = 0;
    GLuint materialAmbientUniform = 0;
    GLuint materialSpecularUniform = 0;
    GLuint materialShininessUniform = 0;


    GLuint keyPressUniform = 0;

    BOOL bLightingEnable = FALSE;

    BOOL bAnimationEnable = FALSE;


    // Light Variables

    GLfloat lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // white diffuse light
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightPosition[] = { 100.0f, 100.0f, 100.0f, 1.0f }; //

    GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat materialSpecular[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat materialShininess = 128.0f;



    // Texture Variable/Object
    GLuint marble_Texture = 0;



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
    
    if(bAnimationEnable == TRUE)
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
                "#version 410 core\n"
                "\n"
                "in vec4 aPosition;\n"
                "in vec3 aNormal;\n"
                "in vec4 aColor;\n"
                "in vec2 aTexCoord;\n"  // Added input for texture coordinates
                "out vec4 oColor;\n"
                "uniform mat4 uModelMatrix;\n"
                "uniform mat4 uViewMatrix;\n"
                "uniform mat4 uProjectionMatrix;\n"
                "uniform vec3 ulightAmbient;\n"
                "uniform vec3 ulightDiffuse;\n"
                "uniform vec3 ulightSpecular;\n"
                "uniform vec4 ulightPosition;\n"
                "uniform vec3 uMaterialAmbient;\n"
                "uniform vec3 uMaterialDiffuse;\n"
                "uniform vec3 uMaterialSpecular;\n"
                "uniform float uMaterialShininess;\n"
                "uniform int uKeyPress;\n"
                "out vec3 otransformedNormals;\n"
                "out vec3 olightDirection;\n"
                "out vec3 oviewerVector;\n"
                "out vec2 oTexCoord;\n"  // Output for texture coordinates
                "void main(void)\n"
                "{\n"
                "if(uKeyPress == 1)\n"
                "{\n"
                "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;\n"
                "otransformedNormals = (mat3(uViewMatrix * uModelMatrix) * aNormal);\n"
                "olightDirection = (vec3 (ulightPosition - eyeCoordinates));\n"
                "oviewerVector = (- eyeCoordinates.xyz);\n"
                "}\n"
                "else\n"
                "{\n"
                "otransformedNormals = vec3(0.0, 0.0, 0.0);\n"
                "olightDirection = vec3(0.0, 0.0, 0.0);\n"
                "oviewerVector = vec3(0.0, 0.0, 0.0);\n"
                "}\n"
                "    oColor = aColor;\n"
                "oTexCoord = aTexCoord;\n"  // Pass texture coordinates to the fragment shader
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
                "in vec3 otransformedNormals;\n"
                "in vec3 olightDirection;\n"
                "in vec3 oviewerVector;\n"
                "in vec2 oTexCoord;\n"  // Receive texture coordinates from vertex shader
                "uniform vec3 ulightAmbient;\n"
                "uniform vec3 ulightDiffuse;\n"
                "uniform vec3 ulightSpecular;\n"
                "uniform vec3 uMaterialAmbient;\n"
                "uniform vec3 uMaterialDiffuse;\n"
                "uniform vec3 uMaterialSpecular;\n"
                "uniform float uMaterialShininess;\n"
                "uniform int uKeyPress;\n"
                "uniform sampler2D uTextureSampler;\n"  // Texture sampler uniform
                "out vec4 FragColor;\n"
                "void main(void)\n"
                "{\n"
                "vec3 PhongADSLight;\n"
                "if(uKeyPress == 1)\n"
                "{\n"
                "vec3 normalizedTransformedNormals = normalize(otransformedNormals);\n"
                "vec3 normalizedLightDirection = normalize(olightDirection);\n"
                "vec3 normalizedViwerVector = normalize(oviewerVector);\n"
                "vec3 ambientLight = ulightAmbient * uMaterialAmbient;\n"
                "vec3 diffuseLight = ulightDiffuse * uMaterialDiffuse * max(dot(normalizedLightDirection, normalizedTransformedNormals), 0.0);\n"
                "vec3 reflectionVector = reflect(-normalizedLightDirection, normalizedTransformedNormals);\n"
                "vec3 specularLight = ulightSpecular * uMaterialSpecular * pow(max(dot(reflectionVector, normalizedViwerVector), 0.0), uMaterialShininess);\n"
                "PhongADSLight = ambientLight + diffuseLight + specularLight;\n"
                "}\n"
                "else\n"
                "{\n"
                "PhongADSLight = vec3(1.0, 1.0, 1.0);\n"
                "}\n"
                "vec3 tex = texture(uTextureSampler, oTexCoord).rgb;\n"
                "FragColor = vec4(tex * vec3(oColor) * PhongADSLight, 1.0);\n"
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
        
        glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_TEXCOORD, "aTexCoord");
        
        glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_NORMAL, "aNormal");
        
        
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

       //  mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "uMVPMatrix");  //must be done after linkage of shader program
        
       
        modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "uModelMatrix");



        viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "uViewMatrix");

        projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "uProjectionMatrix");


        lightAmbientUniform = glGetUniformLocation(shaderProgramObject, "ulightAmbient");


        lightDiffuseUniform = glGetUniformLocation(shaderProgramObject, "ulightDiffuse");

        lightSpecularUniform = glGetUniformLocation(shaderProgramObject, "ulightSpecular");


        lightPositionUniform = glGetUniformLocation(shaderProgramObject, "ulightPosition");

        materialAmbientUniform = glGetUniformLocation(shaderProgramObject, "uMaterialAmbient");

        materialDiffuseUniform = glGetUniformLocation(shaderProgramObject, "uMaterialDiffuse");


        materialSpecularUniform = glGetUniformLocation(shaderProgramObject, "uMaterialSpecular");

        materialShininessUniform = glGetUniformLocation(shaderProgramObject, "uMaterialShininess");

        keyPressUniform = glGetUniformLocation(shaderProgramObject, "uKeyPress");

        textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "uTextureSampler");
        
        
        if (textureSamplerUniform == -1)
        {
            fprintf(gpFile, "Loading of uniform variable failed !! Exiting now\n");
            return(-7);
        }
        
        //step 12: Declare position

       const GLfloat cube_PCNT[] =
        {
            // front
            // position                // color             // normals                // texcoords
             1.0f,  1.0f,  1.0f,    1.0f, 0.0f, 0.0f,     0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
            -1.0f,  1.0f,  1.0f,    1.0f, 0.0f, 0.0f,     0.0f,  0.0f,  1.0f,    0.0f, 1.0f,
            -1.0f, -1.0f,  1.0f,    1.0f, 0.0f, 0.0f,     0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
             1.0f, -1.0f,  1.0f,    1.0f, 0.0f, 0.0f,     0.0f,  0.0f,  1.0f,    1.0f, 0.0f,

             // right
             // position                // color             // normals                // texcoords
              1.0f,  1.0f, -1.0f,    0.0f, 0.0f, 1.0f,     1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
              1.0f,  1.0f,  1.0f,    0.0f, 0.0f, 1.0f,     1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
              1.0f, -1.0f,  1.0f,    0.0f, 0.0f, 1.0f,     1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
              1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 1.0f,     1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

              // back
              // position                // color             // normals                // texcoords
               1.0f,  1.0f, -1.0f,    1.0f, 1.0f, 0.0f,     0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
              -1.0f,  1.0f, -1.0f,    1.0f, 1.0f, 0.0f,     0.0f,  0.0f, -1.0f,    0.0f, 1.0f,
              -1.0f, -1.0f, -1.0f,    1.0f, 1.0f, 0.0f,     0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
               1.0f, -1.0f, -1.0f,    1.0f, 1.0f, 0.0f,     0.0f,  0.0f, -1.0f,    1.0f, 0.0f,

               // left
               // position                // color             // normals                // texcoords
               -1.0f,  1.0f,  1.0f,    1.0f, 0.0f, 1.0f,    -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
               -1.0f,  1.0f, -1.0f,    1.0f, 0.0f, 1.0f,    -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
               -1.0f, -1.0f, -1.0f,    1.0f, 0.0f, 1.0f,    -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
               -1.0f, -1.0f,  1.0f,    1.0f, 0.0f, 1.0f,    -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

               // top
               // position                // color             // normals                // texcoords
                1.0f,  1.0f, -1.0f,    0.0f, 1.0f, 0.0f,     0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
               -1.0f,  1.0f, -1.0f,    0.0f, 1.0f, 0.0f,     0.0f,  1.0f,  0.0f,    0.0f, 1.0f,
               -1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 0.0f,     0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
                1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 0.0f,     0.0f,  1.0f,  0.0f,    1.0f, 0.0f,

                // bottom
                // position                // color             // normals                // texcoords
                 1.0f, -1.0f,  1.0f,    1.0f, 0.5f, 0.0f,     0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
                -1.0f, -1.0f,  1.0f,    1.0f, 0.5f, 0.0f,     0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
                -1.0f, -1.0f, -1.0f,    1.0f, 0.5f, 0.0f,     0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
                 1.0f, -1.0f, -1.0f,    1.0f, 0.5f, 0.0f,     0.0f, -1.0f,  0.0f,    1.0f, 0.0f
        };


        ////step 13: Create vertex array object
        //VAO

        // ********************************************** for cube ***********************************************

        
       // Generate VAO for the cube
        glGenVertexArrays(1, &vao);

        //step 14: Bind with VAO

        glBindVertexArray(vao);

        //step 15: Create vertex buffer object (VBO) for position

        //VBO for position

        glGenBuffers(1, &vbo_PCNT);

        //step 16: Bind with VBO of position

        glBindBuffer(GL_ARRAY_BUFFER, vbo_PCNT);

        glBufferData(GL_ARRAY_BUFFER, 24*11*sizeof(float), cube_PCNT, GL_STATIC_DRAW); // 24*11*sizeof(float) is nothing but size of cube_PCNT

        //****************** Position *****************************

        glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(0*sizeof(float)));

        glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);


        //******************** Color ********************************

        glVertexAttribPointer(AMC_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));

        glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR);

        //******************* Normals ************************

        glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));

        glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);

        //***************** Texcoord *****************************
        glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));

        glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);

        // unbind of vbo

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
        
        marble_Texture = [self loadGLTexture:"marble.bmp"];
        
        if (marble_Texture == 0)
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
        //code
        // 3rd Change GL_DEPTH_BUFFER_BIT
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //PP steps for display function

        //step 1: Use shader program object

        glUseProgram(shaderProgramObject);




        // Cube

          //transformations

        vmath::mat4 modelMatrix = vmath::mat4::identity();
        vmath::mat4 viewMatrix = vmath::mat4::identity();

        vmath::mat4 translationMatrix = vmath::mat4::identity();


        translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

        vmath::mat4 scaleMatrix = vmath::mat4::identity();

        scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

        vmath::mat4 rotationMatrix1 = vmath::mat4::identity();

        rotationMatrix1 = vmath::rotate(AngleCube, 1.0f, 0.0f, 0.0f);

        vmath::mat4 rotationMatrix2 = vmath::mat4::identity();

        rotationMatrix2 = vmath::rotate(AngleCube, 0.0f, 1.0f, 0.0f);

        vmath::mat4 rotationMatrix3 = vmath::mat4::identity();

        rotationMatrix3 = vmath::rotate(AngleCube, 0.0f, 0.0f, 1.0f);

        vmath::mat4 rotationMatrix = rotationMatrix1 * rotationMatrix2 * rotationMatrix3;

        modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;





        //push above mvp into vertex shader's mvp uniform

        glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


        glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


        glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

        if (bLightingEnable == TRUE)
        {
            glUniform1i(keyPressUniform, 1);

            glUniform3fv(lightAmbientUniform, 1, lightAmbient);

            glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);

            glUniform3fv(lightSpecularUniform, 1, lightSpecular);

            glUniform4fv(lightPositionUniform, 1, lightPosition);

            glUniform3fv(materialAmbientUniform, 1, materialAmbient);

            glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

            glUniform3fv(materialSpecularUniform, 1, materialSpecular);

            glUniform1f(materialShininessUniform, materialShininess);

        }
        else
        {
            glUniform1i(keyPressUniform, 0);
        }


        // For texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, marble_Texture);
        glUniform1i(textureSamplerUniform, 0);


        // Bind with (VAO)
        glBindVertexArray(vao);

        // Draw the geometry
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
        glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
        glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
        glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
        glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

        // Unbind with (VAO)
        glBindVertexArray(0);

        //step 5: Unuse shader program object
        glUseProgram(0);
       


    }

    -(void)myUpdate
    {
        AngleCube = AngleCube + 0.2f;

        if (AngleCube >= 360.0f)
        {
            AngleCube = 0.0f;
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
        
        //step 10 : delete vbo for texture

        


        //step 10 : delete vbo for position

        if (vbo_PCNT)
        {
            glDeleteBuffers(1, &vbo_PCNT);
            vbo_PCNT = 0;
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
                
            case 'L':
            case 'l':
                if (bLightingEnable == FALSE)
                {
                    bLightingEnable = TRUE;
                }
                else
                {
                    bLightingEnable = FALSE;
                }
                break;
            case 'A':
            case 'a':
                if (bAnimationEnable == FALSE)
                {
                    bAnimationEnable = TRUE;
                }
                else
                {
                    bAnimationEnable = FALSE;
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




    
