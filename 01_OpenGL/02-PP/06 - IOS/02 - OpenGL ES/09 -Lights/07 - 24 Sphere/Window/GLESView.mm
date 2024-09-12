//
//  MyView.m
//  Window
//
//  Created by user258815 on 8/4/24.
//

#define GLES_SILENCE_DEPRECATION

#import<OpenGLES/ES3/gl.h>

#import<OpenGLES/ES3/glext.h>

#import "GLESView.h"

#import "vmath.h"

#import "sphere.h"

using namespace vmath;


enum
{
    AMC_ATTRIBUTE_POSITION = 0,

    AMC_ATTRIBUTE_NORMAL = 1
};


// Light Variables
GLfloat lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // white diffuse light
GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f }; //


GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat materialDiffuse[] = { 0.5f, 0.2f, 0.7f, 1.0f };
GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialShininess = 128.0f;

bool bLightingEnable = false;


@implementation GLESView
{
    EAGLContext * eaglContext;

    GLuint customeFrameBuffer;

    GLuint colorRenderBuffer;

    GLuint depthRenderBuffer;

    id displayLink;

    NSInteger framesPerSecond;

    bool isDisplayLink;

    // OpenGL related field variables

    GLuint shaderProgramObject;

      GLuint mvpMatrixUniform;

      mat4 perspectiveProjectionMatrix;

      mesh sphere;


      // Uniforms

      GLuint modelMatrixUniform;
      GLuint viewMatrixUniform;
      GLuint projectionMatrixUniform;

      GLuint lightDiffuseUniform;
      GLuint lightAmbientUniform;
      GLuint lightSpecularUniform;
      GLuint lightPositionUniform;

      GLuint materialDiffuseUniform;
      GLuint materialAmbientUniform;
      GLuint materialSpecularUniform;
      GLuint materialShininessUniform;

      GLuint keyPressUniform;

      GLint touchCount;

     GLfloat WindowWidth;
     GLfloat WindowHeight;

     //light rotation variables

    GLfloat angleForXRotation;
    GLfloat angleForYRotation;
    GLfloat angleForZRotation;


}

//-(id)initWithFrame:(CGRect)frame
- (id)initWithFrame:(CGRect)frame
{
    //code

    self = [super initWithFrame : frame];

    if (self)
    {
        //set the background to black

        [self setBackgroundColor : [UIColor blackColor] ] ;

        // step 1 : create EAGL Layer

        CAEAGLLayer* eaglLayer = (CAEAGLLayer*)[super layer];

        [eaglLayer setOpaque : YES] ;

        NSDictionary* dictionary = [NSDictionary dictionaryWithObjectsAndKeys : [NSNumber numberWithBool : NO] , kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];

        [eaglLayer setDrawableProperties : dictionary] ;

        //step 2 : create EAGL Context

        eaglContext = [[EAGLContext alloc]initWithAPI:kEAGLRenderingAPIOpenGLES3];

        if (eaglContext == nil)
        {
            printf("OpenGL ES Context creation failed !!");
            [self uninitialize] ;
            [self release] ;
            exit(0);
        }

        //step 3 : set this context as current context

        [EAGLContext setCurrentContext : eaglContext];

        //step 4 : Create custome frame buffer

        glGenFramebuffers(1, &customeFrameBuffer);

        glBindFramebuffer(GL_FRAMEBUFFER, customeFrameBuffer);

        //create color render buffer

        glGenRenderbuffers(1, &colorRenderBuffer);

        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);

        //give storage to color render buffer by using IOS Method

        [eaglContext renderbufferStorage : GL_RENDERBUFFER fromDrawable : eaglLayer];

        //step 5 : give above color buffer to custome frame buffer

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBuffer);

        //depth render buffer

        //find the width of the color buffer

        GLint width;

        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);

        //heigt

        GLint height;

        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);

        //create and bind with depth render buffer

        glGenRenderbuffers(1, &depthRenderBuffer);

        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);

        //give storage to depth render buffer

        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);

        //give this depth render buffer to custome frame buffer

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

        //check status of newly created custom framebuffer

        GLenum framebuffer_Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

        if (framebuffer_Status != GL_FRAMEBUFFER_COMPLETE)
        {
            printf("Frame buffer creation failed !!");
            [self uninitialize] ;
            [self release] ;
            exit(0);
        }

        //initialize frames per second

        framesPerSecond = 60;  //value 60 is recomended from ios 8.2

        //initialize display link existance variable

        isDisplayLink = NO;



        //call our initialize method

        int result = [self initialize];

        if (result != 0)
        {
            printf("initialize function failed !!");
            [self uninitialize] ;
            [self release] ;
            exit(0);
        }



        //single tap

        //create object of single tap gesture recognizer object

        UITapGestureRecognizer* singleTapGestureRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action : @selector(OnsingleTap:)];

        // set number of taps

        [singleTapGestureRecognizer setNumberOfTapsRequired : 1];

        //set number of fingers

        [singleTapGestureRecognizer setNumberOfTouchesRequired : 1];

        //set the deligate as self

        [singleTapGestureRecognizer setDelegate : self];

        //add this gesture recognizer to self

        [self addGestureRecognizer : singleTapGestureRecognizer];

        //double tap

        UITapGestureRecognizer* doubleTapGestureRecognizer = [[UITapGestureRecognizer alloc ]initWithTarget:self action : @selector(OndoubleTap:)];

        // set number of taps

        [doubleTapGestureRecognizer setNumberOfTapsRequired : 2];

        //set number of fingers

        [doubleTapGestureRecognizer setNumberOfTouchesRequired : 1];

        //set the deligate as self

        [doubleTapGestureRecognizer setDelegate : self];

        //add this gesture recognizer to self

        [self addGestureRecognizer : doubleTapGestureRecognizer];

        //double tap should fail for two single taps

        [singleTapGestureRecognizer requireGestureRecognizerToFail : doubleTapGestureRecognizer];

        //swipe

        UISwipeGestureRecognizer* swipeGestureRecognizer = [[UISwipeGestureRecognizer alloc]initWithTarget:self action : @selector(OnSwipe:)];

        [swipeGestureRecognizer setDelegate : self] ;

        [self addGestureRecognizer : swipeGestureRecognizer] ;

        //longpress

        UILongPressGestureRecognizer* longPressGestureRecognizer = [[UILongPressGestureRecognizer alloc]initWithTarget:self action : @selector(onLongPress:)];

        [longPressGestureRecognizer setDelegate : self] ;

        [self addGestureRecognizer : longPressGestureRecognizer] ;



    }

    return(self);
}



