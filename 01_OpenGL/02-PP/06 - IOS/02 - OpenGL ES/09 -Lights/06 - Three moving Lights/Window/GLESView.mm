



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

// Light Variables


/*GLfloat lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // white diffuse light
GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightPosition[] = { 100.0f, 100.0f, 100.0f, 1.0f };*/

GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat materialDiffuse[] = { 0.5f, 0.2f, 0.7f, 1.0f };
GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

//per vertex light properties



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


GLfloat materialShininess = 128.0f;

bool bLightingEnable = false;

bool select_SPO = false;



enum
{
    AMC_ATTRIBUTE_POSITION = 0,
    AMC_ATTRIBUTE_NORMAL = 1

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

    GLuint shaderProgramObject_PV;

    GLuint shaderProgramObject_PF;

    /*  GLuint vao;
      GLuint vbo_Position;*/

      GLuint mvpMatrixUniform;

      mat4 perspectiveProjectionMatrix;

      mesh sphere;


      // Uniforms per vertex

      GLuint modelMatrixUniform_PV;
      GLuint viewMatrixUniform_PV;
      GLuint projectionMatrixUniform_PV;

      GLuint lightDiffuseUniform_PV[3];
      GLuint lightAmbientUniform_PV[3];
      GLuint lightSpecularUniform_PV[3];
      GLuint lightPositionUniform_PV[3];

      GLuint materialDiffuseUniform_PV;
      GLuint materialAmbientUniform_PV;
      GLuint materialSpecularUniform_PV;
      GLuint materialShininessUniform_PV;


      GLuint keyPressUniform_PV ;

      // Uniforms for Per Fragment

        GLuint modelMatrixUniform_PF;
        GLuint viewMatrixUniform_PF;
        GLuint projectionMatrixUniform_PF;

        GLuint lightDiffuseUniform_PF[3];
        GLuint lightAmbientUniform_PF[3];
        GLuint lightSpecularUniform_PF[3];
        GLuint lightPositionUniform_PF[3];

        GLuint materialDiffuseUniform_PF;
        GLuint materialAmbientUniform_PF;
        GLuint materialSpecularUniform_PF;
        GLuint materialShininessUniform_PF;


        GLuint keyPressUniform_PF;



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

    const GLchar* vertexShaderSourceCode_PV =
        "#version 300 es\n"             //make verson to 300
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

    GLuint vertexShaderObject_PV = glCreateShader(GL_VERTEX_SHADER);

    //step 3 : Give vertex shader code to OpenGL
    glShaderSource(vertexShaderObject_PV, 1, (const GLchar**)&vertexShaderSourceCode_PV, NULL);

    //Step 4 : Let OpenGL engine Compile the vertex shader code
    glCompileShader(vertexShaderObject_PV);

    GLint status = 0;
    GLint infoLogLength = 0;
    GLchar* szInfoLog = NULL;

    //step 5 : Check for vertex shader compilation
    //step 5 (a) :  get compile status in status variable
    glGetShaderiv(vertexShaderObject_PV, GL_COMPILE_STATUS, &status);

