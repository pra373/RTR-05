



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

#define FBO_WIDTH 512
#define FBO_HEIGHT 512

// Light Variables


GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat materialDiffuse[] = { 0.5f, 0.2f, 0.7f, 1.0f };
GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

//per vertex light properties



GLfloat light1Ambient[3] = { 0.0, 0.0, 0.0 };
GLfloat light2Ambient[3] = { 0.0, 0.0, 0.0 };
GLfloat light1Diffuse[3] = { 1.0, 0.0, 0.0 };
GLfloat light2Diffuse[3] = { 0.0, 0.0, 1.0 };
GLfloat light1Specular[3] = { 1.0, 0.0, 0.0 };
GLfloat light2Specular[3] = { 0.0, 0.0, 1.0 };
GLfloat light1Position[4] = { -2.0, 0.0, 1.0, 1.0 };
GLfloat light2Position[4] = { 2.0, 0.0, 1.0, 1.0 };

GLfloat light3Ambient[3] = { 0.0, 0.0, 0.0 };
GLfloat light3Diffuse[3] = { 0.0, 1.0, 0.0 };
GLfloat light3Specular[3] = { 0.0, 1.0, 0.0 };
GLfloat light3Position[4] = { 0.0, 0.0, 1.0, 1.0 };

//per fragment lights

GLfloat light1Ambient_F[3] = { 0.0, 0.0, 0.0 };

GLfloat light2Ambient_F[3] = { 0.0, 0.0, 0.0 };

GLfloat light3Ambient_F[3] = { 0.0, 0.0, 0.0 };



GLfloat light1Diffuse_F[3] = { 1.0, 0.0, 0.0 };

GLfloat light2Diffuse_F[3] = { 0.0, 1.0, 0.0 };

GLfloat light3Diffuse_F[3] = { 0.0, 0.0, 1.0 };



GLfloat light1Specular_F[3] = { 1.0, 0.0, 0.0 };

GLfloat light2Specular_F[3] = { 0.0, 1.0, 0.0 };

GLfloat light3Specular_F[3] = { 0.0, 0.0, 1.0 };



GLfloat light1Position_F[4] = { -2.0, 0.0, 1.0, 1.0 };

GLfloat light2Position_F[4] = { 2.0, 0.0, 1.0, 1.0 };

GLfloat light3Position_F[4] = { 0.0, 0.0, 1.0, 1.0 };

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

bool cubeAnimationEnable = false;

int count = 0;

enum
{
    AMC_ATTRIBUTE_POSITION = 0,
    AMC_ATTRIBUTE_TEXCOORD = 1,
    AMC_ATTRIBUTE_NORMAL = 2

};

