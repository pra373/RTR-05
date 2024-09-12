//
//  MyView.m
//  Window
//
//  Created by user258815 on 8/4/24.
//

#define GLES_SILENCE_DEPRECATION

#import<OpenGLES/ES3/gl.h>

#import<OpenGLES/ES3/glext.h>

#import<iostream>

#import<vector>

#import "GLESView.h"

#import "vmath.h"

using namespace vmath;


enum
{
    AMC_ATTRIBUTE_POSITION = 0
    
};

enum
{
    AMC_ATTRIBUTE_POSITION_SHAPES = 0,
    AMC_ATTRIBUTE_COLOR_SHAPES

};

@implementation GLESView
{
    EAGLContext *eaglContext;
    
    GLuint customeFrameBuffer;
    
    GLuint colorRenderBuffer;
    
    GLuint depthRenderBuffer;
    
    id displayLink;
    
    NSInteger framesPerSecond;
    
    bool isDisplayLink;

    std::vector<GLfloat>linePositions;
    std::vector<GLfloat>circlePositions;
    std::vector<GLfloat>CircleColor;
    
    // OpenGL related field variables
    
    GLuint shaderProgramObject;
    GLuint shaderProgramObject_Shapes;

    GLuint vao;
    GLuint vbo_Position;

    GLuint mvpMatrixUniform;

    mat4 perspectiveProjectionMatrix;

    GLuint mvpMatrixUniform_Shapes;

    GLuint vbo_Position_Triangle;
    GLuint vbo_Color_Triangle;

    GLuint vbo_Position_Square;
    GLuint vbo_Color_Square;

    GLuint vbo_Position_Circle;
    GLuint vbo_Color_Circle;

    GLuint vao_Triangle;
    GLuint vao_Square;
    GLuint vao_Circle;

    GLint touchCount;

    bool enableGraph;
   

    
}

//-(id)initWithFrame:(CGRect)frame
-(id)initWithFrame:(CGRect)frame
{
    //code
    
    self = [super initWithFrame:frame];
    
    if(self)
    {
        //set the background to black
        
        [self setBackgroundColor:[UIColor blackColor]];
        
        // step 1 : create EAGL Layer
        
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)[super layer];
        
        [eaglLayer setOpaque:YES];
        
        NSDictionary *dictionary = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:NO],kEAGLDrawablePropertyRetainedBacking,kEAGLColorFormatRGBA8,kEAGLDrawablePropertyColorFormat, nil];
        
        [eaglLayer setDrawableProperties:dictionary];
        
        //step 2 : create EAGL Context
        
        eaglContext = [[EAGLContext alloc]initWithAPI:kEAGLRenderingAPIOpenGLES3];
        
        if(eaglContext == nil)
        {
            printf("OpenGL ES Context creation failed !!");
            [self uninitialize];
            [self release];
            exit(0);
        }
        
        //step 3 : set this context as current context
        
        [EAGLContext setCurrentContext:eaglContext];
        
        //step 4 : Create custome frame buffer
        
        glGenFramebuffers(1, &customeFrameBuffer);
        
        glBindFramebuffer(GL_FRAMEBUFFER, customeFrameBuffer);
        
        //create color render buffer
        
        glGenRenderbuffers(1, &colorRenderBuffer);
        
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
        
        //give storage to color render buffer by using IOS Method
        
        [eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
        
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
        
        if(framebuffer_Status != GL_FRAMEBUFFER_COMPLETE)
        {
            printf("Frame buffer creation failed !!");
            [self uninitialize];
            [self release];
            exit(0);
        }
        
        //initialize frames per second
        
        framesPerSecond = 60;  //value 60 is recomended from ios 8.2
        
        //initialize display link existance variable
        
        isDisplayLink = NO;
        
        
        
        //call our initialize method
        
        int result = [self initialize];
        
        if(result != 0)
        {
            printf("initialize function failed !!");
            [self uninitialize];
            [self release];
            exit(0);
        }
        
        
        
        //single tap
        
        //create object of single tap gesture recognizer object
        
        UITapGestureRecognizer * singleTapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget: self action:@selector(OnsingleTap:)];
        
        // set number of taps
        
        [singleTapGestureRecognizer setNumberOfTapsRequired:1];
        
        //set number of fingers
        
        [singleTapGestureRecognizer setNumberOfTouchesRequired:1];
        
        //set the deligate as self
        
        [singleTapGestureRecognizer setDelegate:self];
        
        //add this gesture recognizer to self
        
        [self addGestureRecognizer:singleTapGestureRecognizer];
        
        //double tap
        
        UITapGestureRecognizer * doubleTapGestureRecognizer = [[UITapGestureRecognizer alloc ]initWithTarget:self action:@selector(OndoubleTap:)];
        
        // set number of taps
        
        [doubleTapGestureRecognizer setNumberOfTapsRequired:2];
        
        //set number of fingers
        
        [doubleTapGestureRecognizer setNumberOfTouchesRequired:1];
        
        //set the deligate as self
        
        [doubleTapGestureRecognizer setDelegate:self];
        
        //add this gesture recognizer to self
        
        [self addGestureRecognizer:doubleTapGestureRecognizer];
        
        //double tap should fail for two single taps
        
        [singleTapGestureRecognizer requireGestureRecognizerToFail:doubleTapGestureRecognizer];
        
        //swipe
        
        UISwipeGestureRecognizer * swipeGestureRecognizer = [[UISwipeGestureRecognizer alloc]initWithTarget:self action:@selector(OnSwipe:)];
        
        [swipeGestureRecognizer setDelegate:self];
        
        [self addGestureRecognizer:swipeGestureRecognizer];
        
        //longpress
        
        UILongPressGestureRecognizer * longPressGestureRecognizer = [[UILongPressGestureRecognizer alloc]initWithTarget:self action:@selector(onLongPress:)];
        
        [longPressGestureRecognizer setDelegate:self];
        
        [self addGestureRecognizer:longPressGestureRecognizer];
        
        
        
    }
    
    return(self);
}



