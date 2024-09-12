//
//  MyView.m
//  Window
//	
//  Created by user258815 on 8/4/24.
//

#import<OpenGLES/ES3/gl.h>

#import<OpenGLES/ES3/glext.h>

#import "GLESView.h"

@implementation GLESView
{
    EAGLContext *eaglContext;
    
    GLuint customeFrameBuffer;
    
    GLuint colorRenderBuffer;
    
    GLuint depthRenderBuffer;
    
    id displayLink;
    
    NSInteger framesPerSecond;
    
    bool isDisplayLink;
    
    
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
    //code
       
    [self printGLInfo];   // copy paste point
       
       
    // depth setting
       
    glClearDepthf(1.0f); // Compulsory
    glEnable(GL_DEPTH_TEST); // 8 test paike ek(depth) test enable kr
    glDepthFunc(GL_LEQUAL);
       
    //clear color
       
    glClearColor(0.0f,0.0f,1.0f,1.0f);
       
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
}

-(void)display
{
    //code
       
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
}

-(void)myUpdate
{
    //code
       
       
}

-(void)uninitialize
{
    //code
    
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
    
    
    
    
}

-(void)OndoubleTap:(UITapGestureRecognizer *)gestureRecognizer
{
    
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