bool bFBOResult = false;

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

    GLuint shaderProgramObject_Cube;

    GLuint vao_Cube;
    GLuint vbo_Position_Cube;
    GLuint vbo_Texcoord_Cube;

    GLfloat angle_Cube;



    GLuint textureSamplerUniform_Cube;

    GLuint mvpMatrixUniform_Cube;

    mat4 perspectiveProjectionMatrix_Cube;

    GLint winWidth;
    GLint winHeight;

    // Sphere related global variables
     // OpenGL related field variables

    GLuint shaderProgramObject_PV_Sphere;

    GLuint shaderProgramObject_PF_Sphere;



    mat4 perspectiveProjectionMatrix_Sphere;

    mesh sphere;


    // Uniforms per vertex

    GLuint modelMatrixUniform_PV_Sphere;
    GLuint viewMatrixUniform_PV_Sphere;
    GLuint projectionMatrixUniform_PV_Sphere;

    GLuint lightDiffuseUniform_PV_Sphere[3];
    GLuint lightAmbientUniform_PV_Sphere[3];
    GLuint lightSpecularUniform_PV_Sphere[3];
    GLuint lightPositionUniform_PV_Sphere[3];

    GLuint materialDiffuseUniform_PV_Sphere;
    GLuint materialAmbientUniform_PV_Sphere;
    GLuint materialSpecularUniform_PV_Sphere;
    GLuint materialShininessUniform_PV_Sphere;


    GLuint keyPressUniform_PV_Sphere;

    // Uniforms for Per Fragment

    GLuint modelMatrixUniform_PF_Sphere;
    GLuint viewMatrixUniform_PF_Sphere;
    GLuint projectionMatrixUniform_PF_Sphere;

    GLuint lightDiffuseUniform_PF_Sphere[3];
    GLuint lightAmbientUniform_PF_Sphere[3];
    GLuint lightSpecularUniform_PF_Sphere[3];
    GLuint lightPositionUniform_PF_Sphere[3];

    GLuint materialDiffuseUniform_PF_Sphere;
    GLuint materialAmbientUniform_PF_Sphere;
    GLuint materialSpecularUniform_PF_Sphere;
    GLuint materialShininessUniform_PF_Sphere;


    GLuint keyPressUniform_PF_Sphere;


    // FBO realted global variables

    GLuint kundali_Texture;

    GLuint fbo;
    GLuint rbo;
    GLuint texture_FBO;


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
            [self uninitialize_Cube] ;
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
            [self uninitialize_Cube] ;
            [self release] ;
            exit(0);
        }

        //initialize frames per second

        framesPerSecond = 60;  //value 60 is recomended from ios 8.2

        //initialize display link existance variable

        isDisplayLink = NO;



        //call our initialize method

        int result = [self initialize_Cube];

        if (result != 0)
        {
            printf("initialize cube function failed !!");
            [self uninitialize_Cube] ;
            [self release] ;
            exit(0);
        }

        //call our initialize method

      //int result = [self initialize];
        /*bool result = [self initialize_Sphere : width height : height];


        if (result != 0)
        {
            printf("initialize Sphere function failed !!");
            [self uninitialize_Sphere] ;
            [self release] ;
            exit(0);
        }*/

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

    [self display_Cube];

    //call update

    if (cubeAnimationEnable == true)
    {
        [self myUpdate_Cube] ;
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
        [self uninitialize_Cube] ;
        [self release] ;
        exit(0);
    }

    //call our resize here

    [self resize_Cube : width height : height];

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


- (int)initialize_Cube
{
    //code

    [self printGLInfo] ;   // copy paste point

    //vertex shader

    const GLchar* vertexShaderSourceCode =
        "#version 300 es\n"             //make verson to 300
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
                printf("Vertex shader compilation error log : %s\n", szInfoLog);


                //step 5 (f) : free allocated memory
                free(szInfoLog);
                szInfoLog = NULL;


                //step 5 (g) : uninitialize and exit.


            }

        }

        [self uninitialize_Cube];
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
        "out vec4 FragColor;\n"
        "in vec2 oTexCoord;\n"
        "uniform sampler2D uTextureSampler;\n"
        "void main(void)\n"
        "{\n"
        "  FragColor = texture(uTextureSampler, oTexCoord);\n"
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

        [self uninitialize_Cube];
        [self release] ;
        exit(-1);

    }

    //linking process for shader program.

    // shader program

    //step 7 : create shader program object

    shaderProgramObject_Cube = glCreateProgram();

    //step 8 : Attach both shaders into this program            also know as prelinking binding  should be done before linking and after attachment
    glAttachShader(shaderProgramObject_Cube, vertexShaderObject);
    glAttachShader(shaderProgramObject_Cube, fragmentShaderObject);

    //step 9: bind attribute location to the shader program object

    glBindAttribLocation(shaderProgramObject_Cube, AMC_ATTRIBUTE_POSITION, "aPosition");

    glBindAttribLocation(shaderProgramObject_Cube, AMC_ATTRIBUTE_TEXCOORD, "aTexCoord");


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
                printf("shader program linking error log : %s\n", szInfoLog);
                free(szInfoLog);
                szInfoLog = NULL;


            }



        }

        [self uninitialize_Cube];
        [self release] ;
        exit(-1);

    }

    //step 12: Declare position and color arrays

    //get Shader uniform Locations

    mvpMatrixUniform_Cube = glGetUniformLocation(shaderProgramObject_Cube, "uMVPMatrix");  //must be done after linkage of shader program

    textureSamplerUniform_Cube = glGetUniformLocation(shaderProgramObject_Cube, "uTextureSampler");

    const GLfloat cube_Position[] = {
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

    const GLfloat cube_Texcoord[] =
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

    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_Position), cube_Position, GL_STATIC_DRAW);

    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

    glBindBuffer(GL_ARRAY_BUFFER, 0);


    //step 17: Repete VBO position steps for VBO color


    //VBO for Texcoord

    glGenBuffers(1, &vbo_Texcoord_Cube);

    //step 16: Bind with VBO of Texcoord

    glBindBuffer(GL_ARRAY_BUFFER, vbo_Texcoord_Cube);

    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_Texcoord), cube_Texcoord, GL_STATIC_DRAW);

    glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //step 18 : unbind with VAO
    glBindVertexArray(0);


    // Call loadGLTextureFromImageFile function
    //kundali_Texture = [self loadGLTextureFromImageFile : @"Kundali" :@"bmp"];


    // depth setting

    glClearDepthf(1.0f); // Compulsory
    glEnable(GL_DEPTH_TEST); // 8 test paike ek(depth) test enable kr
    glDepthFunc(GL_LEQUAL);

    //clear color

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    //FBO related code

    if ([self createFBO : FBO_WIDTH height : FBO_HEIGHT] == true)
    {
        bFBOResult = [self initialize_Sphere : FBO_WIDTH height : FBO_HEIGHT];
    }

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