+(Class) layerClass
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
    
    [EAGLContext setCurrentContext:eaglContext];
    
    //bind with frame buffer again
    
    glBindBuffer(GL_FRAMEBUFFER, customeFrameBuffer);
    
    //call your display function
    
    [self display];
    
    //call update
    
    [self myUpdate];
    
    //bind with the color render buffer again
    
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
    
    //present this color render buffer which will internally do double buffering
    
    [eaglContext presentRenderbuffer:GL_RENDERBUFFER];
    
    
    
}

//override layout subviews method

-(void)layoutSubviews
{
    //code
    
    //bind with the color render buffer again
    
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
    
    //create color render buffer storage again
    
    [eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)[self layer]];
    
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
    
    if(framebuffer_Status != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("Frame buffer creation failed !!");
        [self uninitialize];
        [self release];
        exit(0);
    }
    
    //call our resize here
    
    [self resize:width height:height];
    
    //it is recomended that you should call draw view here
    
    [self drawView:displayLink];
    
    
    
    
}

// start display link custome method which will be called by app delegate

-(void)startDisplayLink
{
    //code
    
    if(isDisplayLink == NO)
    {
        //create display link
        
        displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawView:)];
        
        //set frame per seconds
        
        [displayLink setPreferredFramesPerSecond:framesPerSecond];
        
        //add this display link to our run loop
        
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        
        //set isdisplay link to yes
        
        isDisplayLink= YES;
    }
}

// stop display link custome method which will be called by appdelegate

-(void)stopDisplayLink
{
    if(isDisplayLink == YES)
    {
        //remove display link from runloop by invalidating it
        
        [displayLink invalidate];
        
        isDisplayLink = NO;
    }
}