+(Class)layerClass
{
    //code

    return([CAEAGLLayer class]);
}

/*-(void)drawRect:(CGRect)rect
{
    //code


}*/

-(void)drawView:(id)displayLink
{
    //code

    //set current context again

    [EAGLContext setCurrentContext : eaglContext];

    //bind with frame buffer again

    glBindBuffer(GL_FRAMEBUFFER, customeFrameBuffer);

    //call your display function

    [self display];

    //call update

    [self myUpdate];

    //bind with the color render buffer again

    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);

    //present this color render buffer which will internally do double buffering

    [eaglContext presentRenderbuffer : GL_RENDERBUFFER];



}

//override layout subviews method

-(void)layoutSubviews
{
    //code

    //bind with the color render buffer again

    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);

    //create color render buffer storage again

    [eaglContext renderbufferStorage : GL_RENDERBUFFER fromDrawable : (CAEAGLLayer*)[self layer]];

    //depth render buffer

    //find the width of the color buffer

    GLint width;

    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);

    //heigt

    GLint height;

    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);

    //create and bind with depth render buffer

    glGenRenderbuffers(1, &depthRenderBuffer);

    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);

    //give storage to depth render buffer

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);

    //give this depth render buffer to custome frame buffer

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

    //check status of newly created custom framebuffer

    GLenum framebuffer_Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (framebuffer_Status != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("Frame buffer creation failed !!");
        [self uninitialize] ;
        [self release] ;
        exit(0);
    }

    //call our resize here

    [self resize : width height : height];

    //it is recomended that you should call draw view here

    [self drawView : displayLink];




}

// start display link custome method which will be called by app delegate

-(void)startDisplayLink
{
    //code

    if (isDisplayLink == NO)
    {
        //create display link

        displayLink = [CADisplayLink displayLinkWithTarget : self selector : @selector(drawView:)];

        //set frame per seconds

        [displayLink setPreferredFramesPerSecond : framesPerSecond];

        //add this display link to our run loop

        [displayLink addToRunLoop : [NSRunLoop currentRunLoop] forMode : NSDefaultRunLoopMode];

        //set isdisplay link to yes

        isDisplayLink = YES;
    }
}

// stop display link custome method which will be called by appdelegate

-(void)stopDisplayLink
{
    if (isDisplayLink == YES)
    {
        //remove display link from runloop by invalidating it

        [displayLink invalidate] ;

        isDisplayLink = NO;
    }
}


