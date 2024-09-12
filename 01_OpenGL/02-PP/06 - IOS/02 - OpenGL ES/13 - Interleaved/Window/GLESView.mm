

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

using namespace vmath;


// Light Variables

GLfloat lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // white diffuse light
GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightPosition[] = { 100.0f, 100.0f, 100.0f, 1.0f }; //

GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialSpecular[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat materialShininess = 128.0f;

bool bLightingEnable = false;

bool bAnimationEnable = false;


enum
{
    AMC_ATTRIBUTE_POSITION = 0,
    AMC_ATTRIBUTE_TEXCOORD = 1,
    AMC_ATTRIBUTE_NORMAL = 2,
    AMC_ATTRIBUTE_COLOR = 3
};

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

    GLuint vao;
    GLuint vbo_PCNT;
    GLuint vbo_Texcoord;

    GLfloat rAngle;

    GLuint marble_Texture;

    GLuint textureSamplerUniform;

    GLuint mvpMatrixUniform;

    mat4 perspectiveProjectionMatrix;

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

    if (bAnimationEnable == true)
    {
        [self myUpdate] ;
    }
 

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
        "uniform highp int uKeyPress;\n"
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
        "uniform highp int uKeyPress;\n"
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

    glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_TEXCOORD, "aTexCoord");

    glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_NORMAL, "aNormal");

    glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_COLOR, "aColor");

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

    textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "uTextureSampler");

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

    glBufferData(GL_ARRAY_BUFFER, 24 * 11 * sizeof(float), cube_PCNT, GL_STATIC_DRAW); // 24*11*sizeof(float) is nothing but size of cube_PCNT

    //****************** Position *****************************

    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(0 * sizeof(float)));

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



    // Call loadGLTextureFromImageFile function
    marble_Texture = [self loadGLTextureFromImageFile : @"marble" :@"bmp"];


    // depth setting

    glClearDepthf(1.0f); // Compulsory
    glEnable(GL_DEPTH_TEST); // 8 test paike ek(depth) test enable kr
    glDepthFunc(GL_LEQUAL);

    //clear color

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    return 0;
}


- (GLuint)loadGLTextureFromImageFile:(NSString*)imageFileName : (NSString*)extension
{
    //code

    //local variable

    GLuint texture;

    //step 1

     // IOS Texture Change

    NSString* textureFileNameWithPath = [[NSBundle mainBundle]pathForResource:imageFileName ofType : extension];

    //step 2

    UIImage* image = [[UIImage alloc]initWithContentsOfFile:textureFileNameWithPath];

    //do error checking here

    if (image == nil)
    {
        printf("Image loading failed: %s\n", textureFileNameWithPath.UTF8String);
        [self uninitialize] ;
        [self release] ;
        exit(0);
        return 0;
    }     //step 3

    CGImageRef cgImage = [image CGImage];

    //step 4

    int imageWidth = (int)CGImageGetWidth(cgImage);

    int imageHeight = (int)CGImageGetHeight(cgImage);

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



    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

- (void)display
{
    //code

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //step 1: Use shader program object

    glUseProgram(shaderProgramObject);


    //transformations



    vmath::mat4 modelMatrix = vmath::mat4::identity();
    vmath::mat4 viewMatrix = vmath::mat4::identity();

    vmath::mat4 translationMatrix = vmath::mat4::identity();


    translationMatrix = vmath::translate(0.0f, 0.0f, -5.0f);

    vmath::mat4 scaleMatrix = vmath::mat4::identity();

    scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

    vmath::mat4 rotationMatrix1 = vmath::mat4::identity();

    rotationMatrix1 = vmath::rotate(rAngle, 1.0f, 0.0f, 0.0f);

    vmath::mat4 rotationMatrix2 = vmath::mat4::identity();

    rotationMatrix2 = vmath::rotate(rAngle, 0.0f, 1.0f, 0.0f);

    vmath::mat4 rotationMatrix3 = vmath::mat4::identity();

    rotationMatrix3 = vmath::rotate(rAngle, 0.0f, 0.0f, 1.0f);

    vmath::mat4 rotationMatrix = rotationMatrix1 * rotationMatrix2 * rotationMatrix3;

    modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

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

    //texture lines for Pyramid

    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, marble_Texture);

    glUniform1i(textureSamplerUniform, 0);

    glBindVertexArray(vao);

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

- (void)myUpdate
{
    //code
    rAngle = rAngle - 2.0f;

    if (rAngle <= 0.0f)
    {
        rAngle = 360.0f;
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

    // delete texcoord vbo
    if (vbo_Texcoord)
    {
        glDeleteBuffers(1, &vbo_Texcoord);
        vbo_Texcoord = 0;
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
    //code

    
    if (bAnimationEnable == false)
    {
        bAnimationEnable = true;
    }
    else
    {
        bAnimationEnable = false;
    }

}

- (void)OndoubleTap:(UITapGestureRecognizer*)gestureRecognizer
{
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