-(int)initialize
{

       //variable declaration

     GLfloat upperX = 0.0f;
     GLfloat lowerX = 0.0f;
     GLfloat upperY = 1.0f;
     GLfloat lowerY = -1.0f;
    //code
       
    [self printGLInfo];   // copy paste point
    
    //vertex shader

          const GLchar* vertexShaderSourceCode =
              "#version 300 es\n"             //make verson to 300
              "\n"
              "in vec4 aPosition;\n"
              "out vec4 oColor;\n"
              "vec2 position = aPosition.xy;"
              "uniform mat4 uMVPMatrix;\n"          //u for uniform datatype
              "void main(void)\n"
              "{\n"
                 "gl_Position=uMVPMatrix * vec4(position,0.0,1.0);\n"
                 "if(gl_VertexID < 2)\n"
                 "{\n"
                     "oColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
                 "}\n"
                 "else if(gl_VertexID >= 2 && gl_VertexID <= 3 )\n"
                 "{\n"
                    "oColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
                 "}\n"
                 "else\n"
                 "{\n"
                     "oColor = vec4(0.0, 0.0, 1.0, 1.0);\n"
                 "}\n"
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
              [self release];
              exit(-1);
          }

          //follow all steps of vertex shader for fragment shader

          //step 6  : Write the source code for fragment shader

          //fragment shader

          const GLchar* fragmentShaderSourceCode =
              "#version 300 es\n"
              "\n"
              "precision highp float;\n"         // Specify precision for floating-point types
              "in vec4 oColor;"
              "out vec4 FragColor;\n"
              "void main(void)\n"
              "{\n"
                "FragColor = oColor;\n"                       //vec4 is working as a constructor here not as a datatype
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
               [self release];
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
               [self release];
               exit(-1);
               
           }

           //step 12: Declare position and color arrays

           //get Shader uniform Locations

           mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "uMVPMatrix");  //must be done after linkage of shader program

           GLfloat lineArray[8] = {0.0, 1.0, 0.0, -1.0, -1.0, 0.0, 1.0, 0.0};

           /*const GLfloat trianglePosition[] = { 0.0f, 1.0f, 0.0f,
                                               -1.0f,-1.0f,0.0f,
                                                1.0f,-1.0f,0.0f
                                              };*/

          
            for(int i=0; i<=7; i++)
            {
                linePositions.push_back(lineArray[i]);
            }

             for (int i = 0; i <= 39; i++)   //loop for +x vertical lines
            {
                upperX = upperX + 0.025f;
                lowerX = lowerX + 0.025f;

                linePositions.push_back(upperX);

                linePositions.push_back(upperY);

                linePositions.push_back(lowerX);

                linePositions.push_back(lowerY);

             
            }

             // ********************************** //
        upperX = 0.0f;
         lowerX = 0.0f;

         for (int i = 0; i <= 39; i++)   //loop for -X vertical lines
         {
             upperX = upperX - 0.025f;
             lowerX = lowerX - 0.025f;

             linePositions.push_back(upperX);

             linePositions.push_back(upperY);

             linePositions.push_back(lowerX);

             linePositions.push_back(lowerY);


         }

         
        // *********************************//
        GLfloat leftX = -1.0f;
         GLfloat rightX = 1.0f;
         
         GLfloat leftY = 0.0f;
         GLfloat rightY = 0.0f;

         for (int i = 0; i <= 39; i++)    //loop for +y vertical lines
         {
             leftY = leftY + 0.025f;
             rightY = rightY + 0.025f;

             linePositions.push_back(leftX);

             linePositions.push_back(leftY);

             linePositions.push_back(rightX);

             linePositions.push_back(rightY);


         }

         // ***************************************//
         leftX = -1.0f;
         rightX = 1.0f;

         leftY = 0.0f;
         rightY = 0.0f;

        for (int i = 0; i <= 39; i++)    //loop for -y vertical lines
        {
             leftY = leftY - 0.025f;
             rightY = rightY - 0.025f;

             linePositions.push_back(leftX);

             linePositions.push_back(leftY);

             linePositions.push_back(rightX);

             linePositions.push_back(rightY);

        }

           

           ////step 13: Create vertex array object
           //VAO

           glGenVertexArrays(1, &vao);

           //step 14: Bind with VAO

           glBindVertexArray(vao);

           //step 15: Create vertex buffer object (VBO) for position
           
           //VBO for position

           glGenBuffers(1, &vbo_Position);

           //step 16: Bind with VBO of position

           glBindBuffer(GL_ARRAY_BUFFER, vbo_Position);

           //glBufferData(GL_ARRAY_BUFFER, sizeof(trianglePosition), trianglePosition, GL_STATIC_DRAW);

           glBufferData(GL_ARRAY_BUFFER, linePositions.size()*sizeof(GLfloat), linePositions.data(), GL_STATIC_DRAW);

           glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);

           glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

           glBindBuffer(GL_ARRAY_BUFFER, 0);

           //step 18 : unbind with VAO
           glBindVertexArray(0);

           // ************* Shader Program For Shapes ************************* //
        
        const GLchar* vertexShaderSourceCode_Shapes =
             "#version 300 es\n"
             "in vec4 inputPosition;\n"
             "in vec4 inputColor;\n"
             "out vec4 FragColor;\n"
             "vec2 position = inputPosition.xy;"
             "uniform mat4 uMVPMatrixShapes;"
             "void main(void)\n"
             "{\n"
               "gl_Position = uMVPMatrixShapes * vec4(position, 0.0, 1.0);\n"
               "FragColor = inputColor;\n"
             "}\n";
        
        //step 2: Create vertex shader object

        GLuint vertexShaderObject_Shapes = glCreateShader(GL_VERTEX_SHADER);

         //step 3 : Give vertex shader code to OpenGL
         glShaderSource(vertexShaderObject_Shapes, 1, (const GLchar**)&vertexShaderSourceCode_Shapes, NULL);

         //Step 4 : Let OpenGL engine Compile the vertex shader code
         glCompileShader(vertexShaderObject_Shapes);

         status = 0;
         infoLogLength = 0;
         szInfoLog = NULL;

         //step 5 : Check for vertex shader compilation
         //step 5 (a) :  get compile status in status variable
         glGetShaderiv(vertexShaderObject_Shapes, GL_COMPILE_STATUS, &status);

         //step 5 (b) : if error occur i.e status == GL_FALSE,  get length of error message into infoLogLength variable
         if (status == GL_FALSE)
         {

             glGetShaderiv(vertexShaderObject_Shapes, GL_INFO_LOG_LENGTH, &infoLogLength);

             if (infoLogLength > 0)
             {
                 //step 5 (c) : declare a string variable allocate memory equal to above length i.e length of error message
                 szInfoLog = (GLchar*)malloc(infoLogLength);


                 if (szInfoLog != NULL)
                 {
                     // step 5 (d) : get the compilation error log info into memory allocated string
                     glGetShaderInfoLog(vertexShaderObject_Shapes, infoLogLength, NULL, szInfoLog);

                     //step 5 (e) : print error message into log file
                     printf("Vertex shader for shapes compilation error log : %s\n", szInfoLog);


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

         const GLchar* fragmentShaderSourceCode_Shapes =
         "#version 300 es\n"
         "\n"
         "precision highp float;\n"
         "in vec4 FragColor;\n"
         "out vec4 outputColor;\n"
         "void main(void)\n"
         "{\n"
         "outputColor = FragColor;\n"
         "}\n";

        GLuint fragmentShaderObject_Shapes = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(fragmentShaderObject_Shapes, 1, (const GLchar**)&fragmentShaderSourceCode_Shapes, NULL);

        glCompileShader(fragmentShaderObject_Shapes);

        status = 0;
        infoLogLength = 0;
        szInfoLog = NULL;

        glGetShaderiv(fragmentShaderObject_Shapes, GL_COMPILE_STATUS, &status);

        if (status == GL_FALSE)
        {

         glGetShaderiv(fragmentShaderObject_Shapes, GL_INFO_LOG_LENGTH, &infoLogLength);

         if (infoLogLength > 0)
         {
             szInfoLog = (GLchar*)malloc(infoLogLength);


             if (szInfoLog != NULL)
             {
                 glGetShaderInfoLog(fragmentShaderObject_Shapes, infoLogLength, NULL, szInfoLog);
                 printf("Fragment shader for shapes compilation error log : %s\n", szInfoLog);
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

             shaderProgramObject_Shapes = glCreateProgram();

             //step 8 : Attach both shaders into this program            also know as prelinking binding  should be done before linking and after attachment
             glAttachShader(shaderProgramObject_Shapes, vertexShaderObject_Shapes);
             glAttachShader(shaderProgramObject_Shapes, fragmentShaderObject_Shapes);

             //step 9: bind attribute location to the shader program object

             glBindAttribLocation(shaderProgramObject_Shapes, AMC_ATTRIBUTE_POSITION_SHAPES, "inputPosition");
             glBindAttribLocation(shaderProgramObject_Shapes, AMC_ATTRIBUTE_COLOR_SHAPES, "inputColor");


             //step 10: Link the shader program

             glLinkProgram(shaderProgramObject_Shapes);

             //step 11: do linking error check fpr shader program

             status = 0;
             infoLogLength = 0;
             szInfoLog = NULL;

             glGetProgramiv(shaderProgramObject_Shapes, GL_LINK_STATUS, &status);

             if (status == GL_FALSE)
             {
                 glGetProgramiv(shaderProgramObject_Shapes, GL_INFO_LOG_LENGTH, &infoLogLength);

                 if (infoLogLength > 0)
                 {
                     szInfoLog = (GLchar*)malloc(infoLogLength);


                     if (szInfoLog != NULL)
                     {
                         glGetProgramInfoLog(shaderProgramObject_Shapes, infoLogLength, NULL, szInfoLog);
                         printf("shader program for shapes linking error log : %s\n", szInfoLog);
                         free(szInfoLog);
                         szInfoLog = NULL;


                     }



                 }

                 [self uninitialize];
                 [self release];
                 exit(0);

             }
        
        //get Shader uniform Locations

         mvpMatrixUniform_Shapes = glGetUniformLocation(shaderProgramObject_Shapes, "uMVPMatrixShapes");  //must be done after linkage of shader program
         

         const GLfloat trianglePosition[] =
         {
            0.0f,0.5f,  //top vertex
            -0.5f,-0.5f, //left vertex

            -0.5f,-0.5f, //left vertex
            0.5f,-0.5f, //right vertex

            0.5f,-0.5f, //right vertex
            0.0f,0.5f,  //top vertex

         };

         const GLfloat triangleColor[] =
         {
             1.0f,1.0f,0.0f,
             1.0f,1.0f,0.0f,
             1.0f,1.0f,0.0f,
             1.0f,1.0f,0.0f,
             1.0f,1.0f,0.0f,
             1.0f,1.0f,0.0f
         };

          const GLfloat squarePosition[] =
         {
             0.5f,0.5f, //right top
             -0.5f,0.5f, //left top

             -0.5f,0.5f, //left top
             -0.5f,-0.5f, //left bottom

              -0.5f,-0.5f, //left bottom
             0.5f,-0.5f,   //right bottom

              0.5f,-0.5f,   //right bottom
              0.5f,0.5f     //right top


         };

         const GLfloat squareColor[] =
         {
             1.0f,1.0f,0.0f,
             1.0f,1.0f,0.0f,
             1.0f,1.0f,0.0f,
             1.0f,1.0f,0.0f,
             1.0f,1.0f,0.0f,
             1.0f,1.0f,0.0f,
             1.0f,1.0f,0.0f,
             1.0f,1.0f,0.0f
         };
        
        //code to find circle positions

         GLfloat radious = 0.5f;

         for (GLfloat angle = 0.0f; angle <= 360.0f; angle = angle + 0.01f)
         {

             //float radianangle = angle * (M_PI * 180.0f);
             GLfloat x = radious * cos((angle * M_PI) / 180.0f) + (0.0f);
             GLfloat y = radious * sin((angle * M_PI) / 180.0f) + (0.0f);

             circlePositions.push_back(x);
             circlePositions.push_back(y);


         }

         for (int i = 0; i < circlePositions.size(); i++)
         {
             CircleColor.push_back(1.0f);
             CircleColor.push_back(1.0f);
             CircleColor.push_back(0.0f);
         }
        

          ////step 13: Create vertex array object
        //VAO

         glGenVertexArrays(1, &vao_Triangle);

         //step 14: Bind with VAO

         glBindVertexArray(vao_Triangle);

         //step 15: Create vertex buffer object (VBO) for position

         //VBO for position

         glGenBuffers(1, &vbo_Position_Triangle);

         //step 16: Bind with VBO of position

         glBindBuffer(GL_ARRAY_BUFFER, vbo_Position_Triangle);

         glBufferData(GL_ARRAY_BUFFER, sizeof(trianglePosition), trianglePosition, GL_STATIC_DRAW);

         glVertexAttribPointer(AMC_ATTRIBUTE_POSITION_SHAPES, 2, GL_FLOAT, GL_FALSE, 0, NULL);

         glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION_SHAPES);

         glBindBuffer(GL_ARRAY_BUFFER, 0);

         //vbo for color

         glGenBuffers(1, &vbo_Color_Triangle);

         //step 16: Bind with VBO of position

         glBindBuffer(GL_ARRAY_BUFFER, vbo_Color_Triangle);

         glBufferData(GL_ARRAY_BUFFER, sizeof(triangleColor), triangleColor, GL_STATIC_DRAW);

         glVertexAttribPointer(AMC_ATTRIBUTE_COLOR_SHAPES, 3, GL_FLOAT, GL_FALSE, 0, NULL);

         glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR_SHAPES);

         glBindBuffer(GL_ARRAY_BUFFER, 0);

         //vao for square
          ////step 13: Create vertex array object
        //VAO

         glGenVertexArrays(1, &vao_Square);

         //step 14: Bind with VAO

         glBindVertexArray(vao_Square);

         //step 15: Create vertex buffer object (VBO) for position

         //VBO for position

         glGenBuffers(1, &vbo_Position_Square);

         //step 16: Bind with VBO of position

         glBindBuffer(GL_ARRAY_BUFFER, vbo_Position_Square);

         glBufferData(GL_ARRAY_BUFFER, sizeof(squarePosition), squarePosition, GL_STATIC_DRAW);

         glVertexAttribPointer(AMC_ATTRIBUTE_POSITION_SHAPES, 2, GL_FLOAT, GL_FALSE, 0, NULL);

         glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION_SHAPES);

         glBindBuffer(GL_ARRAY_BUFFER, 0);

         //vbo for color

         glGenBuffers(1, &vbo_Color_Square);

         //step 16: Bind with VBO of position

         glBindBuffer(GL_ARRAY_BUFFER, vbo_Color_Square);

         glBufferData(GL_ARRAY_BUFFER, sizeof(squareColor), squareColor, GL_STATIC_DRAW);

         glVertexAttribPointer(AMC_ATTRIBUTE_COLOR_SHAPES, 3, GL_FLOAT, GL_FALSE, 0, NULL);

         glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR_SHAPES);

         glBindBuffer(GL_ARRAY_BUFFER, 0);


         //step 18 : unbind with VAO
         glBindVertexArray(0);
        
        //VAO for circle

         glGenVertexArrays(1, &vao_Circle);

         //step 14: Bind with VAO

         glBindVertexArray(vao_Circle);

         //step 15: Create vertex buffer object (VBO) for position

         //VBO for position

         glGenBuffers(1, &vbo_Position_Circle);

         //step 16: Bind with VBO of position

         glBindBuffer(GL_ARRAY_BUFFER, vbo_Position_Circle);

         glBufferData(GL_ARRAY_BUFFER, circlePositions.size()*sizeof(float), circlePositions.data(), GL_STATIC_DRAW);

         glVertexAttribPointer(AMC_ATTRIBUTE_POSITION_SHAPES, 2, GL_FLOAT, GL_FALSE, 0, NULL);

         glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION_SHAPES);

         glBindBuffer(GL_ARRAY_BUFFER, 0);

         //vbo for color

         glGenBuffers(1, &vbo_Color_Circle);

         //step 16: Bind with VBO of position

         glBindBuffer(GL_ARRAY_BUFFER, vbo_Color_Circle);

         glBufferData(GL_ARRAY_BUFFER, CircleColor.size() * sizeof(float), CircleColor.data(), GL_STATIC_DRAW);

         glVertexAttribPointer(AMC_ATTRIBUTE_COLOR_SHAPES, 3, GL_FLOAT, GL_FALSE, 0, NULL);

         glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR_SHAPES);

         glBindBuffer(GL_ARRAY_BUFFER, 0);


         //step 18 : unbind with VAO
         glBindVertexArray(0);
       

        
       
    // depth setting
       
    glClearDepthf(1.0f); // Compulsory
    glEnable(GL_DEPTH_TEST); // 8 test paike ek(depth) test enable kr
    glDepthFunc(GL_LEQUAL);
       
    //clear color
       
    glClearColor(0.0f,0.0f,0.0f,1.0f);
       
    return 0;
}

