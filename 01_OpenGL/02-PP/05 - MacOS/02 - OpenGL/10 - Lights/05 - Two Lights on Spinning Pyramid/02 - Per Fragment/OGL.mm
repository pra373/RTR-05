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
        
        [window setTitle:@"Prathamesh Paropkar : Two Static Light On Pyramid Per Fragment Lighting"];
        
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
    

    GLuint vbo_Position_Pyramid = 0;
    GLuint vbo_Color_Pyramid = 0;
    GLuint vbo_Normal = 0;
    

    GLuint mvpMatrixUniform;

    //rotation variable

    GLfloat AnglePyramid = 0.0f;

   
    mat4 perspectiveProjectionMatrix;

    // Uniforms

    GLuint modelMatrixUniform = 0;
    GLuint viewMatrixUniform = 0;
    GLuint projectionMatrixUniform = 0;

    GLuint lightDiffuseUniform[2];
    GLuint lightAmbientUniform[2];
    GLuint lightSpecularUniform[2];
    GLuint lightPositionUniform[2];

    GLuint materialDiffuseUniform = 0;
    GLuint materialAmbientUniform = 0;
    GLuint materialSpecularUniform = 0;
    GLuint materialShininessUniform = 0;

    GLuint keyPressUniform = 0;

    bool bLightingEnable = false;

    bool bAnimationEnable = false;

    
    // Light Variables

    GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // white diffuse light
    GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };

    GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat materialShininess = 128.0f;

   

    struct Light
    {
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        vec4 position;
    };

    struct Light light[2];

    
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
            "uniform mat4 uModelMatrix;\n"
            "uniform mat4 uViewMatrix;\n"
            "uniform mat4 uProjectionMatrix;\n"
            "uniform vec4 ulightPosition[2];\n"
            "uniform int uKeyPress;\n"
            "out vec3 otransformedNormals;\n"
            "out vec3 olightDirection[2];\n"
            "out vec3 oviewerVector;\n"
            "void main(void)\n"
            "{\n"
            "if(uKeyPress == 1)\n"
            "{\n"
            "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;\n"
            "otransformedNormals = (mat3(uViewMatrix * uModelMatrix) * aNormal);\n"
            "oviewerVector = -eyeCoordinates.xyz;\n"
            "for(int i = 0; i < 2; i++)\n"
            "{\n"
            "olightDirection[i] = vec3(ulightPosition[i] - eyeCoordinates);\n"
            "}\n"
            "}\n"
            "else\n"
            "{\n"
            "for(int i = 0; i < 2; i++)\n"
            "{\n"
            "olightDirection[i] = vec3(0.0, 0.0, 0.0);\n"
            "}\n"
            "otransformedNormals =  vec3(0.0, 0.0, 0.0);\n"
            "oviewerVector = vec3(0.0, 0.0, 0.0);\n"
            "}\n"
            "gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;\n"
            "}\n";
    
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
            "in vec3 PhongADSLight;\n"
            "in vec3 otransformedNormals;\n"
            "in vec3 olightDirection[2];\n"
            "in vec3 oviewerVector;\n"
            "uniform mat4 uModelMatrix;\n"
            "uniform mat4 uViewMatrix;\n"
            "uniform mat4 uProjectionMatrix;\n"
            "uniform vec3 ulightAmbient[2];\n"
            "uniform vec3 ulightDiffuse[2];\n"
            "uniform vec3 ulightSpecular[2];\n"
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
            "vec3 normalizedLightDirection[2];\n"
            "vec3 normalizedTransformedNormals = normalize(otransformedNormals);\n"
            "vec3 normalizedViwerVector = normalize(oviewerVector);\n"
            "vec3 lightAmbient[2];\n"
            "vec3 lightDiffuse[2];\n"
            "vec3 reflectionVector[2];\n"
            "vec3 lightSpecular[2];\n"
            "for(int i = 0; i < 2; i++)\n"
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
    
    // mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "uMVPMatrix");  //must be done after linkage of shader program
    
    modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "uModelMatrix");

        viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "uViewMatrix");

        projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "uProjectionMatrix");

        lightAmbientUniform[0] = glGetUniformLocation(shaderProgramObject, "ulightAmbient[0]");

        lightDiffuseUniform[0] = glGetUniformLocation(shaderProgramObject, "ulightDiffuse[0]");

        lightSpecularUniform[0] = glGetUniformLocation(shaderProgramObject, "ulightSpecular[0]");

        lightPositionUniform[0] = glGetUniformLocation(shaderProgramObject, "ulightPosition[0]");

        lightAmbientUniform[1] = glGetUniformLocation(shaderProgramObject, "ulightAmbient[1]");

        lightDiffuseUniform[1] = glGetUniformLocation(shaderProgramObject, "ulightDiffuse[1]");

        lightSpecularUniform[1] = glGetUniformLocation(shaderProgramObject, "ulightSpecular[1]");

        lightPositionUniform[1] = glGetUniformLocation(shaderProgramObject, "ulightPosition[1]");


        materialAmbientUniform = glGetUniformLocation(shaderProgramObject, "uMaterialAmbient");

        materialDiffuseUniform = glGetUniformLocation(shaderProgramObject, "uMaterialDiffuse");


        materialSpecularUniform = glGetUniformLocation(shaderProgramObject, "uMaterialSpecular");

        materialShininessUniform = glGetUniformLocation(shaderProgramObject, "uMaterialShininess");
    
    keyPressUniform = glGetUniformLocation(shaderProgramObject, "uKeyPress");

    const GLfloat pyramidPosition[] = {
        
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
    
    // normals
    const  GLfloat pyramidNormals[] =
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
        -0.894427f, 0.447214f,  0.000000f // left-right
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
    
    // VBO for Normals ***********************************
    
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
    
    // initializing struct
    light[0].ambient = vec3(0.0, 0.0, 0.0);
    light[1].ambient = vec3(0.0, 0.0, 0.0);
    light[0].diffuse = vec3(1.0, 1.0, 0.0);
    light[1].diffuse = vec3(0.0, 1.0, 0.0);
    light[0].specular = vec3(1.0, 1.0, 0.0);
    light[1].specular = vec3(0.0, 1.0, 0.0);
    light[0].position = vec4(-2.0, 0.0, 0.0, 1.0);
    light[1].position = vec4(2.0, 0.0, 1.0, 1.0);
    
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
          mat4 viewMatrix = mat4::identity();
          mat4 translationMatrix = mat4::identity();
          mat4 rotationMatrix = mat4::identity();

          // Set up rotation matrix
          rotationMatrix = vmath::rotate(AnglePyramid, 0.0f, 1.0f, 0.0f);

          // Set up translation matrix
          translationMatrix = vmath::translate(0.0f, 0.0f, -4.0f);

          // Combine translation and rotation
          modelViewMatrix = translationMatrix * rotationMatrix;

          ///push above mvp into vertex shader's mvp uniform

          // Pass modelViewMatrix to the vertex shader's uniform
          glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelViewMatrix);

          // Pass viewMatrix to the vertex shader's uniform
          glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

          // Pass perspectiveProjectionMatrix to the vertex shader's uniform
          glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

          

        if (bLightingEnable == true)
            {
                glUniform1i(keyPressUniform, 1);

                glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);

                glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);

                glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);

                glUniform4fv(lightPositionUniform[0], 1, light[0].position);

                glUniform3fv(lightAmbientUniform[1], 1, light[1].ambient);

                glUniform3fv(lightDiffuseUniform[1], 1, light[1].diffuse);

                glUniform3fv(lightSpecularUniform[1], 1, light[1].specular);

                glUniform4fv(lightPositionUniform[1], 1, light[1].position);

                glUniform3fv(materialAmbientUniform, 1, materialAmbient);

                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);

                glUniform3fv(materialSpecularUniform, 1, materialSpecular);

                glUniform1f(materialShininessUniform, materialShininess);

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




    
