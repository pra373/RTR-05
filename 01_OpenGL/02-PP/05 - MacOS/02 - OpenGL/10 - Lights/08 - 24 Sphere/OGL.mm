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

#import "vmath.h"

#import "sphere.h"

using namespace vmath;

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
// global function declaration

CVReturn MyDisplayLinkCallback(CVDisplayLinkRef, const CVTimeStamp *,
                               const CVTimeStamp *, CVOptionFlags,
                               CVOptionFlags *, void *);

// global variable declarations

FILE *gpFile = NULL;

// OpenGL related global variables

@interface AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>

@end

@interface GLview : NSOpenGLView
@end

// main function

int main(int argc, char *argv[]) {
  // code
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

  NSApp = [NSApplication sharedApplication];

  [NSApp setDelegate:[[AppDelegate alloc] init]];

  [NSApp run];

  [pool release];

  return 0;
}

// implementation of custom AppDelegate interface

@implementation AppDelegate

{

  NSWindow *window;

  GLview *glView;
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification {
  // code

  // log file creation code

  NSBundle *appBundle = [NSBundle mainBundle];

  NSString *appDirPath = [appBundle bundlePath];

  NSString *parentDirPath = [appDirPath stringByDeletingLastPathComponent];

  NSString *logFileNameWithPath =
      [NSString stringWithFormat:@"%@/Log.txt", parentDirPath];

  const char *pszLogFileNameWithPath =
      [logFileNameWithPath cStringUsingEncoding:NSASCIIStringEncoding];

  gpFile = fopen(pszLogFileNameWithPath, "w");

  if (gpFile == NULL) {
    NSLog(@"Log File Cannot Be Created");

    [self release];

    [NSApp terminate:self];
  }

  fprintf(gpFile, "Program Started Successfully\n");

  // Painting window background black
  [window setBackgroundColor:[NSColor blackColor]];

  // declare rectangle for frame of our window

  NSRect win_rect = NSMakeRect(0.0, 0.0, WIN_WIDTH, WIN_HEIGHT);

  // create the window

  window = [[NSWindow alloc]
      initWithContentRect:win_rect
                styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable |
                          NSWindowStyleMaskMiniaturizable |
                          NSWindowStyleMaskResizable
                  backing:NSBackingStoreBuffered
                    defer:NO];

  // give title to the window

  [window setTitle:@"Prathamesh Paropkari : 24 Spheres"];

  // centre the window

  [window center];

  // create the custom view

  glView = [[GLview alloc] initWithFrame:win_rect];

  // set this newly created custom as view of our custom Window

  [window setContentView:glView];

  [window setDelegate:self];

  // now actually show the window, give it keyboard focus and make it top in the
  // z order

  [window makeKeyAndOrderFront:self];
}

- (void)applicationWillTerminate:(NSNotification *)notification {
  // log file closing code

  if (gpFile) {
    fprintf(gpFile, "Program Finished Successfully");

    fclose(gpFile);

    gpFile = NULL;
  }
}

- (void)windowWillClose:(NSNotification *)notification {
  // code

  [NSApp terminate:self];
}

- (void)dealloc {
  [super dealloc];

  [glView release];

  [window release];
}

@end

// implementation of custome view interface

@implementation GLview

CVDisplayLinkRef displayLink;

// OpenGL Related Field variables

GLuint shaderProgramObject;
GLuint mvpMatrixUniform;
mat4 perspectiveProjectionMatrix;
mesh sphere;

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

    GLfloat WindowWidth;
    GLfloat WindowHeight;

GLuint keyPressUniform = 0;

bool bLightingEnable = false;

int keyPressed = 0;

//light rotation variables

GLfloat angleForXRotation = 0.0f;
GLfloat angleForYRotation = 0.0f;
GLfloat angleForZRotation = 0.0f;

// Light Variables


GLfloat lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // white diffuse light
GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f }; //


GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat materialDiffuse[] = { 0.5f, 0.2f, 0.7f, 1.0f };
GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialShininess = 128.0f;


- (id)initWithFrame:(NSRect)frame {
  // code

  self = [super initWithFrame:frame];

  if (self) {
    // steps for code in view constructor

    // step 1: Declare OpenGL Attributes

    NSOpenGLPixelFormatAttribute attributes[] = {

        NSOpenGLPFAOpenGLProfile,
        NSOpenGLProfileVersion4_1Core,
        NSOpenGLPFAScreenMask,
        CGDisplayIDToOpenGLDisplayMask(kCGDirectMainDisplay),
        NSOpenGLPFAColorSize,
        24,
        NSOpenGLPFADepthSize,
        32,
        NSOpenGLPFAAlphaSize,
        8,
        NSOpenGLPFANoRecovery,
        NSOpenGLPFAAccelerated,
        NSOpenGLPFADoubleBuffer,
        0

    };

    // step2 : Create OpenGL Pixel Format using above attributes

    NSOpenGLPixelFormat *pixelFormat = [[[NSOpenGLPixelFormat alloc]
        initWithAttributes:attributes] autorelease];

    if (pixelFormat == nil) {
      fprintf(gpFile, "OpenGL Pixel Format Failed");
      [self uninitialize];
      [self release];
      [NSApp terminate:self];
    }
    // step3: Create OpenGL Context using above Pixel Format

    NSOpenGLContext *glContext =
        [[[NSOpenGLContext alloc] initWithFormat:pixelFormat
                                    shareContext:nil] autorelease];

    if (glContext == nil) {
      fprintf(gpFile, "glContext Creation Failed");
      [self uninitialize];
      [self release];
      [NSApp terminate:self];
    }
    // step4: Set Pixel Format

    [self setPixelFormat:pixelFormat];

    // step 5:Set OpenGL Context

    [self setOpenGLContext:glContext];
  }

  return (self);
}