- (int)initialize
{
    //code

    [self printGLInfo] ;   // copy paste point

    //vertex shader

    const GLchar* vertexShaderSourceCode =
        "#version 300 es\n"             //make verson to 300
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
        "uniform highp int uKeyPress;\n"
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
                printf("Vertex shader compilation error log : %s\n", szInfoLog);


                //step 5 (f) : free allocated memory
                free(szInfoLog);
                szInfoLog = NULL;


                //step 5 (g) : uninitialize and exit.


            }

        }

        [self uninitialize];
        [self release] ;
        exit(-1);
    }

    //follow all steps of vertex shader for fragment shader

    //step 6  : Write the source code for fragment shader

    //fragment shader

    const GLchar* fragmentShaderSourceCode =
        "#version 300 es\n"
        "\n"
        "precision highp float;\n"         // Specify precision for floating-point types
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
        "uniform highp int uKeyPress;\n"
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
                printf("Fragment shader compilation error log : %s\n", szInfoLog);
                free(szInfoLog);
                szInfoLog = NULL;


            }



        }

        [self uninitialize];
        [self release] ;
        exit(-1);

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
                printf("shader program linking error log : %s\n", szInfoLog);
                free(szInfoLog);
                szInfoLog = NULL;


            }



        }

        [self uninitialize];
        [self release] ;
        exit(-1);

    }

    //step 12: Declare position and color arrays

    //get Shader uniform Locations

    //mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "uMVPMatrix");  //must be done after linkage of shader program

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


    // Make sphere
    makeSphere(sphere, 1.35, 40, 40);

    // depth setting

    glClearDepthf(1.0f); // Compulsory
    glEnable(GL_DEPTH_TEST); // 8 test paike ek(depth) test enable kr
    glDepthFunc(GL_LEQUAL);

    //clear color

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    return 0;
}

- (void)printGLInfo
{
    //code

    printf("OpenGL Vendor : %s\n\n", glGetString(GL_VENDOR));

    printf("OpenGL Renderer : %s\n\n", glGetString(GL_RENDERER));

    printf("OpenGL Version : %s\n\n", glGetString(GL_VERSION));

    printf("Graphic Library Shading Language  (GLSL) Version : %s\n\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

}

- (void)resize:(int)width height : (int)height
{
    //code

    if (height <= 0)
    {
        height = 1;
    }

    WindowHeight = height;
    WindowWidth = width;

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
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

    
    //step 5: Unuse shader program object
    glUseProgram(0);

}

- (void)myUpdate
{
    //code
    if (touchCount == 1)
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
    
    if (touchCount == 2)   // rotation around Y axis
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
    
    if (touchCount == 3)   // rotation around Z axis
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

- (void)uninitialize
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






    // free depth render buffer

    if (depthRenderBuffer)
    {
        glDeleteRenderbuffers(1, (GLuint*)&depthRenderBuffer);
        depthRenderBuffer = 0;
    }

    // delete colore render buffer

    if (colorRenderBuffer)
    {
        glDeleteRenderbuffers(1, &colorRenderBuffer);
        colorRenderBuffer = 0;
    }

    //delete custome frame buffer

    if (customeFrameBuffer)
    {
        glDeleteFramebuffers(1, &customeFrameBuffer);
        customeFrameBuffer = 0;
    }

    //release eagl context

    if (eaglContext && [EAGLContext currentContext] == eaglContext)
    {
        [EAGLContext setCurrentContext : nil] ;

        [eaglContext release] ;

        eaglContext = nil;
    }

}
- (BOOL)becomeFirstResponder
{
    //code

    return(YES);
}

- (void)touchesBegan:(NSSet*)touches withEvent : (UIEvent*)event
{
    //code
}


- (void)OnsingleTap:(UITapGestureRecognizer*)gestureRecognizer
{
    
        touchCount++;

            if(touchCount>3)
            {
                touchCount = 1;
            }


}

- (void)OndoubleTap:(UITapGestureRecognizer*)gestureRecognizer
{
    

    //code

    if (bLightingEnable == false)
    {
        bLightingEnable = true;
    }
    else
    {
        bLightingEnable = false;
    }
}

- (void)OnSwipe : (UISwipeGestureRecognizer*)gestureRecognizer
{
    //code

    [self uninitialize] ;
    [self release] ;
    exit(0);


}

- (void)onLongPress:(UILongPressGestureRecognizer*)gestureRecognizer
{


}

- (void)dealloc
{
    //code

    [super dealloc] ;

    [self uninitialize] ;

    // release display link

    if (displayLink)
    {
        [displayLink invalidate] ;

        [displayLink stop] ;

        [displayLink release] ;

        displayLink = nil;
    }
}


@end