    //step 5 (b) : if error occur i.e status == GL_FALSE,  get length of error message into infoLogLength variable
    if (status == GL_FALSE)
    {

        glGetShaderiv(vertexShaderObject_PV, GL_INFO_LOG_LENGTH, &infoLogLength);

        if (infoLogLength > 0)
        {
            //step 5 (c) : declare a string variable allocate memory equal to above length i.e length of error message
            szInfoLog = (GLchar*)malloc(infoLogLength);


            if (szInfoLog != NULL)
            {
                // step 5 (d) : get the compilation error log info into memory allocated string
                glGetShaderInfoLog(vertexShaderObject_PV, infoLogLength, NULL, szInfoLog);

                //step 5 (e) : print error message into log file
                printf("Vertex shader Per Vertex compilation error log : %s\n", szInfoLog);


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

    const GLchar* fragmentShaderSourceCode_PV =
        "#version 300 es\n"
        "\n"
        "precision highp float;\n"         // Specify precision for floating-point types
        "in vec3 oPhongADSLight;\n"
        "uniform highp int uKeyPress;\n"
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
    GLuint fragmentShaderObject_PV = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShaderObject_PV, 1, (const GLchar**)&fragmentShaderSourceCode_PV, NULL);

    glCompileShader(fragmentShaderObject_PV);

    status = 0;
    infoLogLength = 0;
    szInfoLog = NULL;

    glGetShaderiv(fragmentShaderObject_PV, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE)
    {

        glGetShaderiv(fragmentShaderObject_PV, GL_INFO_LOG_LENGTH, &infoLogLength);

        if (infoLogLength > 0)
        {
            szInfoLog = (GLchar*)malloc(infoLogLength);


            if (szInfoLog != NULL)
            {
                glGetShaderInfoLog(fragmentShaderObject_PV, infoLogLength, NULL, szInfoLog);
                printf("Fragment shader Per Vertex compilation error log : %s\n", szInfoLog);
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

    shaderProgramObject_PV = glCreateProgram();

    //step 8 : Attach both shaders into this program            also know as prelinking binding  should be done before linking and after attachment
    glAttachShader(shaderProgramObject_PV, vertexShaderObject_PV);
    glAttachShader(shaderProgramObject_PV, fragmentShaderObject_PV);

    //step 9: bind attribute location to the shader program object

    glBindAttribLocation(shaderProgramObject_PV, AMC_ATTRIBUTE_POSITION, "aPosition");

    glBindAttribLocation(shaderProgramObject_PV, AMC_ATTRIBUTE_NORMAL, "aNormal");



    //step 10: Link the shader program

    glLinkProgram(shaderProgramObject_PV);

    //step 11: do linking error check fpr shader program

    status = 0;
    infoLogLength = 0;
    szInfoLog = NULL;

    glGetProgramiv(shaderProgramObject_PV, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObject_PV, GL_INFO_LOG_LENGTH, &infoLogLength);

        if (infoLogLength > 0)
        {
            szInfoLog = (GLchar*)malloc(infoLogLength);


            if (szInfoLog != NULL)
            {
                glGetProgramInfoLog(shaderProgramObject_PV, infoLogLength, NULL, szInfoLog);
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

   // mvpMatrixUniform = glGetUniformLocation(shaderProgramObject_PV, "uMVPMatrix");  //must be done after linkage of shader program

    modelMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "uModelMatrix");
    
    viewMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "uViewMatrix");
    
    projectionMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "uProjectionMatrix");
    
    lightAmbientUniform_PV[0] = glGetUniformLocation(shaderProgramObject_PV, "ulightAmbient[0]");
    
    lightDiffuseUniform_PV[0] = glGetUniformLocation(shaderProgramObject_PV, "ulightDiffuse[0]");
    
    lightSpecularUniform_PV[0] = glGetUniformLocation(shaderProgramObject_PV, "ulightSpecular[0]");
    
    lightPositionUniform_PV[0] = glGetUniformLocation(shaderProgramObject_PV, "ulightPosition[0]");
    
    
    lightAmbientUniform_PV[1] = glGetUniformLocation(shaderProgramObject_PV, "ulightAmbient[1]");
    
    lightDiffuseUniform_PV[1] = glGetUniformLocation(shaderProgramObject_PV, "ulightDiffuse[1]");
    
    lightSpecularUniform_PV[1] = glGetUniformLocation(shaderProgramObject_PV, "ulightSpecular[1]");
    
    lightPositionUniform_PV[1] = glGetUniformLocation(shaderProgramObject_PV, "ulightPosition[1]");
    
    lightAmbientUniform_PV[2] = glGetUniformLocation(shaderProgramObject_PV, "ulightAmbient[2]");
    
    lightDiffuseUniform_PV[2] = glGetUniformLocation(shaderProgramObject_PV, "ulightDiffuse[2]");
    
    lightSpecularUniform_PV[2] = glGetUniformLocation(shaderProgramObject_PV, "ulightSpecular[2]");
    
    lightPositionUniform_PV[2] = glGetUniformLocation(shaderProgramObject_PV, "ulightPosition[2]");
    
    
    materialAmbientUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "uMaterialAmbient");
    
    materialDiffuseUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "uMaterialDiffuse");
    
    
    materialSpecularUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "uMaterialSpecular");
    
    materialShininessUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "uMaterialShininess");
    
    keyPressUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "uKeyPress");
   
     // **************************** Shader Program Object For Per Fragment Lighting *****************

      //vertex shader

        const GLchar* vertexShaderSourceCode_PF =
            "#version 300 es\n"              // (version of GLSL * 100) core means you are using core profile i.e PP
            "\n"
            "in vec4 aPosition;\n"
            "in vec3 aNormal;\n"
            "uniform mat4 uModelMatrix;\n"
            "uniform mat4 uViewMatrix;\n"
            "uniform mat4 uProjectionMatrix;\n"
            "uniform vec4 ulightPosition[3];\n"
            "uniform highp int uKeyPress;\n"
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
                    printf("Vertex shader per fragment compilation error log : %s\n", szInfoLog);


                    //step 5 (f) : free allocated memory
                    free(szInfoLog);
                    szInfoLog = NULL;


                    //step 5 (g) : uninitialize and exit.


                }

            }

            [self uninitialize];
            [self release];
           exit(0);
            
        }

         //fragment shader

        const GLchar* fragmentShaderSourceCode_PF =
            "#version 300 es\n"
            "\n"
            "precision highp float;\n"
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
            "uniform highp int uKeyPress;\n"
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
                    printf( "Fragment shader Per fragment shader compilation error log : %s\n", szInfoLog);
                    free(szInfoLog);
                    szInfoLog = NULL;


                }



            }

            [self uninitialize];
            [self release];
            exit(0);

        }

         //linking process for shader program.

        // shader program

        //step 7 : create shader program object

        shaderProgramObject_PF = glCreateProgram();

        //step 8 : Attach both shaders into this program            also know as prelinking binding  should be done before linking and after attachment
        glAttachShader(shaderProgramObject_PF, vertexShaderObject_PF);
        glAttachShader(shaderProgramObject_PF, fragmentShaderObject_PF);

        //step 9: bind attribute location to the shader program object

        glBindAttribLocation(shaderProgramObject_PF, AMC_ATTRIBUTE_POSITION, "aPosition");

        glBindAttribLocation(shaderProgramObject_PF, AMC_ATTRIBUTE_NORMAL, "aNormal");

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
                    glGetProgramInfoLog(shaderProgramObject_PF, infoLogLength, NULL, szInfoLog);
                    printf("per fragment shader program linking error log : %s\n", szInfoLog);
                    free(szInfoLog);
                    szInfoLog = NULL;


                }



            }

            [self uninitialize];
            [self release];
            exit(0);

        }
    
    // Get shader uniform location
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



    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