- (void)prepareOpenGL {
  [super prepareOpenGL];

  [[self openGLContext] makeCurrentContext];

  // matching monitor retracing with double buffer swaping

  GLint swapInterval = 1;

  [[self openGLContext] setValues:&swapInterval
                     forParameter:NSOpenGLCPSwapInterval];

  int result = [self initialize];

  // Create and start display link

  // steps of creating display link

  // step 1: Create Display Link According to the current active display

  CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);

  // step 2: set the call back method for OpenGL Thread

  CVDisplayLinkSetOutputCallback(displayLink, &MyDisplayLinkCallback, self);

  // step 3:Convert NSOpenGLPixelFormat to CGL pixel format.

  CGLPixelFormatObj cglPixelFormat =
      (CGLPixelFormatObj)[[self pixelFormat] CGLPixelFormatObj];

  // step 4:convert NSOpenGL Context into CGL Context

  CGLContextObj cglContext =
      (CGLContextObj)[[self openGLContext] CGLContextObj];

  // step 5 : set above two for display link

  CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext,
                                                    cglPixelFormat);

  // step 6 : start display link (start OpenGL Thread)

  CVDisplayLinkStart(displayLink);
}

- (void)reshape {
  [super reshape];

  [[self openGLContext] makeCurrentContext];

  CGLLockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);

  NSRect rect = [self bounds];

  int width = rect.size.width;

  int height = rect.size.height;

  [self resize:width:height];

  CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
}

- (void)drawRect:(NSRect)dirtyRect {
  // code

  // call rendering function here too, to avoid flikkering
}

- (void)drawView // our rendering function
{
  [[self openGLContext] makeCurrentContext];

  CGLLockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);

  [self display];

  CGLFlushDrawable((CGLContextObj)[[self openGLContext] CGLContextObj]);

  CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
}

// below function gets called per frame using auto release pool

- (CVReturn)getFrameForTime:(const CVTimeStamp *)outputTime {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

  // call rendering function here too

  [self drawView];

  [self myUpdate];

  [pool release];

  return (kCVReturnSuccess);
}

