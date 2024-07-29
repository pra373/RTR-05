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

#import "sphere.h"

using namespace vmath;

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
//global function declaration

CVReturn MyDisplayLinkCallback(CVDisplayLinkRef,const CVTimeStamp *,const CVTimeStamp *,CVOptionFlags,CVOptionFlags *,void *);

//global variable declarations

FILE *gpFile = NULL;

/*enum
{
    AMC_ATTRIBUTE_POSITION = 0,
    AMC_ATTRIBUTE_COLOR = 1,
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
        
        [window setTitle:@"Prathamesh Paropkari : Three Moving Lights on Sphere"];
        
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
    GLuint shaderProgramObject_PF;

    mesh sphere;

    GLuint mvpMatrixUniform;

    mat4 perspectiveProjectionMatrix;

    // Uniforms for per vertex

    GLuint modelMatrixUniform = 0;
    GLuint viewMatrixUniform = 0;
    GLuint projectionMatrixUniform = 0;

    GLuint lightDiffuseUniform[3];
    GLuint lightAmbientUniform[3];
    GLuint lightSpecularUniform[3];
    GLuint lightPositionUniform[3];

    GLuint materialDiffuseUniform = 0;
    GLuint materialAmbientUniform = 0;
    GLuint materialSpecularUniform = 0;
    GLuint materialShininessUniform = 0;

    GLuint keyPressUniform = 0;

    // Uniforms for per vertex

    GLuint modelMatrixUniform_PF = 0;
    GLuint viewMatrixUniform_PF = 0;
    GLuint projectionMatrixUniform_PF = 0;

    GLuint lightDiffuseUniform_PF[3];
    GLuint lightAmbientUniform_PF[3];
    GLuint lightSpecularUniform_PF[3];
    GLuint lightPositionUniform_PF[3];

    GLuint materialDiffuseUniform_PF = 0;
    GLuint materialAmbientUniform_PF = 0;
    GLuint materialSpecularUniform_PF = 0;
    GLuint materialShininessUniform_PF = 0;

    GLuint keyPressUniform_PF = 0;

    bool bLightingEnable = false;

    bool bAnimationEnable = false;

    bool bEnableVertexLighting = false;
    bool bEnableFragmentLighting = false;

    
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
    GLfloat fLight2 = 0.0f;    /*struct Light
    {
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        vec4 position;
    };

    struct Light light[2];*/

    
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
    
    lightAmbientUniform[2] = glGetUniformLocation(shaderProgramObject, "ulightAmbient[2]");
    
    lightDiffuseUniform[2] = glGetUniformLocation(shaderProgramObject, "ulightDiffuse[2]");
    
    lightSpecularUniform[2] = glGetUniformLocation(shaderProgramObject, "ulightSpecular[2]");
    
    lightPositionUniform[2] = glGetUniformLocation(shaderProgramObject, "ulightPosition[2]");
    
    
    materialAmbientUniform = glGetUniformLocation(shaderProgramObject, "uMaterialAmbient");
    
    materialDiffuseUniform = glGetUniformLocation(shaderProgramObject, "uMaterialDiffuse");
    
    
    materialSpecularUniform = glGetUniformLocation(shaderProgramObject, "uMaterialSpecular");
    
    materialShininessUniform = glGetUniformLocation(shaderProgramObject, "uMaterialShininess");
    
    keyPressUniform = glGetUniformLocation(shaderProgramObject, "uKeyPress");
    
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
                
                
                //step 5 (g) : uninitialize and exit.
                
                
            }
            
        }
        
        [self uninitialize];
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
        
        [self uninitialize];
        [self release];
        [NSApp terminate:self];
    }
    
    // shader program

    //step 7 : create shader program object

    shaderProgramObject_PF = glCreateProgram();

    //step 8 : Attach both shaders into this program            also know as prelinking binding  should be done before linking and after attachment
    glAttachShader(shaderProgramObject_PF, vertexShaderObject_PF);
    glAttachShader(shaderProgramObject_PF, fragmentShaderObject_PF);

    //step 9: bind attribute location to the shader program object

    glBindAttribLocation(shaderProgramObject_PF, AMC_ATTRIBUTE_POSITION, "aPosition");

    glBindAttribLocation(shaderProgramObject_PF, AMC_ATTRIBUTE_NORMAL , "aNormal");

    //step 10: Link the shader program

    glLinkProgram(shaderProgramObject_PF);

    //step 11: do linking error check fpr shader program

    status = 0;
    infoLogLength = 0;
    szInfoLog = NULL;

    glGetProgramiv(shaderProgramObject_PF, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObject_PF, GL_INFO_LOG_LENGTH, &infoLogLength);
        
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
    
    modelMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "uModelMatrix");

    viewMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "uViewMatrix");

    projectionMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "uProjectionMatrix");

    lightAmbientUniform_PF[0] = glGetUniformLocation(shaderProgramObject_PF, "ulightAmbient[0]");

    lightDiffuseUniform_PF[0] = glGetUniformLocation(shaderProgramObject_PF, "ulightDiffuse[0]");

    lightSpecularUniform_PF[0] = glGetUniformLocation(shaderProgramObject_PF, "ulightSpecular[0]");

    lightPositionUniform_PF[0] = glGetUniformLocation(shaderProgramObject_PF, "ulightPosition[0]");

    lightAmbientUniform_PF[1] = glGetUniformLocation(shaderProgramObject_PF, "ulightAmbient[1]");

    lightDiffuseUniform_PF[1] = glGetUniformLocation(shaderProgramObject_PF, "ulightDiffuse[1]");

    lightSpecularUniform_PF[1] = glGetUniformLocation(shaderProgramObject_PF, "ulightSpecular[1]");

    lightPositionUniform_PF[1] = glGetUniformLocation(shaderProgramObject_PF, "ulightPosition[1]");
    
    lightAmbientUniform_PF[2] = glGetUniformLocation(shaderProgramObject_PF, "ulightAmbient[2]");

    lightDiffuseUniform_PF[2] = glGetUniformLocation(shaderProgramObject_PF, "ulightDiffuse[2]");

    lightSpecularUniform_PF[2] = glGetUniformLocation(shaderProgramObject_PF, "ulightSpecular[2]");

    lightPositionUniform_PF[2] = glGetUniformLocation(shaderProgramObject_PF, "ulightPosition[2]");


    materialAmbientUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "uMaterialAmbient");

    materialDiffuseUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "uMaterialDiffuse");


    materialSpecularUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "uMaterialSpecular");

    materialShininessUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "uMaterialShininess");
    
    keyPressUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "uKeyPress");
        
    
    
    makeSphere(sphere, 1.4, 30, 30);
   
    
    

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
        //step 1: Use shader program object
        
        

         glUseProgram(shaderProgramObject);        //transformations
        
        if(bEnableVertexLighting == true)
        {
            glUseProgram(shaderProgramObject);
        }
        else if(bEnableFragmentLighting == true)
        {
            glUseProgram(shaderProgramObject_PF);
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
        
       
        
        glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelViewMatrix);

            // Pass viewMatrix to the vertex shader's uniform
          
        glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

            // Pass perspectiveProjectionMatrix to the vertex shader's uniform
        glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
        
        
        if(bEnableFragmentLighting == true)
        {
            glUniformMatrix4fv(modelMatrixUniform_PF, 1, GL_FALSE, modelViewMatrix);

            // Pass viewMatrix to the vertex shader's uniform
          
            glUniformMatrix4fv(viewMatrixUniform_PF, 1, GL_FALSE, viewMatrix);

            // Pass perspectiveProjectionMatrix to the vertex shader's uniform
            glUniformMatrix4fv(projectionMatrixUniform_PF, 1, GL_FALSE, perspectiveProjectionMatrix);
        }
        
         

          
        if(bLightingEnable == true)
        {
            if(bEnableVertexLighting == false && bEnableFragmentLighting == false)
            {
                glUniform1i(keyPressUniform, 1);
                
                glUniform3fv(lightAmbientUniform[0], 1, light1Ambient);
                glUniform3fv(lightDiffuseUniform[0], 1, light1Diffuse);
                glUniform3fv(lightSpecularUniform[0], 1, light1Specular);
                glUniform4fv(lightPositionUniform[0], 1, light1Position);
                
                glUniform3fv(lightAmbientUniform[1], 1, light2Ambient);
                glUniform3fv(lightDiffuseUniform[1], 1, light2Diffuse);
                glUniform3fv(lightSpecularUniform[1], 1, light2Specular);
                glUniform4fv(lightPositionUniform[1], 1, light2Position);
                
                glUniform3fv(lightAmbientUniform[2], 1, light3Ambient);
                glUniform3fv(lightDiffuseUniform[2], 1, light3Diffuse);
                glUniform3fv(lightSpecularUniform[2], 1, light3Specular);
                glUniform4fv(lightPositionUniform[2], 1, light3Position);
                
                glUniform3fv(materialAmbientUniform, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform, 1, materialSpecular);
                glUniform1f(materialShininessUniform, materialShininess);
            }
            
            if(bEnableVertexLighting == true && bEnableFragmentLighting == false)
            {
                glUniform1i(keyPressUniform, 1);
                
                glUniform3fv(lightAmbientUniform[0], 1, light1Ambient);
                glUniform3fv(lightDiffuseUniform[0], 1, light1Diffuse);
                glUniform3fv(lightSpecularUniform[0], 1, light1Specular);
                glUniform4fv(lightPositionUniform[0], 1, light1Position);
                
                glUniform3fv(lightAmbientUniform[1], 1, light2Ambient);
                glUniform3fv(lightDiffuseUniform[1], 1, light2Diffuse);
                glUniform3fv(lightSpecularUniform[1], 1, light2Specular);
                glUniform4fv(lightPositionUniform[1], 1, light2Position);
                
                glUniform3fv(lightAmbientUniform[2], 1, light3Ambient);
                glUniform3fv(lightDiffuseUniform[2], 1, light3Diffuse);
                glUniform3fv(lightSpecularUniform[2], 1, light3Specular);
                glUniform4fv(lightPositionUniform[2], 1, light3Position);
                
                glUniform3fv(materialAmbientUniform, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform, 1, materialSpecular);
                glUniform1f(materialShininessUniform, materialShininess);
            }
            
            if(bEnableVertexLighting == false && bEnableFragmentLighting == true)
            {
                glUniform1i(keyPressUniform_PF, 1);
                
                glUniform3fv(lightAmbientUniform_PF[0], 1, light1Ambient_F);
                glUniform3fv(lightDiffuseUniform_PF[0], 1, light1Diffuse_F);
                glUniform3fv(lightSpecularUniform_PF[0], 1, light1Specular_F);
                glUniform4fv(lightPositionUniform_PF[0], 1, light1Position_F);
                
                glUniform3fv(lightAmbientUniform_PF[1], 1, light2Ambient_F);
                glUniform3fv(lightDiffuseUniform_PF[1], 1, light2Diffuse_F);
                glUniform3fv(lightSpecularUniform_PF[1], 1, light2Specular_F);
                glUniform4fv(lightPositionUniform_PF[1], 1, light2Position_F);
                
                glUniform3fv(lightAmbientUniform_PF[2], 1, light3Ambient_F);
                glUniform3fv(lightDiffuseUniform_PF[2], 1, light3Diffuse_F);
                glUniform3fv(lightSpecularUniform_PF[2], 1, light3Specular_F);
                glUniform4fv(lightPositionUniform_PF[2], 1, light3Position_F);
                
                glUniform3fv(materialAmbientUniform_PF, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform_PF, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform_PF, 1, materialSpecular);
                glUniform1f(materialShininessUniform_PF, materialShininess);
            }
           
        }
        else
        {
            glUniform1i(keyPressUniform, 0);
            glUniform1i(keyPressUniform_PF,0);
        }

        sphere.draw();

        // step 5: Unuse shader program object
        glUseProgram(0);
}


       
             
         

    -(void)myUpdate
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
            
            if(bEnableFragmentLighting == true)
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




    