- (bool)initialize_Sphere:(GLint)textureWidth height : (GLint)textureHeight
{
    //code

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

        [self uninitialize_Sphere];
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

        [self uninitialize_Sphere];
        [self release] ;
        exit(-1);

    }

    //linking process for shader program.

    // shader program

    //step 7 : create shader program object

    shaderProgramObject_PV_Sphere = glCreateProgram();

    //step 8 : Attach both shaders into this program            also know as prelinking binding  should be done before linking and after attachment
    glAttachShader(shaderProgramObject_PV_Sphere, vertexShaderObject_PV);
    glAttachShader(shaderProgramObject_PV_Sphere, fragmentShaderObject_PV);

    //step 9: bind attribute location to the shader program object

    glBindAttribLocation(shaderProgramObject_PV_Sphere, AMC_ATTRIBUTE_POSITION, "aPosition");

    glBindAttribLocation(shaderProgramObject_PV_Sphere, AMC_ATTRIBUTE_NORMAL, "aNormal");



    //step 10: Link the shader program

    glLinkProgram(shaderProgramObject_PV_Sphere);

    //step 11: do linking error check fpr shader program

    status = 0;
    infoLogLength = 0;
    szInfoLog = NULL;

    glGetProgramiv(shaderProgramObject_PV_Sphere, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObject_PV_Sphere, GL_INFO_LOG_LENGTH, &infoLogLength);

        if (infoLogLength > 0)
        {
            szInfoLog = (GLchar*)malloc(infoLogLength);


            if (szInfoLog != NULL)
            {
                glGetProgramInfoLog(shaderProgramObject_PV_Sphere, infoLogLength, NULL, szInfoLog);
                printf("shader program linking error log : %s\n", szInfoLog);
                free(szInfoLog);
                szInfoLog = NULL;


            }



        }

        [self uninitialize_Sphere];
        [self release] ;
        exit(-1);

    }

    //step 12: Declare position and color arrays

    //get Shader uniform Locations

   // mvpMatrixUniform = glGetUniformLocation(shaderProgramObject_PV, "uMVPMatrix");  //must be done after linkage of shader program

    modelMatrixUniform_PV_Sphere = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uModelMatrix");

    viewMatrixUniform_PV_Sphere = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uViewMatrix");

    projectionMatrixUniform_PV_Sphere = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uProjectionMatrix");

    lightAmbientUniform_PV_Sphere[0] = glGetUniformLocation(shaderProgramObject_PV_Sphere, "ulightAmbient[0]");

    lightDiffuseUniform_PV_Sphere[0] = glGetUniformLocation(shaderProgramObject_PV_Sphere, "ulightDiffuse[0]");

    lightSpecularUniform_PV_Sphere[0] = glGetUniformLocation(shaderProgramObject_PV_Sphere, "ulightSpecular[0]");

    lightPositionUniform_PV_Sphere[0] = glGetUniformLocation(shaderProgramObject_PV_Sphere, "ulightPosition[0]");


    lightAmbientUniform_PV_Sphere[1] = glGetUniformLocation(shaderProgramObject_PV_Sphere, "ulightAmbient[1]");

    lightDiffuseUniform_PV_Sphere[1] = glGetUniformLocation(shaderProgramObject_PV_Sphere, "ulightDiffuse[1]");

    lightSpecularUniform_PV_Sphere[1] = glGetUniformLocation(shaderProgramObject_PV_Sphere, "ulightSpecular[1]");

    lightPositionUniform_PV_Sphere[1] = glGetUniformLocation(shaderProgramObject_PV_Sphere, "ulightPosition[1]");

    lightAmbientUniform_PV_Sphere[2] = glGetUniformLocation(shaderProgramObject_PV_Sphere, "ulightAmbient[2]");

    lightDiffuseUniform_PV_Sphere[2] = glGetUniformLocation(shaderProgramObject_PV_Sphere, "ulightDiffuse[2]");

    lightSpecularUniform_PV_Sphere[2] = glGetUniformLocation(shaderProgramObject_PV_Sphere, "ulightSpecular[2]");

    lightPositionUniform_PV_Sphere[2] = glGetUniformLocation(shaderProgramObject_PV_Sphere, "ulightPosition[2]");


    materialAmbientUniform_PV_Sphere = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uMaterialAmbient");

    materialDiffuseUniform_PV_Sphere = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uMaterialDiffuse");


    materialSpecularUniform_PV_Sphere = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uMaterialSpecular");

    materialShininessUniform_PV_Sphere = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uMaterialShininess");

    keyPressUniform_PV_Sphere = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uKeyPress");

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

        [self uninitialize_Sphere];
        [self release] ;
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
                printf("Fragment shader Per fragment shader compilation error log : %s\n", szInfoLog);
                free(szInfoLog);
                szInfoLog = NULL;


            }



        }

        [self uninitialize_Sphere];
        [self release] ;
        exit(0);

    }

    //linking process for shader program.

   // shader program

   //step 7 : create shader program object

    shaderProgramObject_PF_Sphere = glCreateProgram();

    //step 8 : Attach both shaders into this program            also know as prelinking binding  should be done before linking and after attachment
    glAttachShader(shaderProgramObject_PF_Sphere, vertexShaderObject_PF);
    glAttachShader(shaderProgramObject_PF_Sphere, fragmentShaderObject_PF);

    //step 9: bind attribute location to the shader program object

    glBindAttribLocation(shaderProgramObject_PF_Sphere, AMC_ATTRIBUTE_POSITION, "aPosition");

    glBindAttribLocation(shaderProgramObject_PF_Sphere, AMC_ATTRIBUTE_NORMAL, "aNormal");

    //step 10: Link the shader program

    glLinkProgram(shaderProgramObject_PF_Sphere);

    //step 11: do linking error check fpr shader program

    status = 0;
    infoLogLength = 0;
    szInfoLog = NULL;

    glGetProgramiv(shaderProgramObject_PF_Sphere, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObject_PF_Sphere, GL_INFO_LOG_LENGTH, &infoLogLength);

        if (infoLogLength > 0)
        {
            szInfoLog = (GLchar*)malloc(infoLogLength);


            if (szInfoLog != NULL)
            {
                glGetProgramInfoLog(shaderProgramObject_PF_Sphere, infoLogLength, NULL, szInfoLog);
                printf("per fragment shader program linking error log : %s\n", szInfoLog);
                free(szInfoLog);
                szInfoLog = NULL;


            }



        }

        [self uninitialize_Sphere];
        [self release] ;
        exit(0);

    }

    // Get shader uniform location
    modelMatrixUniform_PF_Sphere = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uModelMatrix");

    viewMatrixUniform_PF_Sphere = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uViewMatrix");

    projectionMatrixUniform_PF_Sphere = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uProjectionMatrix");

    lightAmbientUniform_PF_Sphere[0] = glGetUniformLocation(shaderProgramObject_PF_Sphere, "ulightAmbient[0]");

    lightDiffuseUniform_PF_Sphere[0] = glGetUniformLocation(shaderProgramObject_PF_Sphere, "ulightDiffuse[0]");

    lightSpecularUniform_PF_Sphere[0] = glGetUniformLocation(shaderProgramObject_PF_Sphere, "ulightSpecular[0]");

    lightPositionUniform_PF_Sphere[0] = glGetUniformLocation(shaderProgramObject_PF_Sphere, "ulightPosition[0]");

    lightAmbientUniform_PF_Sphere[1] = glGetUniformLocation(shaderProgramObject_PF_Sphere, "ulightAmbient[1]");

    lightDiffuseUniform_PF_Sphere[1] = glGetUniformLocation(shaderProgramObject_PF_Sphere, "ulightDiffuse[1]");

    lightSpecularUniform_PF_Sphere[1] = glGetUniformLocation(shaderProgramObject_PF_Sphere, "ulightSpecular[1]");

    lightPositionUniform_PF_Sphere[1] = glGetUniformLocation(shaderProgramObject_PF_Sphere, "ulightPosition[1]");

    lightAmbientUniform_PF_Sphere[2] = glGetUniformLocation(shaderProgramObject_PF_Sphere, "ulightAmbient[2]");

    lightDiffuseUniform_PF_Sphere[2] = glGetUniformLocation(shaderProgramObject_PF_Sphere, "ulightDiffuse[2]");

    lightSpecularUniform_PF_Sphere[2] = glGetUniformLocation(shaderProgramObject_PF_Sphere, "ulightSpecular[2]");

    lightPositionUniform_PF_Sphere[2] = glGetUniformLocation(shaderProgramObject_PF_Sphere, "ulightPosition[2]");


    materialAmbientUniform_PF_Sphere = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uMaterialAmbient");

    materialDiffuseUniform_PF_Sphere = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uMaterialDiffuse");


    materialSpecularUniform_PF_Sphere = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uMaterialSpecular");

    materialShininessUniform_PF_Sphere = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uMaterialShininess");

    keyPressUniform_PF_Sphere = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uKeyPress");









    // Make sphere
    makeSphere(sphere, 0.85, 40, 40);

    // depth setting

    glClearDepthf(1.0f); // Compulsory
    glEnable(GL_DEPTH_TEST); // 8 test paike ek(depth) test enable kr
    glDepthFunc(GL_LEQUAL);

    //clear color

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    return true;
}


