//
//  MyView.m
//  Window
//
//  Created by user258815 on 8/4/24.
//

#import "MyView.h"

@implementation MyView
{
    //@private
    
    NSString *string;
}

//-(id)initWithFrame:(CGRect)frame
-(id)initWithFrame:(CGRect)frame
{
    //code
    
    self = [super initWithFrame:frame];
    
    if(self)
    {
        //set the background to black
        
        [self setBackgroundColor:[UIColor whiteColor]];
        
        //set the string
        
        string = @"Hello World !!!";
        
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

-(void)drawRect:(CGRect)rect
{
    //declare the background color black
           
    UIColor *bgColor = [UIColor whiteColor];
           
    //Set this color
           
    [bgColor set];
           
    //fill the window rectangle / dirty rectangle with above black color
           
    UIRectFill(rect);
           
    //create the font for our string
           
    UIFont *stringFont = [UIFont fontWithName:@"Helvetica" size:24];
           
    //create Sting Color
           
    UIColor *fgColor = [UIColor greenColor];
           
    //create dicsnory for our string attributes for above two
           
    NSDictionary *stringDictionary = [NSDictionary dictionaryWithObjectsAndKeys:stringFont,NSFontAttributeName,fgColor,NSForegroundColorAttributeName,nil];
           
    //create the size of the string according to the size of the font and other attributes
           
    CGSize stringSize = [string sizeWithAttributes:stringDictionary];
           
    //create point for srawing of string
           
    CGPoint Point;
    
    Point.x = ((rect.size.width/2) - (stringSize.width/2));
    
    Point.y = ((rect.size.height/2) - (stringSize.height/2));
           
    //finally draw the string Hellow World !!
           
    [string drawAtPoint:Point withAttributes:stringDictionary];
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
    
    string = @"Single Tap";
    
    [self setNeedsDisplay];
    
    
}

-(void)OndoubleTap:(UITapGestureRecognizer *)gestureRecognizer
{
    string = @"Double Tap";
    
    [self setNeedsDisplay];
}

-(void)OnSwipe:(UISwipeGestureRecognizer *)gestureRecognizer
{
    //code
    
    string = @"Swipe";
    
    [self setNeedsDisplay];
}

-(void)onLongPress:(UILongPressGestureRecognizer *)gestureRecognizer
{
    //code
    
    string = @"Long Press";
    
    [self setNeedsDisplay];
    
}

-(void)dealloc
{
    //code
    
    [super dealloc];
}


@end