- (int)initialize {
  // code

  [self printGLInfo]; // copy paste point

  // vertex shader

  const GLchar *vertexShaderSourceCode =
      "#version 410 core\n" // (version of GLSL * 100) core means you are using
                            // core profile i.e PP
    "\n"
            "in vec4 aPosition;\n"
            "in vec3 aNormal;\n"
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
            "gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;\n"
            "}";

  // step 2: Create vertex shader object

  GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

  // step 3 : Give vertex shader code to OpenGL
  glShaderSource(vertexShaderObject, 1,
                 (const GLchar **)&vertexShaderSourceCode, NULL);

  // Step 4 : Let OpenGL engine Compile the vertex shader code
  glCompileShader(vertexShaderObject);

  GLint status = 0;
  GLint infoLogLength = 0;
  GLchar *szInfoLog = NULL;

  // step 5 : Check for vertex shader compilation
  // step 5 (a) :  get compile status in status variable
  glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);

  // step 5 (b) : if error occur i.e status == GL_FALSE,  get length of error
  // message into infoLogLength variable
  if (status == GL_FALSE) {

    glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

    if (infoLogLength > 0) {
      // step 5 (c) : declare a string variable allocate memory equal to above
      // length i.e length of error message
      szInfoLog = (GLchar *)malloc(infoLogLength);

      if (szInfoLog != NULL) {
        // step 5 (d) : get the compilation error log info into memory allocated
        // string
        glGetShaderInfoLog(vertexShaderObject, infoLogLength, NULL, szInfoLog);

        // step 5 (e) : print error message into log file
        fprintf(gpFile, "Vertex shader compilation error log : %s\n",
                szInfoLog);

        // step 5 (f) : free allocated memory
        free(szInfoLog);
        szInfoLog = NULL;

        // step 5 (g) : uninitialize and exit.
      }
    }

    [self uninitialize];
    [self release];
    [NSApp terminate:self];
  }

  // follow all steps of vertex shader for fragment shader

  // step 6  : Write the source code for fragment shader

  // fragment shader

  const GLchar *fragmentShaderSourceCode =
      "#version 410 core\n"
    "\n"
            "in vec3 otransformedNormals;\n"
            "in vec3 olightDirection;\n"
            "in vec3 oviewerVector;\n"
            "uniform vec3 ulightAmbient;\n"
            "uniform vec3 ulightDiffuse;\n"
            "uniform vec3 ulightSpecular;\n"
            "uniform vec3 uMaterialAmbient;\n"
            "uniform vec3 uMaterialDiffuse;\n"
            "uniform vec3 uMaterialSpecular;\n"
            "uniform float uMaterialShininess;\n"
            "uniform int uKeyPress;\n"
            "out vec4 FragColor;\n"
            "void main(void)\n"
            "{\n"
            "vec3 PhongADSLight;\n" // local variable
            "if(uKeyPress == 1)\n"
            "{\n"
            "vec3 normalizedTransformedNormals = normalize(otransformedNormals);\n"
            "vec3 normalizedLightDirection = normalize(olightDirection);\n"
            "vec3 normalizedViwerVector = normalize(oviewerVector);\n"
            "vec3 ambientLight = ulightAmbient * uMaterialAmbient;\n"
            "vec3 diffuseLight = ulightDiffuse * uMaterialDiffuse * max(dot(normalizedLightDirection, normalizedTransformedNormals),0.0);\n"
            "vec3 reflectionVector = reflect(-normalizedLightDirection, normalizedTransformedNormals);\n"
            "vec3 specularLight = ulightSpecular * uMaterialSpecular * pow(max(dot(reflectionVector, normalizedViwerVector), 0.0),uMaterialShininess);\n"
            "PhongADSLight = ambientLight + diffuseLight + specularLight;\n "
            "}\n"
            "else\n"
            "{\n"
            "PhongADSLight = vec3(1.0, 1.0, 1.0);\n"
            "}\n"
            "FragColor =  vec4 (PhongADSLight, 1.0);\n"
            "}";
    
    
  GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(fragmentShaderObject, 1,
                 (const GLchar **)&fragmentShaderSourceCode, NULL);

  glCompileShader(fragmentShaderObject);

  status = 0;
  infoLogLength = 0;
  szInfoLog = NULL;

  glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);

  if (status == GL_FALSE) {

    glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

    if (infoLogLength > 0) {
      szInfoLog = (GLchar *)malloc(infoLogLength);

      if (szInfoLog != NULL) {
        glGetShaderInfoLog(fragmentShaderObject, infoLogLength, NULL,
                           szInfoLog);
        fprintf(gpFile, "Fragment shader compilation error log : %s\n",
                szInfoLog);
        free(szInfoLog);
          szInfoLog = NULL;
      }
    }

    [self uninitialize];
    [self release];
    [NSApp terminate:self];
  }

  // linking process for shader program.

  // shader program

  // step 7 : create shader program object

  shaderProgramObject = glCreateProgram();

  // step 8 : Attach both shaders into this program            also know as
  // prelinking binding  should be done before linking and after attachment
  glAttachShader(shaderProgramObject, vertexShaderObject);
  glAttachShader(shaderProgramObject, fragmentShaderObject);
    
    
    //step 9: bind attribute location to the shader program object

    glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_POSITION, "aPosition");

    glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_NORMAL, "aNormal");
    
  // step 10: Link the shader program

  glLinkProgram(shaderProgramObject);

  // step 11: do linking error check fpr shader program

  status = 0;
  infoLogLength = 0;
  szInfoLog = NULL;

  glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);

  if (status == GL_FALSE) {
    glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);

    if (infoLogLength > 0) {
      szInfoLog = (GLchar *)malloc(infoLogLength);

      if (szInfoLog != NULL) {
        glGetProgramInfoLog(shaderProgramObject, infoLogLength, NULL,
                            szInfoLog);
        fprintf(gpFile, "shader program linking error log : %s\n", szInfoLog);
        free(szInfoLog);
        szInfoLog = NULL;
      }
    }

    [self uninitialize];
    [self release];
    [NSApp terminate:self];
  }

  // step 12: Declare position and color arrays

  // get Shader uniform Locations

 // mvpMatrixUniform = glGetUniformLocation(shaderProgramObject,"uMVPMatrix"); // must be done after linkage of shader program
    
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

    
    // call makeSphere

  makeSphere(sphere, 1.35, 40, 40);

  // depth setting

  glClearDepth(1.0f);      // Compulsory
  glEnable(GL_DEPTH_TEST); // 8 test paike ek(depth) test enable kr
  glDepthFunc(GL_LEQUAL);

  // clear color

  glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

  perspectiveProjectionMatrix = vmath::mat4::identity();

  [self resize:WIN_WIDTH:WIN_HEIGHT];

  return 0;
}

- (void)printGLInfo {
  // code

  fprintf(gpFile, "OpenGL Vendor : %s\n\n", glGetString(GL_VENDOR));

  fprintf(gpFile, "OpenGL Renderer : %s\n\n", glGetString(GL_RENDERER));

  fprintf(gpFile, "OpenGL Version : %s\n\n", glGetString(GL_VERSION));

  fprintf(gpFile, "Graphic Library Shading Language  (GLSL) Version : %s\n\n",
          glGetString(GL_SHADING_LANGUAGE_VERSION));
}