- (bool)createFBO:(GLint)textureWidth height : (GLint)textureHeight
{
    //local variables
    GLint maxRenderBufferSize = 0;

    //check capacity of render buffer

    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderBufferSize);

    if (maxRenderBufferSize < textureWidth || maxRenderBufferSize < textureHeight)
    {
        printf("Texture size overflow\n");
        return(false);
    }

    //create custom frame buffer

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    //create texture for FBO in which we are going to render Scene 2 (Sphere)

    glGenTextures(1, &texture_FBO);
    glBindTexture(GL_TEXTURE_2D, texture_FBO);
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

    glBindRenderbuffer(GL_RENDERBUFFER, rbo);  //GL_DRAW_RENDERBUFFER also can be used

    //set the storage of above render buffer for texture size for depth

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, textureWidth, textureHeight);

    //attach above depth related render buffer to FBO

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    //check the frame buffer status weather successfull or not

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("Framebuffer creation status is not complete\n\n");
        return(false);
    }

    //unbind with frame buffer

   // glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return(true);
}



-(void)resize_Cube:(int)width height : (int)height
{
    //code

    if (height <= 0)
    {
        height = 1;
    }


    winWidth = width;
    winHeight = height;

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    perspectiveProjectionMatrix_Cube = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

- (void)resize_Sphere:(int)width height : (int)height
{
    //code

    if (height <= 0)
    {
        height = 1;
    }



    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    perspectiveProjectionMatrix_Sphere = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

}

- (void)display_Cube
{
    //code

    if (bFBOResult == true)  // No need to compare with 'true'
    {
        [self display_Sphere : FBO_WIDTH height : FBO_HEIGHT] ;

        /*
        if (bAnimationEnable)  // No need to compare with 'true'
        {
            [self myUpdate_Sphere];
        }
        */

        [self myUpdate_Sphere];

    }


    // Corrected method call syntax
    [self resize_Cube : winWidth height : winHeight];

    glBindFramebuffer(GL_FRAMEBUFFER, customeFrameBuffer);

    //reset the color to white to companset the change done by display sphere

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Code

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //step 1: Use shader program object

    glUseProgram(shaderProgramObject_Cube);


    //transformations

    mat4 modelViewMatrix = vmath::translate(0.0f, 0.0f, -5.0f);

    mat4 rotationMatrixX = mat4::identity();
    mat4 rotationMatrixY = mat4::identity();
    mat4 rotationMatrixZ = mat4::identity();
    mat4 finalRotationMatrix = mat4::identity();

    rotationMatrixX = vmath::rotate(angle_Cube, 1.0f, 0.0f, 0.0f);
    rotationMatrixY = vmath::rotate(angle_Cube, 0.0f, 1.0f, 0.0f);
    rotationMatrixZ = vmath::rotate(angle_Cube, 0.0f, 0.0f, 1.0f);

    finalRotationMatrix = rotationMatrixX * rotationMatrixY * rotationMatrixZ;

    modelViewMatrix = modelViewMatrix * finalRotationMatrix;

    mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix_Cube * modelViewMatrix;  //order of multiplication is not commutativ. i.e mat1 * mat2 != mat2 * mat1

    //push above mvp into vertex shader's mvp uniform

    glUniformMatrix4fv(mvpMatrixUniform_Cube, 1, GL_FALSE, modelViewProjectionMatrix);

    //texture lines for Pyramid

    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, texture_FBO);

    glUniform1i(textureSamplerUniform_Cube, 0);

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

- (void)display_Sphere:(GLint)textureWidth height : (GLint)textureHeight
{
    //code

    //bind with FBO

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);      //step 1: Use shader program object

    //call resize_Sphere()
    [self resize_Sphere : textureWidth height : textureHeight] ;

    //set the clear color to back again to companset the change done by display_Cube()

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Code

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //step 1: Use shader program object

    if (select_SPO == false)
    {
        glUseProgram(shaderProgramObject_PV_Sphere);
    }
    else
    {
        glUseProgram(shaderProgramObject_PF_Sphere);
    }




    //transformations

    mat4 translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -4.0f);

    mat4 modelMatrix = translationMatrix;

    mat4 viewMatrix = mat4::identity();

    //push above mvp into vertex shader's mvp uniform

    if (select_SPO == false)
    {
        glUniformMatrix4fv(modelMatrixUniform_PV_Sphere, 1, GL_FALSE, modelMatrix);


        glUniformMatrix4fv(viewMatrixUniform_PV_Sphere, 1, GL_FALSE, viewMatrix);


        glUniformMatrix4fv(projectionMatrixUniform_PV_Sphere, 1, GL_FALSE, perspectiveProjectionMatrix_Sphere);
    }

    else
    {
        glUniformMatrix4fv(modelMatrixUniform_PF_Sphere, 1, GL_FALSE, modelMatrix);


        glUniformMatrix4fv(viewMatrixUniform_PF_Sphere, 1, GL_FALSE, viewMatrix);


        glUniformMatrix4fv(projectionMatrixUniform_PF_Sphere, 1, GL_FALSE, perspectiveProjectionMatrix_Sphere);
    }



    if (bLightingEnable == true)
    {
        if (select_SPO == false)
        {
            //glUniform1i(keyPressUniform_PV, 1);

            glUniform1i(keyPressUniform_PV_Sphere, 1);

            glUniform3fv(lightAmbientUniform_PV_Sphere[0], 1, light1Ambient);
            glUniform3fv(lightDiffuseUniform_PV_Sphere[0], 1, light1Diffuse);
            glUniform3fv(lightSpecularUniform_PV_Sphere[0], 1, light1Specular);
            glUniform4fv(lightPositionUniform_PV_Sphere[0], 1, light1Position);

            glUniform3fv(lightAmbientUniform_PV_Sphere[1], 1, light2Ambient);
            glUniform3fv(lightDiffuseUniform_PV_Sphere[1], 1, light2Diffuse);
            glUniform3fv(lightSpecularUniform_PV_Sphere[1], 1, light2Specular);
            glUniform4fv(lightPositionUniform_PV_Sphere[1], 1, light2Position);

            glUniform3fv(lightAmbientUniform_PV_Sphere[2], 1, light3Ambient);
            glUniform3fv(lightDiffuseUniform_PV_Sphere[2], 1, light3Diffuse);
            glUniform3fv(lightSpecularUniform_PV_Sphere[2], 1, light3Specular);
            glUniform4fv(lightPositionUniform_PV_Sphere[2], 1, light3Position);

            glUniform3fv(materialAmbientUniform_PV_Sphere, 1, materialAmbient);
            glUniform3fv(materialDiffuseUniform_PV_Sphere, 1, materialDiffuse);
            glUniform3fv(materialSpecularUniform_PV_Sphere, 1, materialSpecular);
            glUniform1f(materialShininessUniform_PV_Sphere, materialShininess);
        }
        else
        {
            //glUniform1i(keyPressUniform_PF, 1);

            glUniform1i(keyPressUniform_PF_Sphere, 1);

            glUniform3fv(lightAmbientUniform_PF_Sphere[0], 1, light1Ambient_F);
            glUniform3fv(lightDiffuseUniform_PF_Sphere[0], 1, light1Diffuse_F);
            glUniform3fv(lightSpecularUniform_PF_Sphere[0], 1, light1Specular_F);
            glUniform4fv(lightPositionUniform_PF_Sphere[0], 1, light1Position_F);

            glUniform3fv(lightAmbientUniform_PF_Sphere[1], 1, light2Ambient_F);
            glUniform3fv(lightDiffuseUniform_PF_Sphere[1], 1, light2Diffuse_F);
            glUniform3fv(lightSpecularUniform_PF_Sphere[1], 1, light2Specular_F);
            glUniform4fv(lightPositionUniform_PF_Sphere[1], 1, light2Position_F);

            glUniform3fv(lightAmbientUniform_PF_Sphere[2], 1, light3Ambient_F);
            glUniform3fv(lightDiffuseUniform_PF_Sphere[2], 1, light3Diffuse_F);
            glUniform3fv(lightSpecularUniform_PF_Sphere[2], 1, light3Specular_F);
            glUniform4fv(lightPositionUniform_PF_Sphere[2], 1, light3Position_F);

            glUniform3fv(materialAmbientUniform_PF_Sphere, 1, materialAmbient);
            glUniform3fv(materialDiffuseUniform_PF_Sphere, 1, materialDiffuse);
            glUniform3fv(materialSpecularUniform_PF_Sphere, 1, materialSpecular);
            glUniform1f(materialShininessUniform_PF_Sphere, materialShininess);
        }


    }
    else
    {
        glUniform1i(keyPressUniform_PV_Sphere, 0);
        glUniform1i(keyPressUniform_PF_Sphere, 0);
    }



    sphere.draw();

    //step 5: Unuse shader program object
    glUseProgram(0);


    //unbind with the frame buffer

   // glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