-(void)printGLInfo
{
    //code
       
    printf("OpenGL Vendor : %s\n\n", glGetString(GL_VENDOR));

    printf("OpenGL Renderer : %s\n\n", glGetString(GL_RENDERER));

    printf("OpenGL Version : %s\n\n", glGetString(GL_VERSION));

    printf("Graphic Library Shading Language  (GLSL) Version : %s\n\n",glGetString(GL_SHADING_LANGUAGE_VERSION));

}

-(void)resize:(int)width height:(int)height
{
    //code
       
    if (height <= 0)
    {
        height = 1;
    }
                   


    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    
    perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

-(void)display
{
    //code
       
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //step 1: Use shader program object

    if(enableGraph == true)
    {
         glUseProgram(shaderProgramObject);
    
    
        //transformations

        mat4 modelViewMatrix = vmath::translate(0.0f, 0.0f, -3.0f);

        mat4 madelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;  //order of multiplication is not commutativ. i.e mat1 * mat2 != mat2 * mat1

        //push above mvp into vertex shader's mvp uniform

        glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, madelViewProjectionMatrix);

        glBindVertexArray(vao);

        //step 3: Draw the geometry

        //glDrawArrays(GL_TRIANGLES, 0, 3);

        glDrawArrays(GL_LINES, 0, 324);

        //step 4: unBind with (VAO)

        glBindVertexArray(0);
    }

   

     // Triangle
        
        if (enableGraph == true && touchCount == 1)
        {
            glUseProgram(shaderProgramObject_Shapes);

            mat4 modelViewMatrixShapes = mat4::identity();

            mat4 madelViewProjectionMatrixShapes = mat4::identity();

            modelViewMatrixShapes = vmath::translate(0.0f, 0.0f, -2.5f);

            madelViewProjectionMatrixShapes = perspectiveProjectionMatrix * modelViewMatrixShapes;  //order of multiplication is not commutativ. i.e mat1 * mat2 != mat2 * mat1

            //push above mvp into vertex shader's mvp uniform

            glUniformMatrix4fv(mvpMatrixUniform_Shapes, 1, GL_FALSE, madelViewProjectionMatrixShapes);

            glBindVertexArray(vao_Triangle);

           
            
            glDrawArrays(GL_LINES, 0, 6);

            glBindVertexArray(0);

            glUseProgram(0);


        }

        // Square
        
        if (enableGraph == true && touchCount == 2)
        {
            glUseProgram(shaderProgramObject_Shapes);

            mat4 modelViewMatrixShapes = mat4::identity();

            mat4 madelViewProjectionMatrixShapes = mat4::identity();

            modelViewMatrixShapes = vmath::translate(0.0f, 0.0f, -2.5f);

            madelViewProjectionMatrixShapes = perspectiveProjectionMatrix * modelViewMatrixShapes;  //order of multiplication is not commutativ. i.e mat1 * mat2 != mat2 * mat1

            //push above mvp into vertex shader's mvp uniform

            glUniformMatrix4fv(mvpMatrixUniform_Shapes, 1, GL_FALSE, madelViewProjectionMatrixShapes);

            glBindVertexArray(vao_Square);

           
            
            glDrawArrays(GL_LINES, 0, 8);

            glBindVertexArray(0);

            glUseProgram(0);
        }
        
        
        // Circle
        
        if (enableGraph == true && touchCount == 3)
        {
            glUseProgram(shaderProgramObject_Shapes);

            mat4 modelViewMatrixShapes = mat4::identity();

            mat4 madelViewProjectionMatrixShapes = mat4::identity();

            modelViewMatrixShapes = vmath::translate(0.0f, 0.0f, -2.5f);

            madelViewProjectionMatrixShapes = perspectiveProjectionMatrix * modelViewMatrixShapes;  //order of multiplication is not commutativ. i.e mat1 * mat2 != mat2 * mat1

            //push above mvp into vertex shader's mvp uniform

            glUniformMatrix4fv(mvpMatrixUniform_Shapes, 1, GL_FALSE, madelViewProjectionMatrixShapes);

            glBindVertexArray(vao_Circle);

            

            glDrawArrays(GL_POINTS, 0, (circlePositions.size()/2));

            

            glBindVertexArray(0);

            glUseProgram(0);
        }
       
    
    //step 5: Unuse shader program object
    glUseProgram(0);
       
}