- (void)resize:(int)width :(int)height
{
  // code
  if (height <= 0)
    height = 1;
    
    WindowHeight = height;
    WindowWidth = width;
  

  glViewport(0, 0, (GLsizei)width, (GLsizei)height);

  

  perspectiveProjectionMatrix =
      vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

- (void)display 
{
    //variable declaration

    GLfloat horizontalDistance = WindowWidth / 6;
    GLfloat verticalDistance = WindowHeight / 7;

    GLfloat materialAmbient[4];
    GLfloat materialDiffuse[4];
    GLfloat materialSpecular[4];

    GLfloat materialShininess;
    
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // step 1: Use shader program object

  glUseProgram(shaderProgramObject);

    //(emerald)

    materialAmbient[0] = 0.0215;
    materialAmbient[1] = 0.1745;
    materialAmbient[2] = 0.0215;
    materialAmbient[3] = 1.0f;



    materialDiffuse[0] = 0.07568;
    materialDiffuse[1] = 0.61424;
    materialDiffuse[2] = 0.07568;
    materialDiffuse[3] = 1.0f;



    materialSpecular[0] = 0.633;
    materialSpecular[1] = 0.727811;
    materialSpecular[2] = 0.633;
    materialSpecular[3] = 1.0f;



    materialShininess = 0.6f * 128.0f;  // transformations

    //Sphere

    
    
    //transformations

    mat4 modelViewMatrix = mat4::identity();

    mat4 translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    mat4 modelMatrix = translationMatrix;

    mat4 viewMatrix = mat4::identity();

    //push above mvp into vertex shader's mvp uniform

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
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

    //first column first sphere

    glViewport(0, (verticalDistance * 5), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));
    
  sphere.draw();
    
    //first column second sphere
    glViewport(0, (verticalDistance * 4), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));


    materialAmbient[0] = 0.135;
    materialAmbient[1] = 0.2225;
    materialAmbient[2] = 0.1575;
    materialAmbient[3] = 1.0f;



    materialDiffuse[0] = 0.54;
    materialDiffuse[1] = 0.89;
    materialDiffuse[2] = 0.63;
    materialDiffuse[3] = 1.0f;



    materialSpecular[0] = 0.316228;
    materialSpecular[1] = 0.316228;
    materialSpecular[2] = 0.316228;
    materialSpecular[3] = 1.0f;



    materialShininess = 0.1 * 128;
    
    //transformations

     modelViewMatrix = mat4::identity();

     translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

     modelMatrix = translationMatrix;

     viewMatrix = mat4::identity();

    //push above mvp into vertex shader's mvp uniform

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
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
    
    
    sphere.draw();  // step 5: Unuse shader program object
    
    //first column Third sphere

    glViewport(0, (verticalDistance * 3), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));


    materialAmbient[0] = 0.05375;
    materialAmbient[1] = 0.05;
    materialAmbient[2] = 0.06625;
    materialAmbient[3] = 1.0f;

    

    materialDiffuse[0] = 0.18275;
    materialDiffuse[1] = 0.17;
    materialDiffuse[2] = 0.22525;
    materialDiffuse[3] = 1.0f;

    

    materialSpecular[0] = 0.332741;
    materialSpecular[1] = 0.328634;
    materialSpecular[2] = 0.346435;
    materialSpecular[3] = 1.0f;

    

    materialShininess = 0.3 * 128;
    //transformations

    modelViewMatrix = mat4::identity();

    translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    modelMatrix = translationMatrix;

    viewMatrix = mat4::identity();

    //push above mvp into vertex shader's mvp uniform

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
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
    
    sphere.draw();
    
    //first column Fourth sphere

    glViewport(0, (verticalDistance * 2), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));


    materialAmbient[0] = 0.25;
    materialAmbient[1] = 0.20725;
    materialAmbient[2] = 0.20725;
    materialAmbient[3] = 1.0f;



    materialDiffuse[0] = 1.0;
    materialDiffuse[1] = 0.829;
    materialDiffuse[2] = 0.829;
    materialDiffuse[3] = 1.0f;



    materialSpecular[0] = 0.296648;
    materialSpecular[1] = 0.296648;
    materialSpecular[2] = 0.296648;
    materialSpecular[3] = 1.0f;



    materialShininess = 0.088 * 128;

    //transformations

    modelViewMatrix = mat4::identity();

    translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    modelMatrix = translationMatrix;

    viewMatrix = mat4::identity();

    //push above mvp into vertex shader's mvp uniform

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
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


    sphere.draw();
    
    //first column Fifth sphere

    glViewport(0, (verticalDistance * 1), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));


    materialAmbient[0] = 0.1745;
    materialAmbient[1] = 0.01175;
    materialAmbient[2] = 0.01175;
    materialAmbient[3] = 1.0f;


    materialDiffuse[0] = 0.61424;
    materialDiffuse[1] = 0.04136;
    materialDiffuse[2] = 0.04136;
    materialDiffuse[3] = 1.0f;


    materialSpecular[0] = 0.727811;
    materialSpecular[1] = 0.626959;
    materialSpecular[2] = 0.626959;
    materialSpecular[3] = 1.0f;


    materialShininess = 0.6 * 128;

    //transformations

    modelViewMatrix = mat4::identity();

    translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    modelMatrix = translationMatrix;

    viewMatrix = mat4::identity();

    //push above mvp into vertex shader's mvp uniform

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
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


    sphere.draw(); // 3rd parameter should be that

    //first column Sixth sphere

    glViewport(0, 0, (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));


    materialAmbient[0] = 0.1;
    materialAmbient[1] = 0.18725;
    materialAmbient[2] = 0.1745;
    materialAmbient[3] = 1.0f;


    materialDiffuse[0] = 0.396;
    materialDiffuse[1] = 0.74151;
    materialDiffuse[2] = 0.69102;
    materialDiffuse[3] = 1.0f;


    materialSpecular[0] = 0.297254;
    materialSpecular[1] = 0.30829;
    materialSpecular[2] = 0.306678;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.1 * 128;

    //transformations

    modelViewMatrix = mat4::identity();

    translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    modelMatrix = translationMatrix;

    viewMatrix = mat4::identity();

    //push above mvp into vertex shader's mvp uniform

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
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


    sphere.draw();
    
    //second column first sphere

    glViewport((horizontalDistance), (verticalDistance * 5), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

    materialAmbient[0] = 0.329412;
    materialAmbient[1] = 0.223529;
    materialAmbient[2] = 0.027451;
    materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.780392;
    materialDiffuse[1] = 0.568627;
    materialDiffuse[2] = 0.113725;
    materialDiffuse[3] = 1.0f;


    materialSpecular[0] = 0.992157;
    materialSpecular[1] = 0.941176;
    materialSpecular[2] = 0.807843;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.21794872 * 128;

    //transformations

    modelViewMatrix = mat4::identity();

    translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    modelMatrix = translationMatrix;

    viewMatrix = mat4::identity();

    //push above mvp into vertex shader's mvp uniform

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
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


    sphere.draw();

    //second column second sphere

    glViewport((horizontalDistance), (verticalDistance * 4), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

    materialAmbient[0] = 0.2125;
    materialAmbient[1] = 0.1275;
    materialAmbient[2] = 0.054;
    materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.714;
    materialDiffuse[1] = 0.568627;
    materialDiffuse[2] = 0.113725;
    materialDiffuse[3] = 1.0f;

    materialSpecular[0] = 0.992157;
    materialSpecular[1] = 0.941176;
    materialSpecular[2] = 0.807843;
    materialSpecular[3] = 1.0f;


    materialShininess = 0.21794872 * 128;

    //transformations

    modelViewMatrix = mat4::identity();

    translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    modelMatrix = translationMatrix;

    viewMatrix = mat4::identity();

    //push above mvp into vertex shader's mvp uniform

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
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


    sphere.draw();


    //second column third sphere

    glViewport((horizontalDistance), (verticalDistance * 3), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

    materialAmbient[0] = 0.25;
    materialAmbient[1] = 0.25;
    materialAmbient[2] = 0.25;
    materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.4;
    materialDiffuse[1] = 0.4;
    materialDiffuse[2] = 0.4;
    materialDiffuse[3] = 1.0f;


    materialSpecular[0] = 0.774597;
    materialSpecular[1] = 0.774597;
    materialSpecular[2] = 0.774597;
    materialSpecular[3] = 1.0f;


    materialShininess = 0.6 * 128;

    //transformations

    modelViewMatrix = mat4::identity();

    translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    modelMatrix = translationMatrix;

    viewMatrix = mat4::identity();

    //push above mvp into vertex shader's mvp uniform

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
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


    sphere.draw();


    //second column fourth sphere

    glViewport((horizontalDistance), (verticalDistance * 2), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

    materialAmbient[0] = 0.19125;
    materialAmbient[1] = 0.0735;
    materialAmbient[2] = 0.0225;
    materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.7038;
    materialDiffuse[1] = 0.27048;
    materialDiffuse[2] = 0.0828;
    materialDiffuse[3] = 1.0f;


    materialSpecular[0] = 0.256777;
    materialSpecular[1] = 0.137622;
    materialSpecular[2] = 0.086014;
    materialSpecular[3] = 1.0f;


    materialShininess = 0.1 * 128;

    //transformations

    modelViewMatrix = mat4::identity();

    translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    modelMatrix = translationMatrix;

    viewMatrix = mat4::identity();

    //push above mvp into vertex shader's mvp uniform

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
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


    sphere.draw();

    //second column fifth sphere

    glViewport((horizontalDistance), (verticalDistance * 1), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

    materialAmbient[0] = 0.24725;
    materialAmbient[1] = 0.1995;
    materialAmbient[2] = 0.0745;
    materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.75164;
    materialDiffuse[1] = 0.60648;
    materialDiffuse[2] = 0.22648;
    materialDiffuse[3] = 1.0f;


    materialSpecular[0] = 0.628281;
    materialSpecular[1] = 0.555802;
    materialSpecular[2] = 0.366065;
    materialSpecular[3] = 1.0f;


    materialShininess = 0.4 * 128;

    //transformations

    modelViewMatrix = mat4::identity();

    translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    modelMatrix = translationMatrix;

    viewMatrix = mat4::identity();

    //push above mvp into vertex shader's mvp uniform

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
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


    sphere.draw();


    //second column Sixth sphere

    glViewport((horizontalDistance), 0, (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

    materialAmbient[0] = 0.19225;
    materialAmbient[1] = 0.19225;
    materialAmbient[2] = 0.19225;
    materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.50754;
    materialDiffuse[1] = 0.50754;
    materialDiffuse[2] = 0.50754;
    materialDiffuse[3] = 1.0f;


    materialSpecular[0] = 0.508273;
    materialSpecular[1] = 0.508273;
    materialSpecular[2] = 0.508273;
    materialSpecular[3] = 1.0f;


    materialShininess = 0.4 * 128;

    //transformations

    modelViewMatrix = mat4::identity();

    translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    modelMatrix = translationMatrix;

    viewMatrix = mat4::identity();

    //push above mvp into vertex shader's mvp uniform

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
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


    sphere.draw();
    
    //Third column First sphere

    glViewport((horizontalDistance * 2), (verticalDistance * 5), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

    materialAmbient[0] = 0.0f;
    materialAmbient[1] = 0.0f;
    materialAmbient[2] = 0.0f;
    materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.01;
    materialDiffuse[1] = 0.01;
    materialDiffuse[2] = 0.01;
    materialDiffuse[3] = 1.0f;


    materialSpecular[0] = 0.5;
    materialSpecular[1] = 0.5;
    materialSpecular[2] = 0.5;
    materialSpecular[3] = 1.0f;


    materialShininess = 0.25 * 128;

    //transformations

    modelViewMatrix = mat4::identity();

    translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    modelMatrix = translationMatrix;

    viewMatrix = mat4::identity();

    //push above mvp into vertex shader's mvp uniform

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
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


    sphere.draw();
    //Third column Second sphere

    glViewport((horizontalDistance * 2), (verticalDistance * 4), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.1;
    materialAmbient[2] = 0.06;
    materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.0;
    materialDiffuse[1] = 0.50980392;
    materialDiffuse[2] = 0.50980392;
    materialDiffuse[3] = 1.0f;


    materialSpecular[0] = 0.50196078;
    materialSpecular[1] = 0.50196078;
    materialSpecular[2] = 0.50196078;
    materialSpecular[3] = 1.0f;


    materialShininess = 0.25 * 128;

    //transformations

    modelViewMatrix = mat4::identity();

    translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    modelMatrix = translationMatrix;

    viewMatrix = mat4::identity();

    //push above mvp into vertex shader's mvp uniform

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
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


    sphere.draw();

    //Third column Third sphere

    glViewport((horizontalDistance * 2), (verticalDistance * 3), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.0;
    materialAmbient[2] = 0.0;
    materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.1;
    materialDiffuse[1] = 0.35;
    materialDiffuse[2] = 0.1;
    materialDiffuse[3] = 1.0f;


    materialSpecular[0] = 0.45;
    materialSpecular[1] = 0.55;
    materialSpecular[2] = 0.45;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.25 * 128;

    //transformations

    modelViewMatrix = mat4::identity();

    translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    modelMatrix = translationMatrix;

    viewMatrix = mat4::identity();

    //push above mvp into vertex shader's mvp uniform

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
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


    sphere.draw();

    //Third column fourth sphere

    glViewport((horizontalDistance * 2), (verticalDistance * 2), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.0;
    materialAmbient[2] = 0.0;
    materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.5;
    materialDiffuse[1] = 0.0;
    materialDiffuse[2] = 0.0;
    materialDiffuse[3] = 1.0f;


    materialSpecular[0] = 0.7;
    materialSpecular[1] = 0.6;
    materialSpecular[2] = 0.6;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.25 * 128;

    //transformations

    modelViewMatrix = mat4::identity();

    translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    modelMatrix = translationMatrix;

    viewMatrix = mat4::identity();

    //push above mvp into vertex shader's mvp uniform

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
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


    sphere.draw();

    //Third column fifth sphere

    glViewport((horizontalDistance * 2), (verticalDistance ), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.0;
    materialAmbient[2] = 0.0;
    materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.55;
    materialDiffuse[1] = 0.55;
    materialDiffuse[2] = 0.55;
    materialDiffuse[3] = 1.0f;


    materialSpecular[0] = 0.70;
    materialSpecular[1] = 0.70;
    materialSpecular[2] = 0.70;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.25 * 128;

    //transformations

    modelViewMatrix = mat4::identity();

    translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    modelMatrix = translationMatrix;

    viewMatrix = mat4::identity();

    //push above mvp into vertex shader's mvp uniform

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
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


    sphere.draw();

    //Third column sixth sphere

    glViewport((horizontalDistance * 2), 0, (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.0;
    materialAmbient[2] = 0.0;
    materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.5;
    materialDiffuse[1] = 0.5;
    materialDiffuse[2] = 0.0;
    materialDiffuse[3] = 1.0f;


    materialSpecular[0] = 0.60;
    materialSpecular[1] = 0.60;
    materialSpecular[2] = 0.50;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.25 * 128;

    //transformations

    modelViewMatrix = mat4::identity();

    translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    modelMatrix = translationMatrix;

    viewMatrix = mat4::identity();

    //push above mvp into vertex shader's mvp uniform

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
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


    sphere.draw();
    
    //Fourth Column First sphere

    glViewport((horizontalDistance * 3), (verticalDistance * 5), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

    materialAmbient[0] = 0.02;
    materialAmbient[1] = 0.02;
    materialAmbient[2] = 0.02;
    materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.01;
    materialDiffuse[1] = 0.01;
    materialDiffuse[2] = 0.01;
    materialDiffuse[3] = 1.0f;


    materialSpecular[0] = 0.4;
    materialSpecular[1] = 0.4;
    materialSpecular[2] = 0.4;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.078125 * 128;

    //transformations

    modelViewMatrix = mat4::identity();

    translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    modelMatrix = translationMatrix;

    viewMatrix = mat4::identity();

    //push above mvp into vertex shader's mvp uniform

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
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


    sphere.draw();

    //Fourth Column Second sphere

    glViewport((horizontalDistance * 3), (verticalDistance * 4), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.05;
    materialAmbient[2] = 0.05;
    materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.4;
    materialDiffuse[1] = 0.5;
    materialDiffuse[2] = 0.5;
    materialDiffuse[3] = 1.0f;


    materialSpecular[0] = 0.04;
    materialSpecular[1] = 0.7;
    materialSpecular[2] = 0.7;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.078125 * 128;

    //transformations

    modelViewMatrix = mat4::identity();

    translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    modelMatrix = translationMatrix;

    viewMatrix = mat4::identity();

    //push above mvp into vertex shader's mvp uniform

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
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


    sphere.draw();

    //Fourth Column Third sphere

    glViewport((horizontalDistance * 3), (verticalDistance * 3), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.05;
    materialAmbient[2] = 0.0;
    materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.4;
    materialDiffuse[1] = 0.5;
    materialDiffuse[2] = 0.4;
    materialDiffuse[3] = 1.0f;

    materialSpecular[0] = 0.04;
    materialSpecular[1] = 0.7;
    materialSpecular[2] = 0.04;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.078125 * 128;

    //transformations

    modelViewMatrix = mat4::identity();

    translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    modelMatrix = translationMatrix;

    viewMatrix = mat4::identity();

    //push above mvp into vertex shader's mvp uniform

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
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


    sphere.draw();

    //Fourth Column fourth sphere

    glViewport((horizontalDistance * 3), (verticalDistance * 2), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

    materialAmbient[0] = 0.05;
    materialAmbient[1] = 0.0;
    materialAmbient[2] = 0.0;
    materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.5;
    materialDiffuse[1] = 0.4;
    materialDiffuse[2] = 0.4;
    materialDiffuse[3] = 1.0f;

    materialSpecular[0] = 0.7;
    materialSpecular[1] = 0.04;
    materialSpecular[2] = 0.04;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.078125 * 128;

    //transformations

    modelViewMatrix = mat4::identity();

    translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    modelMatrix = translationMatrix;

    viewMatrix = mat4::identity();

    //push above mvp into vertex shader's mvp uniform

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
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


    sphere.draw();

    //Fourth Column fifth sphere

    glViewport((horizontalDistance * 3), (verticalDistance * 1), (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

    materialAmbient[0] = 0.05;
    materialAmbient[1] = 0.05;
    materialAmbient[2] = 0.05;
    materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.5;
    materialDiffuse[1] = 0.5;
    materialDiffuse[2] = 0.5;
    materialDiffuse[3] = 1.0f;

    materialSpecular[0] = 0.7;
    materialSpecular[1] = 0.7;
    materialSpecular[2] = 0.7;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.078125 * 128;

    //transformations

    modelViewMatrix = mat4::identity();

    translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    modelMatrix = translationMatrix;

    viewMatrix = mat4::identity();

    //push above mvp into vertex shader's mvp uniform

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
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


    sphere.draw();
    //Fourth Column sixth sphere

    glViewport((horizontalDistance * 3), 0, (GLsizei)(WindowWidth / 4), (GLsizei)(WindowHeight / 4));

    materialAmbient[0] = 0.05;
    materialAmbient[1] = 0.05;
    materialAmbient[2] = 0.0;
    materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.5;
    materialDiffuse[1] = 0.5;
    materialDiffuse[2] = 0.4;
    materialDiffuse[3] = 1.0f;

    materialSpecular[0] = 0.7;
    materialSpecular[1] = 0.7;
    materialSpecular[2] = 0.04;
    materialSpecular[3] = 1.0f;


    materialShininess = 0.078125 * 128;

    //transformations

    modelViewMatrix = mat4::identity();

    translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    modelMatrix = translationMatrix;

    viewMatrix = mat4::identity();

    //push above mvp into vertex shader's mvp uniform

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);


    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);


    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
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


    sphere.draw();

        
  glUseProgram(0);
}

- (void)myUpdate 
{
    // code
    
    //code
    //only at time of change animation here code will come
    
    if (keyPressed == 1)
    {
        if (angleForXRotation < (2 * M_PI))   // rotation around X axis
        {
            lightPosition[0] = 0.0f;
            lightPosition[1] = 45.0f * (GLfloat)sin(angleForXRotation);
            lightPosition[2] = 45.0f * (GLfloat)cos(angleForXRotation);
            lightPosition[3] = 1.0f;
            
            angleForXRotation = angleForXRotation + 0.01;
        }
        
        else
        {
            angleForXRotation = angleForXRotation - (2 * M_PI);
        }
        
    }
    
    if (keyPressed == 2)   // rotation around Y axis
    {
        if (angleForYRotation < (2 * M_PI))
        {
            lightPosition[0] = 45.0f * (GLfloat)sin(angleForYRotation);
            lightPosition[1] = 0.0f;
            lightPosition[2] = 45.0f * (GLfloat)cos(angleForYRotation);
            lightPosition[3] = 1.0f;
            
            angleForYRotation = angleForYRotation + 0.01;
        }
        
        else
        {
            angleForYRotation = angleForYRotation - (2 * M_PI);
        }
        
    }
    
    if (keyPressed == 3)   // rotation around Z axis
    {
        if (angleForZRotation < (2 * M_PI))
        {
            lightPosition[0] = 45.0f * (GLfloat)sin(angleForZRotation);
            lightPosition[1] = 45.0f * (GLfloat)cos(angleForZRotation);
            lightPosition[2] = 0.0f;
            lightPosition[3] = 1.0f;
            
            angleForZRotation = angleForZRotation + 0.01;
        }
        
        else
        {
            angleForZRotation = angleForZRotation - (2 * M_PI);
        }
        
    }
}

- (void)uninitialize {
  // code

  // steps for uninitialize function in PP

  if (shaderProgramObject) {
    // step 1 : use shader program object

    glUseProgram(shaderProgramObject);

    GLint numShaders = 0;

    // step 2 : get number of attached shaders
    glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &numShaders);

    if (numShaders > 0) {
      // step 3 : Create and allocate memory of shader objects according to
      // number of attached shaders
      GLuint *pShaders = (GLuint *)malloc(numShaders * sizeof(GLuint));

      if (pShaders != NULL) {
        // step 4 : Get shader object into the allocated array.

        glGetAttachedShaders(shaderProgramObject, numShaders, NULL, pShaders);

        // step 5 : Detach and delete shader objects iteratively

        for (GLint i = 0; i < numShaders; i++) {
          glDetachShader(shaderProgramObject, pShaders[i]);
          glDeleteShader(pShaders[i]);
          pShaders[i] = 0;
        }
        // step 6 : Free the memory allocated for array of shader objects

        free(pShaders);
        pShaders = NULL;
      }
    }
    // step 7 : Unuse shader program objects

    glUseProgram(0);

    // step 8 : Delete shader program objects

    glDeleteProgram(shaderProgramObject);
    shaderProgramObject = 0;
  }
}

- (BOOL)acceptsFirstResponder {
  // code

  [[self window] makeFirstResponder:self];

  return (YES);
}

- (void)keyDown:(NSEvent *)event {
  // code

  int key = (int)[[event characters] characterAtIndex:0];

  switch (key) {
  case 27:
    [self uninitialize];
    [self release];
    [NSApp terminate:self];

    break;

  case 'f':
  case 'F':
    [[self window] toggleFullScreen:self];
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
}

- (void)mouseDown:(NSEvent *)event {
}

- (void)rightMouseDown:(NSEvent *)event {
}

- (void)dealloc {
  [super dealloc];

  [self uninitialize];

  if (displayLink) {
    CVDisplayLinkStop(displayLink);
    CVDisplayLinkRelease(displayLink);
    displayLink = nil;
  }
}

@end

// definition of global callback function

CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink,
                               const CVTimeStamp *now,
                               const CVTimeStamp *outputTime,
                               CVOptionFlags flagsIn, CVOptionFlags *flagsOut,
                               void *displayLinkContext) {
  // code

  CVReturn result = [(GLview *)displayLinkContext getFrameForTime:outputTime];

  return (result);
}