- (void)display
{
    //code

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //step 1: Use shader program object

    if(select_SPO == false)
    {
        glUseProgram(shaderProgramObject_PV);
    }
    else
    {
        glUseProgram(shaderProgramObject_PF);
    }

    


    //transformations

    mat4 translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -4.0f);

    mat4 modelMatrix = translationMatrix;

    mat4 viewMatrix = mat4::identity();

    //push above mvp into vertex shader's mvp uniform

    if(select_SPO == false)
    {
         glUniformMatrix4fv(modelMatrixUniform_PV, 1, GL_FALSE, modelMatrix);


        glUniformMatrix4fv(viewMatrixUniform_PV, 1, GL_FALSE, viewMatrix);


        glUniformMatrix4fv(projectionMatrixUniform_PV, 1, GL_FALSE, perspectiveProjectionMatrix);
    }

    else
    {
         glUniformMatrix4fv(modelMatrixUniform_PF, 1, GL_FALSE, modelMatrix);


        glUniformMatrix4fv(viewMatrixUniform_PF, 1, GL_FALSE, viewMatrix);


        glUniformMatrix4fv(projectionMatrixUniform_PF, 1, GL_FALSE, perspectiveProjectionMatrix);
    }

   

    if (bLightingEnable == true)
    {
        if(select_SPO == false)
        {
             //glUniform1i(keyPressUniform_PV, 1);

           glUniform1i(keyPressUniform_PV, 1);
                
                glUniform3fv(lightAmbientUniform_PV[0], 1, light1Ambient);
                glUniform3fv(lightDiffuseUniform_PV[0], 1, light1Diffuse);
                glUniform3fv(lightSpecularUniform_PV[0], 1, light1Specular);
                glUniform4fv(lightPositionUniform_PV[0], 1, light1Position);
                
                glUniform3fv(lightAmbientUniform_PV[1], 1, light2Ambient);
                glUniform3fv(lightDiffuseUniform_PV[1], 1, light2Diffuse);
                glUniform3fv(lightSpecularUniform_PV[1], 1, light2Specular);
                glUniform4fv(lightPositionUniform_PV[1], 1, light2Position);
                
                glUniform3fv(lightAmbientUniform_PV[2], 1, light3Ambient);
                glUniform3fv(lightDiffuseUniform_PV[2], 1, light3Diffuse);
                glUniform3fv(lightSpecularUniform_PV[2], 1, light3Specular);
                glUniform4fv(lightPositionUniform_PV[2], 1, light3Position);
                
                glUniform3fv(materialAmbientUniform_PV, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform_PV, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform_PV, 1, materialSpecular);
                glUniform1f(materialShininessUniform_PV, materialShininess);
        }
        else
        {
            //glUniform1i(keyPressUniform_PF, 1);
           
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
        glUniform1i(keyPressUniform_PV, 0);
        glUniform1i(keyPressUniform_PF, 0);
    }



    sphere.draw();

    //step 5: Unuse shader program object
    glUseProgram(0);

}

- (void)myUpdate
{
    //code

    if (bLightingEnable == true)
        {
            if(select_SPO == false)
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
            
            if(select_SPO == true)
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

- (void)uninitialize
{
    //code


    //steps for uninitialize function in PP

    if (shaderProgramObject_PV)
    {
        //step 1 : use shader program object

        glUseProgram(shaderProgramObject_PV);

        GLint numShaders = 0;

        //step 2 : get number of attached shaders
        glGetProgramiv(shaderProgramObject_PV, GL_ATTACHED_SHADERS, &numShaders);



        if (numShaders > 0)
        {
            //step 3 : Create and allocate memory of shader objects according to number of attached shaders
            GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint));

            if (pShaders != NULL)
            {
                //step 4 : Get shader object into the allocated array.

                glGetAttachedShaders(shaderProgramObject_PV, numShaders, NULL, pShaders);

                //step 5 : Detach and delete shader objects iteratively

                for (GLint i = 0; i < numShaders; i++)
                {
                    glDetachShader(shaderProgramObject_PV, pShaders[i]);
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

        glDeleteProgram(shaderProgramObject_PV);
        shaderProgramObject_PV = 0;






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

    if (bLightingEnable == false)
    {
        bLightingEnable = true;
    }
    else
    {
        bLightingEnable = false;
    }


}

- (void)OndoubleTap:(UITapGestureRecognizer*)gestureRecognizer
{
    if(select_SPO == false)
    {
        select_SPO = true;
    }
    else
    {
        select_SPO = false;
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