-(void)myUpdate
{
    //code
       
       
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
    
    if (vbo_Position)
    {
        glDeleteBuffers(1, &vbo_Position);
        vbo_Position = 0;
    }
    
    //step 11 : delete VAO
    
    if (vao)
    {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
    
    
    // free depth render buffer
    
    if(depthRenderBuffer)
    {
        glDeleteRenderbuffers(1, (GLuint *)&depthRenderBuffer);
        depthRenderBuffer = 0;
    }
    
    // delete colore render buffer
    
    if(colorRenderBuffer)
    {
        glDeleteRenderbuffers(1, &colorRenderBuffer);
        colorRenderBuffer = 0;
    }
    
    //delete custome frame buffer
    
    if(customeFrameBuffer)
    {
        glDeleteFramebuffers(1, &customeFrameBuffer);
        customeFrameBuffer = 0;
    }
    
    //release eagl context
    
    if(eaglContext && [EAGLContext currentContext] == eaglContext)
    {
        [EAGLContext setCurrentContext:nil];
        
        [eaglContext release];
        
        eaglContext = nil;
    }
       
}
-(BOOL)becomeFirstResponder
{
    //code
    
    return(YES);
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    //code
}


-(void)OnsingleTap:(UITapGestureRecognizer *)gestureRecognizer
{
    //code
    
    touchCount++;

    if(touchCount > 4)
    {
        touchCount = 0;
    }


    
    
    
}

-(void)OndoubleTap:(UITapGestureRecognizer *)gestureRecognizer
{
    if(enableGraph == false)
    {
        enableGraph =true;
    }
    else{

        enableGraph =false;
    }
}

-(void)OnSwipe:(UISwipeGestureRecognizer *)gestureRecognizer
{
    //code
    
    [self uninitialize];
    [self release];
    exit(0);
    
    
}

-(void)onLongPress:(UILongPressGestureRecognizer *)gestureRecognizer
{
    
    
}

-(void)dealloc
{
    //code
    
    [super dealloc];
    
    [self uninitialize];
    
    // release display link
    
    if(displayLink)
    {
        [displayLink invalidate];
        
        [displayLink stop];
        
        [displayLink release];
        
        displayLink = nil;
    }
}


@end