- (void)myUpdate_Cube
{
    //code
    angle_Cube = angle_Cube - 2.0f;

    if (angle_Cube <= 0.0f)
    {
        angle_Cube = 360.0f;
    }

}

- (void)myUpdate_Sphere
{
    //code

    if (bLightingEnable == true)
    {
        if (select_SPO == false)
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

        if (select_SPO == true)
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


- (void)uninitialize_Cube
{
    //code


    //steps for uninitialize function in PP

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

    // delete texcoord vbo
    if (vbo_Texcoord_Cube)
    {
        glDeleteBuffers(1, &vbo_Texcoord_Cube);
        vbo_Texcoord_Cube = 0;
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

- (void)uninitialize_Sphere
{
    //code


    //steps for uninitialize function in PP

    if (shaderProgramObject_PV_Sphere)
    {
        //step 1 : use shader program object

        glUseProgram(shaderProgramObject_PV_Sphere);

        GLint numShaders = 0;

        //step 2 : get number of attached shaders
        glGetProgramiv(shaderProgramObject_PV_Sphere, GL_ATTACHED_SHADERS, &numShaders);



        if (numShaders > 0)
        {
            //step 3 : Create and allocate memory of shader objects according to number of attached shaders
            GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint));

            if (pShaders != NULL)
            {
                //step 4 : Get shader object into the allocated array.

                glGetAttachedShaders(shaderProgramObject_PV_Sphere, numShaders, NULL, pShaders);

                //step 5 : Detach and delete shader objects iteratively

                for (GLint i = 0; i < numShaders; i++)
                {
                    glDetachShader(shaderProgramObject_PV_Sphere, pShaders[i]);
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

        glDeleteProgram(shaderProgramObject_PV_Sphere);
        shaderProgramObject_PV_Sphere = 0;






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
    if (select_SPO == false)
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

    [self uninitialize_Cube] ;
    [self release] ;
    exit(0);


}

- (void)onLongPress:(UILongPressGestureRecognizer*)gestureRecognizer
{
    if (cubeAnimationEnable == false)
    {
        cubeAnimationEnable = true;
    }
    else
    {
        cubeAnimationEnable = false;
    }
    

}

- (void)dealloc
{
    //code

    [super dealloc] ;

    [self uninitialize_Cube] ;

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
